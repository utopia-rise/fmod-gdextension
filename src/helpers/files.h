#ifndef GODOTFMOD_FILES_H
#define GODOTFMOD_FILES_H

#include <classes/dir_access.hpp>
#include <variant/packed_string_array.hpp>

namespace godot {
    static void list_files_in_folder(PackedStringArray& result, const String& folder, const String& extension = "",
                                     const PackedStringArray& excluded_folders = PackedStringArray()) {
        for (const String& excluded : excluded_folders) {
            if (folder == excluded) {
                return;
            }
        }

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
                list_files_in_folder(result, current_file_path, extension, excluded_folders);
            } else if (current_file.ends_with(extension)) {
                result.push_back(current_file_path);
            }

            current_file = folder_access->get_next();
        }
        folder_access->list_dir_end();
    }
}

#endif// GODOTFMOD_FILES_H
