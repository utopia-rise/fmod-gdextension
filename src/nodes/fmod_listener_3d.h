#ifndef GODOTFMOD_FMOD_LISTENER_3D_H
#define GODOTFMOD_FMOD_LISTENER_3D_H

#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_listener.h"

namespace godot {
    class FmodListener3D : public Node3D,
                           public FmodListener {
        GDCLASS(FmodListener3D, Node3D)

    public:

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_LISTENER_3D_H
