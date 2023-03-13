#ifndef GODOTFMOD_FMOD_LISTENER_2D_H
#define GODOTFMOD_FMOD_LISTENER_2D_H

#include "classes/node.hpp"
#include "fmod_listener.h"

namespace godot {
    class FmodListener2D : public Node, public FmodListener {
        GDCLASS(FmodListener2D, Node)

    public:
    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_LISTENER_2D_H
