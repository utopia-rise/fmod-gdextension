#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_3D_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_3D_H

#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_event_emitter.h"

namespace godot {
    class FmodEventEmitter3D : public Node3D {
        GDCLASS(FmodEventEmitter3D, Node3D)

        FmodEventEmitter internal_emitter;

    public:
        FmodEventEmitter3D() = default;
        ~FmodEventEmitter3D() override = default;

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

#endif// GODOTFMOD_FMOD_EVENT_EMITTER_3D_H