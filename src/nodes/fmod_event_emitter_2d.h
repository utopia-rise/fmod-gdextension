#ifndef FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
#define FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H

#include "fmod_event_emitter.h"
#include "studio/fmod_event.h"

#include <classes/node2d.hpp>

namespace godot {
    class FmodEventEmitter2D : public FmodEventEmitter<FmodEventEmitter2D, Node2D>  {
        friend class FmodEventEmitter<FmodEventEmitter2D, Node2D>;
        GDCLASS(FmodEventEmitter2D, Node2D)

    private:
        void set_space_attribute_impl() const;
        void free_impl();

    public:
        FmodEventEmitter2D() = default;
        ~FmodEventEmitter2D() override = default;

        virtual void _ready() override;
        virtual  void _process(double delta) override;
        void _notification(int p_what);
        virtual void _exit_tree() override;

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
