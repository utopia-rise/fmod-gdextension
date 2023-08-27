#include "fmod_listener_3d.h"

#include "fmod_server.h"

using namespace godot;

void FmodListener3D::_bind_methods() {
    FmodListener<FmodListener3D, Node3D>::_bind_methods();
}

void FmodListener3D::_ready() {
    FmodListener<FmodListener3D, Node3D>::_ready();
}

void FmodListener3D::_exit_tree() {
    FmodListener<FmodListener3D, Node3D>::_exit_tree();
}
