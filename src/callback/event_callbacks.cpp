#include "fmod_server.h"
#include "fmod_studio.hpp"

#include <callback/event_callbacks.h>
#include <studio/fmod_event.h>

#include <variant/callable.hpp>
#include <variant/dictionary.hpp>

namespace Callbacks {

    FMOD_RESULT F_CALL event_callback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters) {
        // A programmer sound is released even once its FmodEvent is gone: FMOD blocks its update thread until it is.
        if (type == FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND) {
            auto* props {reinterpret_cast<FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*>(parameters)};
            auto* sound {(FMOD::Sound*) props->sound};
            if (sound) { ERROR_CHECK(sound->release()); }
            return FMOD_OK;
        }

        auto* instance = reinterpret_cast<FMOD::Studio::EventInstance*>(event);
        godot::FmodEvent* event_instance = nullptr;
        instance->getUserData((void**) &event_instance);
        if (event_instance) {
            if (type == FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND) {
                const godot::String& sound_key {event_instance->get_programmers_callback_sound_key()};
                FMOD_STUDIO_SOUND_INFO sound_info {godot::FmodServer::get_singleton()->get_sound_info(sound_key)};
#if defined(WEB_ENABLED) && !defined(THREADS_ENABLED)
                // FMOD's single-threaded web build has no thread to load a sound without blocking.
                FMOD_MODE loading = FMOD_DEFAULT;
#else
                FMOD_MODE loading = FMOD_NONBLOCKING;
#endif
                FMOD::Sound* sound {
                  godot::FmodServer::get_singleton()->create_sound(sound_info, FMOD_LOOP_NORMAL | FMOD_CREATECOMPRESSEDSAMPLE | loading)
                };

                auto* props {reinterpret_cast<FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*>(parameters)};

                props->sound = (FMOD_SOUND*) sound;
                props->subsoundIndex = sound_info.subsoundindex;

                return FMOD_OK;
            }
            godot::Dictionary dictionary;
            if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
                auto* props {reinterpret_cast<FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*>(parameters)};
                dictionary["name"] = props->name;
                dictionary["position"] = props->position;
            } else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
                auto* props {reinterpret_cast<FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*>(parameters)};
                dictionary["beat"] = props->beat;
                dictionary["bar"] = props->bar;
                dictionary["tempo"] = props->tempo;
                dictionary["time_signature_upper"] = props->timesignatureupper;
                dictionary["time_signature_lower"] = props->timesignaturelower;
                dictionary["position"] = props->position;
            }
            const godot::Callable& callback {event_instance->get_callback()};
            if (!callback.is_null() && callback.is_valid()) {
                godot::FmodServer::get_singleton()->add_callback({type, callback, dictionary});
            }
        }

        return FMOD_OK;
    }
}// namespace Callbacks
