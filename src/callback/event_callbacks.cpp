#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "studio/fmod_event.h"

#include <callback/event_callbacks.h>

#include <variant/callable.hpp>
#include <variant/dictionary.hpp>
#include <variant/variant.hpp>

namespace Callbacks {

    FMOD_RESULT F_CALLBACK eventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters) {
        auto* instance = reinterpret_cast<FMOD::Studio::EventInstance*>(event);
        godot::FmodEvent* event_instance;
        instance->getUserData((void**) &event_instance);
        if (event_instance) {
            godot::Dictionary dictionary;
            if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
                auto* props = (FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*) parameters;
                dictionary["name"] = props->name;
                dictionary["position"] = props->position;
            } else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
                auto* props = (FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*) parameters;
                dictionary["beat"] = props->beat;
                dictionary["bar"] = props->bar;
                dictionary["tempo"] = props->tempo;
                dictionary["time_signature_upper"] = props->timesignatureupper;
                dictionary["time_signature_lower"] = props->timesignaturelower;
                dictionary["position"] = props->position;
            } else if (type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED || type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED) {
                auto* sound = (FMOD::Sound*) parameters;
                char n[256];
                sound->getName(n, 256);
                godot::String name(n);
                dictionary["name"] = name;
            }
            godot::Callable callback = event_instance->get_callback();
            if (!callback.is_null() && callback.is_valid()) {
                godot::Array args = godot::Array();
                args.append(dictionary);
                args.append(type);
                callback.callv(args);
            }
        }

        return FMOD_OK;
    }
}// namespace Callbacks
