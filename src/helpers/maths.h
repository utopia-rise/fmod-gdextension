#ifndef GODOTFMOD_MATHS_H
#define GODOTFMOD_MATHS_H

#include "fmod_common.h"
#include "variant/dictionary.hpp"
#include "variant/transform2d.hpp"
#include "variant/transform3d.hpp"
#include "variant/vector2.hpp"
#include "variant/vector3.hpp"

namespace godot {

    static inline FMOD_VECTOR get_fmod_vector_from_3d(const Vector3& vec) {
        FMOD_VECTOR fv;
        fv.x = vec.x;
        fv.y = vec.y;
        fv.z = vec.z;
        return fv;
    }

    static inline FMOD_3D_ATTRIBUTES get_3d_attributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward, const FMOD_VECTOR& vel) {
        FMOD_3D_ATTRIBUTES f3d;
        f3d.forward = forward;
        f3d.position = pos;
        f3d.up = up;
        f3d.velocity = vel;
        return f3d;
    }

    static inline FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform3d(const Transform3D& transform, const Vector3& velocity, const float distanceScale) {
        Vector3 pos = transform.get_origin() / distanceScale;
        Vector3 up = transform.get_basis().rows[1];
        Vector3 forward = transform.get_basis().rows[2];
        return get_3d_attributes(get_fmod_vector_from_3d(pos), get_fmod_vector_from_3d(up), get_fmod_vector_from_3d(forward), get_fmod_vector_from_3d(velocity));
    }

    static inline FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform3d(const Transform3D& transform, const float distanceScale) {
        Vector3 pos = transform.get_origin() / distanceScale;
        Vector3 up = transform.get_basis().rows[1];
        Vector3 forward = transform.get_basis().rows[2];
        Vector3 vel(0, 0, 0);
        return get_3d_attributes(get_fmod_vector_from_3d(pos), get_fmod_vector_from_3d(up), get_fmod_vector_from_3d(forward), get_fmod_vector_from_3d(vel));
    }

    static inline FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform2d(const Transform2D& transform, const Vector2& velocity, const float distanceScale) {
        Vector2 posVector = transform.get_origin() / distanceScale;
        Vector3 pos(-posVector.x, 0.0f, posVector.y);
        Vector3 up(0, 1, 0);
        Vector3 forward = Vector3(-transform.columns[1].x, 0, transform.columns[1].y).normalized();
        Vector3 vel(-velocity.x, 0, velocity.y);
        const FMOD_VECTOR& posFmodVector = get_fmod_vector_from_3d(pos);
        return get_3d_attributes(posFmodVector, get_fmod_vector_from_3d(up), get_fmod_vector_from_3d(forward), get_fmod_vector_from_3d(vel));
    }

    static inline FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform2d(const Transform2D& transform, const float distanceScale) {
        Vector2 posVector = transform.get_origin() / distanceScale;
        Vector3 pos(-posVector.x, 0.0f, posVector.y);
        Vector3 up(0, 1, 0);
        Vector3 forward = Vector3(-transform.columns[1].x, 0, transform.columns[1].y).normalized();
        Vector3 vel(0, 0, 0);
        const FMOD_VECTOR& posFmodVector = get_fmod_vector_from_3d(pos);
        return get_3d_attributes(posFmodVector, get_fmod_vector_from_3d(up), get_fmod_vector_from_3d(forward), get_fmod_vector_from_3d(vel));
    }

    static inline Transform3D get_transform3d_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr, const float distanceScale) {
        Transform3D transform;
        transform.origin = Vector3(attr.position.x, attr.position.y, attr.position.z) * distanceScale;
        const Vector3& upVector = Vector3(attr.up.x, attr.up.y, attr.up.z);
        transform.basis.rows[1] = upVector;
        const Vector3& forwardVector = Vector3(attr.forward.x, attr.forward.y, attr.forward.z);
        transform.basis.rows[2] = forwardVector;
        transform.basis.rows[0] = upVector.cross(forwardVector);
        return transform;
    }

    static inline Transform2D get_transform2d_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr, const float distanceScale) {
        Transform2D transform;
        transform.set_origin(Vector2(-attr.position.x, attr.position.z) * distanceScale);
        const Vector2& forward = Vector2(-attr.forward.x, attr.forward.z);
        transform.columns[1] = forward;
        transform.columns[0] = Vector2(forward.y, forward.x);
        return transform;
    }

    static inline Vector3 get_velocity3d_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr, const float distanceScale) {
        return {attr.velocity.x, attr.velocity.y, attr.velocity.z};
    }

    static inline Vector2 get_velocity2d_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr, const float distanceScale) {
        return {-attr.velocity.x, attr.velocity.z};
    }

}// namespace godot

#endif// GODOTFMOD_MATHS_H
