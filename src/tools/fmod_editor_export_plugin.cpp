#ifdef TOOLS_ENABLED

#include "fmod_editor_export_plugin.h"
#include "fmod_server.h"

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

    bool is_windows_export {features.has("windows")};
    bool is_linux_export {features.has("linux")};
    bool is_macos_export {features.has("macos")};

    Vector<String> plugins_paths = FmodServer::get_plugins_libraries_paths(
            FmodPluginsSettings::get_from_project_settings()
    );

    if (is_macos_export) {
        for (const String& plugin_path : plugins_paths) {
            add_macos_plugin_file(plugin_path);
        }
    } else if (is_linux_export || is_windows_export) {
        String target_dir = path.get_base_dir();

        Ref<DirAccess> dir_access = DirAccess::open(target_dir);
        for (const String& plugin_path : plugins_paths) {
            GODOT_LOG_INFO(vformat("Will copy %s to %s", plugin_path, target_dir));
            Ref<FileAccess> file_access = FileAccess::open(plugin_path, FileAccess::READ);
            dir_access->copy(plugin_path, target_dir.path_join(file_access->get_path().get_file()));
        }
    }
}

String FmodEditorExportPlugin::_get_name() const {
    return "FmodEditorExportPlugin";
}

void FmodEditorExportPlugin::_bind_methods() {}

#endif