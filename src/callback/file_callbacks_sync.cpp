#ifdef WEB_ENABLED

#include "file_callbacks_sync.h"

#include "file_callbacks.h"

namespace Callbacks {
    // The web reads files synchronously: FMOD's HTML5 libraries only support asynchronous reads through Asyncify, which
    // a side module cannot use, and a build without threads aborts when a thread starts.
    FMOD_RESULT F_CALL
    godotFileRead(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata) {
        godot::Ref<godot::FileAccess> file {reinterpret_cast<GodotFileHandle*>(handle)->file};
        *bytesread = static_cast<unsigned int>(file->get_buffer(static_cast<uint8_t*>(buffer), sizebytes));
        return *bytesread < sizebytes ? FMOD_RESULT::FMOD_ERR_FILE_EOF : FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotFileSeek(void* handle, unsigned int pos, void* userdata) {
        reinterpret_cast<GodotFileHandle*>(handle)->file->seek(pos);
        return FMOD_RESULT::FMOD_OK;
    }
} // namespace Callbacks

#endif
