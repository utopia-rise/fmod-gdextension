#ifndef GODOTFMOD_FMOD_LISTENER_2D_H
#define GODOTFMOD_FMOD_LISTENER_2D_H

#include "classes/node.hpp"
#include "classes/node2d.hpp"
#include "fmod_listener.h"

namespace godot {
    class FmodListener2D : public Node2D,
                           public FmodListener {
        GDCLASS(FmodListener2D, Node2D)

    public:

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_LISTENER_2D_H
