#ifdef TOOLS_ENABLED

#include "fmod_editor_export_plugin.h"
#include "fmod_server.h"
#include <plugins/plugins_helper.h>

#include <helpers/common.h>
#include <helpers/files.h>

using namespace godot;

constexpr const char* FMOD_FILE_EXTENSIONS[4] {".bank", ".ogg", ".mp3", ".wav"};
constexpr const char* ANDROID_BUILD_DIRS[2] = { "res://android/build", "res:///android/build" };

void FmodEditorExportPlugin::_export_begin(const PackedStringArray& features, bool is_debug, const String& path, uint32_t flags) {
    PackedStringArray excluded_folders;
    for (const char* dir : ANDROID_BUILD_DIRS) {
        excluded_folders.append(dir);
    }
    for (const char* extension : FMOD_FILE_EXTENSIONS) {
        PackedStringArray files;
        list_files_in_folder(files, "res://", extension, excluded_folders);
        for (const String& file : files) {
            GODOT_LOG_VERBOSE(vformat("Adding %s to pck", file));
            add_file(file, FileAccess::get_file_as_bytes(file), false);
        }
    }

    bool is_windows_export = features.has("windows");
    bool is_linux_export = features.has("linux");
    bool is_macos_export = features.has("macos");
    bool is_ios_export = features.has("ios");

    Ref<FmodPluginsSettings> plugins_settings = FmodPluginsSettings::get_from_project_settings();

    if (is_macos_export) {
        Vector<String> plugins_paths = get_fmod_plugins_libraries_paths(plugins_settings, "macos");
        for (const String& plugin_path : plugins_paths) {
            add_macos_plugin_file(plugin_path);
        }
    } else if (is_linux_export || is_windows_export) {
        String target_dir = path.get_base_dir();
        String os_name = is_linux_export ? "linux" : "windows";
        Vector<String> plugins_paths = get_fmod_plugins_libraries_paths(plugins_settings, os_name);

        Ref<DirAccess> dir_access = DirAccess::open(target_dir);
        for (const String& plugin_path : plugins_paths) {
            GODOT_LOG_INFO(vformat("Will copy %s to %s", plugin_path, target_dir));
            Ref<FileAccess> file_access = FileAccess::open(plugin_path, FileAccess::READ);
            dir_access->copy(plugin_path, target_dir.path_join(file_access->get_path().get_file()));
        }
    } else if (is_ios_export) {
        Vector<String> plugins_paths = get_fmod_plugins_libraries_paths(plugins_settings, "ios");
        for (const String& plugin_path : plugins_paths) {
            add_ios_project_static_lib(plugin_path);
        }

        String cpp_code_declaration = R"(
#include<stdint.h>
#include<cstdlib>

struct FMOD_DSP_DESCRIPTION;
struct FMOD_CODEC_DESCRIPTION;
struct FMOD_OUTPUT_DESCRIPTION;

typedef void* FMOD_SYSTEM_PTR;

typedef uint32_t (*REGISTER_DSP_METHOD)(FMOD_SYSTEM_PTR system, FMOD_DSP_DESCRIPTION* description, uint32_t* handle);
typedef uint32_t (*REGISTER_CODEC_METHOD)(FMOD_SYSTEM_PTR system, FMOD_CODEC_DESCRIPTION* description, uint32_t* handle);
typedef uint32_t (*REGISTER_OUTPUT_METHOD)(FMOD_SYSTEM_PTR system, FMOD_OUTPUT_DESCRIPTION* description, uint32_t* handle);

typedef struct {
    FMOD_SYSTEM_PTR system;
    REGISTER_DSP_METHOD register_dsp_method;
    REGISTER_CODEC_METHOD register_codec_method;
    REGISTER_OUTPUT_METHOD register_output_method;
} FMOD_IOS_INTERFACE;
        )";

        String cpp_code_external_plugin_declaration = "extern \"C\" {\n";
        String cpp_code_load_method = R"(
extern "C" __attribute__((visibility("default"))) __attribute__((used)) uint32_t* load_all_fmod_plugins(FMOD_IOS_INTERFACE* p_interface, uint32_t* r_count) {
    FMOD_SYSTEM_PTR fmod_system = p_interface->system;
    uint32_t handle;
    uint32_t* handles = reinterpret_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * %s));

)";
        const Array& static_plugins_settings = plugins_settings->get_static_plugins_settings();
        int count = 0;
        for (int i = 0; i < static_plugins_settings.size(); ++i) {
            const Ref<FmodStaticPluginSetting>& static_plugin_setting = static_plugins_settings[i];
            const Array& plugin_methods = static_plugin_setting->get_methods();

            for (int j = 0; j < plugin_methods.size(); ++j) {
                const Ref<FmodStaticPluginMethod>& plugin_method = plugin_methods[j];
                const String& method_name = plugin_method->get_method_name();
                switch (plugin_method->get_type()) {
                    case FmodStaticPluginMethod::CODEC:
                        cpp_code_external_plugin_declaration +=
                                vformat("    FMOD_CODEC_DESCRIPTION* %s();\n", method_name);
                        cpp_code_load_method +=
                                vformat("    p_interface->register_codec_method(fmod_system, %s(), &handle);\n", method_name);
                        break;
                    case FmodStaticPluginMethod::DSP:
                        cpp_code_external_plugin_declaration +=
                                vformat("    FMOD_DSP_DESCRIPTION* %s();\n", method_name);
                        cpp_code_load_method +=
                                vformat("    p_interface->register_dsp_method(fmod_system, %s(), &handle);\n", method_name);
                        break;
                    case FmodStaticPluginMethod::OUTPUT:
                        cpp_code_external_plugin_declaration +=
                                vformat("    FMOD_OUTPUT_DESCRIPTION* %s();\n", method_name);
                        cpp_code_load_method +=
                                vformat("    p_interface->register_output_method(fmod_system, %s(), &handle);\n", method_name);
                        break;
                    case FmodStaticPluginMethod::COUNT:
                        break;
                }
                cpp_code_load_method += vformat("handles[%s] = handle;\n", count);
                ++count;
            }
        }

        cpp_code_external_plugin_declaration += "}\n";
        cpp_code_load_method += R"(
    *r_count = %s;
    return handles;
}
)";
        add_ios_cpp_code(
                vformat(
                        "%s%s%s",
                        cpp_code_declaration,
                        cpp_code_external_plugin_declaration,
                        vformat(cpp_code_load_method, count, count)
                )
        );
    }
}

String FmodEditorExportPlugin::_get_name() const {
    return "FmodEditorExportPlugin";
}

void FmodEditorExportPlugin::_bind_methods() {}

#endif