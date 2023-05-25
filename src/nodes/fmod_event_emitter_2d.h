#ifndef FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
#define FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H

#include "fmod_event_emitter.h"
#include "studio/fmod_event.h"

#include <classes/node2d.hpp>

namespace godot {
    class FmodEventEmitter2D : public Node2D {
        GDCLASS(FmodEventEmitter2D, Node2D)

        FmodEventEmitter internal_emitter;

    public:
        FmodEventEmitter2D() = default;
        ~FmodEventEmitter2D() override = default;

        virtual void _ready() override;
        virtual  void _process(double delta) override;
        void _notification(int p_what);
        virtual void _exit_tree() override;

        void set_param(const String& key, const float value);
        bool is_paused();
        void play();
        void pause();
        void set_event_name(const String& name);
        String get_event_name();
        void set_attached(const bool attached);
        bool is_attached() const;
        void set_autoplay(const bool autoplay);
        bool is_autoplay() const;
        void set_looped(const bool looped);
        bool is_looped() const;
        void set_allow_fadeout(const bool allow_fadeout);
        bool is_allow_fadeout() const;
        void set_preload_event(const bool preload_event);
        bool is_preload_event() const;

        static void _bind_methods();
    };
}// namespace godot

#endif// FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
