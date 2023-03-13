#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_H

#include "studio/fmod_event.h"

namespace godot {

    class FmodEventEmitter {
    protected:
        Ref<FmodEvent> event;

        String event_name;
        bool attached = true;
        bool autoplay = false;
        bool looped = false;
        bool allow_fadeout = true;
        bool preload_event = true;

        Dictionary params;

        void _unpause();
        void _play_one_shot();
        void _play_looped();
        void _set_param_internally(const String& key, const float value);

    public:
        void set_param(const String& key, const float value);
        bool is_paused();
        void play();
        void pause();

        void set_event_name(const String& name);
        String get_event_name();
        void set_attached(const bool attached);
        bool is_attached() const;
        void set_autoplay(const bool autoplay);
        bool is_autoplay();
        void set_looped(const bool looped);
        bool is_looped();
        void set_allow_fadeout(const bool allow_fadeout);
        bool is_allow_fadeout();
        void set_preload_event(const bool preload_event);
        bool is_preload_event();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_EMITTER_H
