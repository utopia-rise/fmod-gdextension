#ifndef GODOTFMOD_MATHS_H
#define GODOTFMOD_MATHS_H

#include "fmod_common.h"
#include "variant/transform2d.hpp"
#include "variant/transform3d.hpp"
#include "variant/vector2.hpp"
#include "variant/vector3.hpp"

namespace godot {

    /*
     * Every conversion between Godot's positions and velocities and FMOD's, in one place. FMOD works in meters:
     * 3D units and 2D pixels are divided by their distance factor on the way in and multiplied on the way out.
     * A 2D position is placed on the XZ plane, mirrored on X.
     */
    class FmodSpatial {
    private:
        // FMOD rejects a zero orientation vector, which a zero scale produces.
        static Vector3 normalized_or(const Vector3& vec, const Vector3& fallback) {
            Vector3 normalized = vec.normalized();
            return normalized == Vector3() ? fallback : normalized;
        }

        static FMOD_VECTOR to_fmod_vector(const Vector3& vec) {
            FMOD_VECTOR fv;
            fv.x = vec.x;
            fv.y = vec.y;
            fv.z = vec.z;
            return fv;
        }

        static Vector3 to_vector3(const FMOD_VECTOR& vec) { return {vec.x, vec.y, vec.z}; }

    public:
        inline static float distance_factor_2d = 64;
        inline static float distance_factor_3d = 1;

        static FMOD_VECTOR vector_to_fmod_3d(const Vector3& vector) {
            return to_fmod_vector(vector / distance_factor_3d);
        }

        static FMOD_VECTOR vector_to_fmod_2d(const Vector2& vector) {
            return to_fmod_vector(Vector3(-vector.x, 0.0f, vector.y) / distance_factor_2d);
        }

        static Vector3 vector_to_godot_3d(const FMOD_VECTOR& vector) { return to_vector3(vector) * distance_factor_3d; }

        static Vector2 vector_to_godot_2d(const FMOD_VECTOR& vector) {
            return Vector2(-vector.x, vector.z) * distance_factor_2d;
        }

        static FMOD_3D_ATTRIBUTES attributes_to_fmod_3d(const Transform3D& transform) {
            FMOD_3D_ATTRIBUTES attributes;
            attributes.position = vector_to_fmod_3d(transform.get_origin());
            attributes.up = to_fmod_vector(normalized_or(transform.get_basis().get_column(1), Vector3(0, 1, 0)));
            attributes.forward = to_fmod_vector(-normalized_or(transform.get_basis().get_column(2), Vector3(0, 0, 1)));
            attributes.velocity = to_fmod_vector(Vector3());
            return attributes;
        }

        static FMOD_3D_ATTRIBUTES attributes_to_fmod_2d(const Transform2D& transform) {
            FMOD_3D_ATTRIBUTES attributes;
            attributes.position = vector_to_fmod_2d(transform.get_origin());
            attributes.up = to_fmod_vector(Vector3(0, 1, 0));
            attributes.forward = to_fmod_vector(
                normalized_or(Vector3(-transform.columns[1].x, 0, transform.columns[1].y), Vector3(0, 0, 1))
            );
            attributes.velocity = to_fmod_vector(Vector3());
            return attributes;
        }

        static Transform3D transform_to_godot_3d(const FMOD_3D_ATTRIBUTES& attributes) {
            Transform3D transform;
            transform.origin = vector_to_godot_3d(attributes.position);
            Vector3 up = to_vector3(attributes.up);
            Vector3 forward = to_vector3(attributes.forward);
            transform.basis.set_column(1, up);
            transform.basis.set_column(2, forward);
            transform.basis.set_column(0, up.cross(forward));
            return transform;
        }

        static Transform2D transform_to_godot_2d(const FMOD_3D_ATTRIBUTES& attributes) {
            Transform2D transform;
            transform.set_origin(vector_to_godot_2d(attributes.position));
            Vector2 forward(-attributes.forward.x, attributes.forward.z);
            transform.columns[1] = forward;
            transform.columns[0] = Vector2(forward.y, forward.x);
            return transform;
        }
    };

}// namespace godot

#endif// GODOTFMOD_MATHS_H
