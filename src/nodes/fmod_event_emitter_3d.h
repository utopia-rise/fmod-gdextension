#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_3D_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_3D_H

#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_event_emitter.h"

namespace godot {
    class FmodEventEmitter3D : public Node3D, public FmodEventEmitter {
        GDCLASS(FmodEventEmitter3D, Node3D);

    public:
        FmodEventEmitter3D();
        ~FmodEventEmitter3D();

        void _init();
        void _ready() override;
        void _exit_tree() override;
        void _notification(int p_what);

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_EVENT_EMITTER_3D_H