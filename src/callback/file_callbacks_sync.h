#ifndef GODOTFMOD_FILE_CALLBACKS_SYNC_H
#define GODOTFMOD_FILE_CALLBACKS_SYNC_H

#ifdef WEB_ENABLED

#include <fmod_common.h>

namespace Callbacks {
    FMOD_RESULT F_CALL
    godotFileRead(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata);
    FMOD_RESULT F_CALL godotFileSeek(void* handle, unsigned int pos, void* userdata);
} // namespace Callbacks

#endif

#endif // GODOTFMOD_FILE_CALLBACKS_SYNC_H
