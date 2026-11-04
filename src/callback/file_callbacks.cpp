#include "file_callbacks.h"

namespace Callbacks {
    FMOD_RESULT F_CALL godotFileOpen(const char* name, unsigned int* filesize, void** handle, void* userdata) {
        godot::Ref<godot::FileAccess> access = godot::FileAccess::open(name, godot::FileAccess::ModeFlags::READ);

        if (access.is_valid() && access->get_error() == godot::Error::OK) {
            *filesize = access->get_length();
            GodotFileHandle* fileHandle {new GodotFileHandle {access}};
            *handle = reinterpret_cast<void*>(fileHandle);
            return FMOD_RESULT::FMOD_OK;
        }
        *handle = nullptr;
        return FMOD_RESULT::FMOD_ERR_FILE_NOTFOUND;
    }

    FMOD_RESULT F_CALL godotFileClose(void* handle, void* userdata) {
        // FMOD closes a file even when opening it failed.
        if (handle == nullptr) { return FMOD_RESULT::FMOD_OK; }
        godot::Ref<godot::FileAccess> file {reinterpret_cast<GodotFileHandle*>(handle)->file};
        delete reinterpret_cast<GodotFileHandle*>(handle);
        return FMOD_RESULT::FMOD_OK;
    }
} // namespace Callbacks
