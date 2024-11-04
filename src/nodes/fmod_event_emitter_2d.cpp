#include <nodes/fmod_event_emitter_2d.h>

using namespace godot;

void FmodEventEmitter2D::set_space_attribute_impl(const Ref<FmodEvent>& p_event) const {
    p_event->set_2d_attributes(get_global_transform());
}

void FmodEventEmitter2D::_ready() {
    FmodEventEmitter<FmodEventEmitter2D, Node2D>::_ready();
}

void FmodEventEmitter2D::_process(double delta) {
    FmodEventEmitter<FmodEventEmitter2D, Node2D>::_process(delta);
}

void FmodEventEmitter2D::_notification(int p_what) {
    FmodEventEmitter<FmodEventEmitter2D, Node2D>::_notification(p_what);
}

void FmodEventEmitter2D::_exit_tree() {
    FmodEventEmitter<FmodEventEmitter2D, Node2D>::_exit_tree();
}

void FmodEventEmitter2D::_bind_methods() {
    FmodEventEmitter<FmodEventEmitter2D, Node2D>::_bind_methods();
}

void FmodEventEmitter2D::free_impl() {
    queue_free();
}