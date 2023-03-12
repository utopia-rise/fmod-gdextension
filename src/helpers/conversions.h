#ifndef GODOTFMOD_CONVERSIONS_H
#define GODOTFMOD_CONVERSIONS_H

#include "classes/canvas_item.hpp"
#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_common.h"
#include "variant/utility_functions.hpp"
#include "variant/vector3.hpp"

namespace godot {

    FMOD_VECTOR to_fmod_vector(Vector3& vec) {
        FMOD_VECTOR fv;
        fv.x = vec.x;
        fv.y = vec.y;
        fv.z = vec.z;
        return fv;
    }

    FMOD_3D_ATTRIBUTES get_3d_attributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward, const FMOD_VECTOR& vel) {
        FMOD_3D_ATTRIBUTES
        f3d;
        f3d.forward = forward;
        f3d.position = pos;
        f3d.up = up;
        f3d.velocity = vel;
        return f3d;
    }

    FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform(const Transform3D& transform) {
        Vector3 pos = transform.get_origin() / distanceScale;
        Vector3 up = transform.get_basis().rows[1];
        Vector3 forward = transform.get_basis().rows[2];
        Vector3 vel(0, 0, 0);
        return get_3d_attributes(to_fmod_vector(pos), to_fmod_vector(up), to_fmod_vector(forward), to_fmod_vector(vel));
    }

    FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform_2d(const Transform2D& transform) {
        Vector2 posVector = transform.get_origin() / distanceScale;
        Vector3 pos(posVector.x, 0.0f, posVector.y);
        Vector3 up(0, 1, 0);
        Vector3 forward = Vector3(transform.columns[1].x, 0, transform.columns[1].y).normalized();
        Vector3 vel(0, 0, 0);// TODO: add doppler
        const FMOD_VECTOR& posFmodVector = to_fmod_vector(pos);
        return get_3d_attributes(posFmodVector, to_fmod_vector(up), to_fmod_vector(forward), to_fmod_vector(vel));
    }

    Dictionary get_transform_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attr) {
        Dictionary _3Dattr;
        Transform3D transform;
        transform.origin = Vector3(attr.position.x, attr.position.y, attr.position.z) * distanceScale;
        const Vector3& upVector = Vector3(attr.up.x, attr.up.y, attr.up.z);
        transform.basis.rows[1] = upVector;
        const Vector3& forwardVector = Vector3(attr.forward.x, attr.forward.y, attr.forward.z);
        transform.basis.rows[2] = forwardVector;
        transform.basis.rows[0] = upVector.cross(forwardVector);
        Vector3 velocity(attr.velocity.x, attr.velocity.y, attr.velocity.z);
        _3Dattr["transform"] = transform;
        _3Dattr["velocity"] = velocity;
        return _3Dattr;
    }

    Dictionary get_transform_2d_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attr) {
        Dictionary _2Dattr;
        Transform2D transform;
        transform.set_origin(Vector2(attr.position.x, attr.position.z) * distanceScale);
        const Vector2& forward = Vector2(attr.forward.x, attr.forward.z);
        transform.columns[1] = forward;
        transform.columns[0] = Vector2(forward.y, -forward.x);
        Vector2 velocity(attr.velocity.x, attr.velocity.z);
        _2Dattr["transform"] = transform;
        _2Dattr["velocity"] = velocity;
        return _2Dattr;
    }

    bool is_dead(Object* node) {
        if (!node) {
            return true;
        }
        return !UtilityFunctions::is_instance_valid(Object::cast_to<Node>(node)->get_owner());
    }

    bool is_fmod_valid(Object* node) {
        if (node) {
            bool ret = Node::cast_to<Node3D>(node) || Node::cast_to<CanvasItem>(node);
            if (!ret) {
                GODOT_LOG(2, "Invalid Object. A listener has to be either a Node3D or CanvasItem.")
            }
            return ret;
        }
        GODOT_LOG(2, "Object is null")
        return false;
    }
}// namespace godot

#endif// GODOTFMOD_CONVERSIONS_H
