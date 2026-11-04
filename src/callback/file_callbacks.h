#ifndef GODOTFMOD_FILE_CALLBACKS_H
#define GODOTFMOD_FILE_CALLBACKS_H

#include <fmod_common.h>

#include <classes/file_access.hpp>
#include <classes/ref.hpp>

// FMOD reads files through Godot's FileAccess, so banks load from res:// and from exported packs alike. Opening and
// closing are shared; reading is synchronous on the web (file_callbacks_sync) and asynchronous elsewhere
// (file_callbacks_async).
namespace Callbacks {
    struct GodotFileHandle {
        godot::Ref<godot::FileAccess> file;
    };

    FMOD_RESULT F_CALL godotFileOpen(const char* name, unsigned int* filesize, void** handle, void* userdata);
    FMOD_RESULT F_CALL godotFileClose(void* handle, void* userdata);
}// namespace Callbacks

#endif// GODOTFMOD_FILE_CALLBACKS_H
