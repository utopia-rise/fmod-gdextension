#include "fmod_listener_2d.h"

using namespace godot;

void FmodListener2D::_bind_methods() {
    FmodListener<FmodListener2D, Node2D>::_bind_methods();
}

void FmodListener2D::_ready() {
    FmodListener<FmodListener2D, Node2D>::_ready();
}

void FmodListener2D::_exit_tree() {
    FmodListener<FmodListener2D, Node2D>::_exit_tree();
}
