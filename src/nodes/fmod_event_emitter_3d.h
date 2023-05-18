#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_3D_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_3D_H

#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_event_emitter.h"

namespace godot {
    class FmodEventEmitter3D : public FmodEventEmitter<Node3D> {
        GDCLASS(FmodEventEmitter3D, Node3D)

    public:
        FmodEventEmitter3D() = default;
        ~FmodEventEmitter3D() override = default;

        void _ready() override;
        void _process(double delta) override;
        void _notification(int p_what);
        void _exit_tree() override;

        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_EVENT_EMITTER_3D_H