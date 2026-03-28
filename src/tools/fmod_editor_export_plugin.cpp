#ifdef TOOLS_ENABLED

#include "fmod_editor_export_plugin.h"
#include "fmod_server.h"
#include <plugins/plugins_helper.h>

#include <helpers/common.h>
#include <helpers/files.h>

using namespace godot;

constexpr const char* FMOD_FILE_EXTENSIONS[4] {".bank", ".ogg", ".mp3", ".wav"};
constexpr const char* ANDROID_BUILD_DIRS[2] = { "res://android/build", "res:///android/build" };
constexpr const char* FMOD_AUTO_EXPORT_BANKS_SETTINGS_KEY = "fmod/auto_export_banks";

void FmodEditorExportPlugin::_export_begin(const PackedStringArray& features, bool is_debug, const String& path, uint32_t flags) {

    if (get_option(FMOD_AUTO_EXPORT_BANKS_SETTINGS_KEY) != Variant(false)) {

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

    }

    bool is_windows_export = features.has("windows");
    bool is_linux_export = features.has("linux");
    bool is_macos_export = features.has("macos");
    bool is_ios_export = features.has("ios");
    bool is_android_export = features.has("android");

    Ref<FmodPluginsSettings> plugins_settings = FmodPluginsSettings::get_from_project_settings();

    if (is_macos_export) {
        PackedStringArray plugins_libraries_path = _get_libraries_to_export(plugins_settings, "macos", ".dylib");
        for (const String& library_path : plugins_libraries_path) {
            add_macos_plugin_file(library_path);
        }
    } else if (is_linux_export || is_windows_export) {
        String target_dir = path.get_base_dir();
        String os_name = is_linux_export ? "linux" : "windows";
        String extension = is_linux_export ? ".so" : ".dll";
        PackedStringArray plugins_libraries_path = _get_libraries_to_export(plugins_settings, os_name, extension);

        Ref<DirAccess> dir_access = DirAccess::open(target_dir);

        if (dir_access.is_null()) {
            GODOT_LOG_ERROR(vformat("Failed to open target directory: %s", target_dir));
            return;
        }

        for (const String& library_path : plugins_libraries_path) {
            GODOT_LOG_INFO(vformat("Will copy %s to %s", library_path, target_dir));
            Ref<FileAccess> file_access = FileAccess::open(library_path, FileAccess::READ);

            if (file_access.is_null()) {
                GODOT_LOG_ERROR(vformat("Failed to open library file: %s", library_path));
                continue;
            }

            dir_access->copy(library_path, target_dir.path_join(file_access->get_path().get_file()));
        }
    } else if (is_ios_export) {
        PackedStringArray plugins_libraries_path = _get_libraries_to_export(plugins_settings, "ios", ".a");
        for (const String& library_path : plugins_libraries_path) {
            add_ios_project_static_lib(library_path);
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
        const Array& plugin_methods = plugins_settings->get_static_plugins_methods();
        int64_t method_count = plugin_methods.size();
        for (int i = 0; i < method_count; ++i) {
            const Ref<FmodStaticPluginMethod>& plugin_method = plugin_methods[i];
            const String& method_name = plugin_method->get_method_name();
            switch (plugin_method->get_type()) {
                case FmodStaticPluginMethod::CODEC:
                    cpp_code_external_plugin_declaration +=
                            vformat("    FMOD_CODEC_DESCRIPTION* %s();\n", method_name);
                    cpp_code_load_method +=
                            vformat("    p_interface->register_codec_method(fmod_system, %s(), &handle);\n", method_name);
                    cpp_code_load_method += vformat("    handles[%s] = handle;\n", i);
                    break;
                case FmodStaticPluginMethod::DSP:
                    cpp_code_external_plugin_declaration +=
                            vformat("    FMOD_DSP_DESCRIPTION* %s();\n", method_name);
                    cpp_code_load_method +=
                            vformat("    p_interface->register_dsp_method(fmod_system, %s(), &handle);\n", method_name);
                    cpp_code_load_method += vformat("    handles[%s] = handle;\n", i);
                    break;
                case FmodStaticPluginMethod::OUTPUT:
                    cpp_code_external_plugin_declaration +=
                            vformat("    FMOD_OUTPUT_DESCRIPTION* %s();\n", method_name);
                    cpp_code_load_method +=
                            vformat("    p_interface->register_output_method(fmod_system, %s(), &handle);\n", method_name);
                    cpp_code_load_method += vformat("    handles[%s] = handle;\n", i);
                    break;
                case FmodStaticPluginMethod::COUNT:
                    break;
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
                        vformat(cpp_code_load_method, method_count, method_count)
                )
        );
    } else if (is_android_export) {
        bool is_x86 = features.has("x86_64");
        bool is_arm64 = features.has("arm64");

        if (is_x86) {
            PackedStringArray plugins_libraries_path = _get_libraries_to_export(plugins_settings, "android", ".so", "x86_64");

            PackedStringArray tags;
            tags.append("x86_64");

            for (const String& library_path : plugins_libraries_path) {
                add_shared_object(library_path, tags, String());
            }
        }

        if (is_arm64) {
            PackedStringArray plugins_libraries_path = _get_libraries_to_export(plugins_settings, "android", ".so", "arm64");

            PackedStringArray tags;
            tags.append("arm64-v8a");

            for (const String& library_path : plugins_libraries_path) {
                add_shared_object(library_path, tags, String());
            }
        }
    }
}

String FmodEditorExportPlugin::_get_name() const {
    return "FmodEditorExportPlugin";
}

TypedArray<Dictionary> FmodEditorExportPlugin::_get_export_options(const Ref<EditorExportPlatform>& platform) const {
    TypedArray<Dictionary> options;

    {
        Dictionary option_dict;
        Dictionary option;
        option["name"] = FMOD_AUTO_EXPORT_BANKS_SETTINGS_KEY;
        option["type"] = Variant::BOOL;
        
        option_dict["option"] = option;
        option_dict["default_value"] = true;
        option_dict["update_visibility"] = true;
        
        options.append(option_dict);
    }
    
    return options;
}

void FmodEditorExportPlugin::_bind_methods() {}

PackedStringArray FmodEditorExportPlugin::_get_libraries_to_export(const Ref<FmodPluginsSettings>& settings, const String& p_os_name, const String& p_extension, const String& p_arch) {
    PackedStringArray result;

    String plugins_libraries_path = get_plugins_os_directory(settings, p_os_name, p_arch);
    list_files_in_folder(result, plugins_libraries_path, p_extension);

    return result;
}

#endif
