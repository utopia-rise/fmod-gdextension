#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_3D_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_3D_H

#include "classes/node.hpp"
#include "classes/node3d.hpp"
#include "fmod_event_emitter.h"

namespace godot {
    class FmodEventEmitter3D : public FmodEventEmitter<FmodEventEmitter3D, Node3D> {
        friend class FmodEventEmitter<FmodEventEmitter3D, Node3D>;
        GDCLASS(FmodEventEmitter3D, Node3D)

    private:
        Vector3 _velocity;
        void set_space_attribute_impl(const Ref<FmodEvent>& p_event) const;
        void free_impl();

    public:
        FmodEventEmitter3D() : _velocity(0, 0, 0) {}
        ~FmodEventEmitter3D() override = default;

        virtual void _ready() override;
        virtual  void _process(double delta) override;
        void _notification(int p_what);
        virtual void _exit_tree() override;

        void set_velocity(const Vector3& velocity);
        Vector3 get_velocity() const;

        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_EVENT_EMITTER_3D_H