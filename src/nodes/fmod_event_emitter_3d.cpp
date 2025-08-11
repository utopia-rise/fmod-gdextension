#include <nodes/fmod_event_emitter_3d.h>

using namespace godot;

void FmodEventEmitter3D::set_space_attribute_impl(const Ref<FmodEvent>& p_event) const {
    p_event->set_3d_attributes_with_velocity(get_global_transform(), _velocity);
}

void FmodEventEmitter3D::_ready() {
    FmodEventEmitter<FmodEventEmitter3D, Node3D>::_ready();
}

void FmodEventEmitter3D::_process(double delta) {
    FmodEventEmitter<FmodEventEmitter3D, Node3D>::_process(delta);
}

void FmodEventEmitter3D::_notification(int p_what) {
    FmodEventEmitter<FmodEventEmitter3D, Node3D>::_notification(p_what);
}

void FmodEventEmitter3D::_exit_tree() {
    FmodEventEmitter<FmodEventEmitter3D, Node3D>::_exit_tree();
}

void FmodEventEmitter3D::set_velocity(const Vector3& velocity) {
    _velocity = velocity;
}

Vector3 FmodEventEmitter3D::get_velocity() const {
    return _velocity;
}

void FmodEventEmitter3D::_bind_methods() {
    FmodEventEmitter<FmodEventEmitter3D, Node3D>::_bind_methods();
    
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &FmodEventEmitter3D::set_velocity);
    ClassDB::bind_method(D_METHOD("get_velocity"), &FmodEventEmitter3D::get_velocity);
    
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_velocity", "get_velocity");
}

void FmodEventEmitter3D::free_impl() {
    queue_free();
}
