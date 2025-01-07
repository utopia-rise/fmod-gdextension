#ifdef TOOLS_ENABLED

#include "fmod_editor_export_plugin.h"

#include <helpers/common.h>
#include <helpers/files.h>

#include <classes/dir_access.hpp>
#include <classes/file_access.hpp>

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
}

String FmodEditorExportPlugin::_get_name() const {
    return "FmodEditorExportPlugin";
}

void FmodEditorExportPlugin::_bind_methods() {}

#endif