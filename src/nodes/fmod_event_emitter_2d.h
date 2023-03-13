#ifndef FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
#define FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H

#include "fmod_event_emitter.h"
#include "studio/fmod_event.h"
#include <classes/node2d.hpp>

namespace godot {

    class FmodEventEmitter2D : public Node2D, public FmodEventEmitter {
        GDCLASS(FmodEventEmitter2D, Node2D);

    public:
        FmodEventEmitter2D();
        ~FmodEventEmitter2D();

        void _init();
        void _ready() override;
        void _exit_tree() override;
        void _notification(int p_what);

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
