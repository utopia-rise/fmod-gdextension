#include <fmod_studio.hpp>
#include "event_callbacks.h"
#include "../godot_fmod.h"

namespace Callbacks{
    std::mutex callback_mut;

    FMOD_RESULT F_CALLBACK eventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters) {
        auto *instance = (FMOD::Studio::EventInstance *) event;
        auto instanceId = (uint64_t) instance;
        godot::EventInfo *eventInfo;
        std::lock_guard<std::mutex> lk(callback_mut);
        instance->getUserData((void **) &eventInfo);
        if (eventInfo) {
            Callbacks::CallbackInfo *callbackInfo = &eventInfo->callbackInfo;

            if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
                auto *props = (FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *) parameters;
                callbackInfo->markerCallbackInfo["event_id"] = instanceId;
                callbackInfo->markerCallbackInfo["name"] = props->name;
                callbackInfo->markerCallbackInfo["position"] = props->position;
                callbackInfo->markerSignalEmitted = false;
            } else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
                auto *props = (FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES *) parameters;
                callbackInfo->beatCallbackInfo["event_id"] = instanceId;
                callbackInfo->beatCallbackInfo["beat"] = props->beat;
                callbackInfo->beatCallbackInfo["bar"] = props->bar;
                callbackInfo->beatCallbackInfo["tempo"] = props->tempo;
                callbackInfo->beatCallbackInfo["time_signature_upper"] = props->timesignatureupper;
                callbackInfo->beatCallbackInfo["time_signature_lower"] = props->timesignaturelower;
                callbackInfo->beatCallbackInfo["position"] = props->position;
                callbackInfo->beatSignalEmitted = false;
            } else if (type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED ||
                       type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED) {
                auto *sound = (FMOD::Sound *) parameters;
                char n[256];
                sound->getName(n, 256);
                godot::String name(n);
                godot::String mType = type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED ? "played" : "stopped";
                callbackInfo->soundCallbackInfo["name"] = name;
                callbackInfo->soundCallbackInfo["type"] = mType;
                callbackInfo->soundSignalEmitted = false;
            }
        }
        return FMOD_OK;
    }
}
