#ifndef GODOTFMOD_GODOT_FMOD_CALLBACK_H
#define GODOTFMOD_GODOT_FMOD_CALLBACK_H

#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <core/Dictionary.hpp>
#include <mutex>

namespace Callbacks {
    struct CallbackInfo {
        bool markerSignalEmitted = true;
        bool beatSignalEmitted = true;
        bool soundSignalEmitted = true;
        godot::Dictionary markerCallbackInfo;
        godot::Dictionary beatCallbackInfo;
        godot::Dictionary soundCallbackInfo;
    };

    extern std::mutex callback_mut;

    FMOD_RESULT F_CALLBACK eventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters);
}

#endif //GODOTFMOD_GODOT_FMOD_CALLBACK_H
