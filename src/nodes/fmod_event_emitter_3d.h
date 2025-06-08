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
        void set_space_attribute_impl(const Ref<FmodEvent>& p_event) const;
        void reset_space_attribute_impl(const Ref<FmodEvent>& p_event) const;
        void free_impl();

    public:
        FmodEventEmitter3D() = default;
        ~FmodEventEmitter3D() override = default;

        virtual void _ready() override;
        virtual  void _process(double delta) override;
        void _notification(int p_what);
        virtual void _exit_tree() override;

        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_EVENT_EMITTER_3D_H