#ifndef GODOTFMOD_FMOD_LISTENER_2D_H
#define GODOTFMOD_FMOD_LISTENER_2D_H

#include "classes/node.hpp"
#include "classes/node2d.hpp"
#include "fmod_listener.h"

namespace godot {
    class FmodListener2D : public FmodListener<FmodListener2D, Node2D> {
        GDCLASS(FmodListener2D, Node2D)

    public:
        virtual void _ready() override;
        virtual void _exit_tree() override;

    protected:

        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_LISTENER_2D_H
