#ifdef TOOLS_ENABLED

#include "fmod_editor_export_plugin.h"

#include <helpers/common.h>

#include <classes/dir_access.hpp>
#include <classes/file_access.hpp>

using namespace godot;

constexpr const char* FMOD_FILE_EXTENSIONS[4] {".bank", ".ogg", ".mp3", ".wav"};

void FmodEditorExportPlugin::_export_begin(const PackedStringArray& features, bool is_debug, const String& path, uint32_t flags) {
    for (const char* extension : FMOD_FILE_EXTENSIONS) {
        PackedStringArray files;
        _list_files_in_folder(files, "res://", extension);
        for (const String& file : files) {
            GODOT_LOG_INFO(vformat("Adding %s to pck", file));
            add_file(file, FileAccess::get_file_as_bytes(file), false);
        }
    }
}

String FmodEditorExportPlugin::_get_name() const {
    return "FmodEditorExportPlugin";
}

void FmodEditorExportPlugin::_list_files_in_folder(PackedStringArray& result, const String& folder, const String& extension) {
    Ref<DirAccess> folder_access {DirAccess::open(folder)};
    folder_access->list_dir_begin();
    String current_file{folder_access->get_next()};
    while (!current_file.is_empty()) {
        if (current_file == ".." || current_file == ".") {
            current_file = folder_access->get_next();
            continue;
        }

        String current_file_path{vformat("%s/%s", folder, current_file)};
        if (folder_access->current_is_dir()) {
            _list_files_in_folder(result, current_file_path, extension);
        } else if (current_file.ends_with(extension)) {
            result.push_back(current_file_path);
        }

        current_file = folder_access->get_next();
    }
    folder_access->list_dir_end();
}

void FmodEditorExportPlugin::_bind_methods() {}

#endif