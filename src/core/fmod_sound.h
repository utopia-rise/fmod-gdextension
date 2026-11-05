#ifndef GODOTFMOD_FMOD_SOUND_H
#define GODOTFMOD_FMOD_SOUND_H

#include "classes/ref.hpp"
#include "classes/ref_counted.hpp"
#include "core/fmod_file.h"
#include "fmod.hpp"
#include "variant/vector2.hpp"
#include "variant/vector3.hpp"

namespace godot {
    class FmodSound : public RefCounted {
        GDCLASS(FmodSound, RefCounted);

        FMOD::Channel* _wrapped = nullptr;
        Ref<FmodFile> file;

        FMOD_3D_ATTRIBUTES get_3d_attributes() const;
        void set_3d_attributes(FMOD_3D_ATTRIBUTES& attributes) const;

    public:
        inline static Ref<FmodSound> create_ref(FMOD::Channel* wrapped, const Ref<FmodFile>& file) {
            Ref<FmodSound> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
                ref->file = file;
                wrapped->setUserData(ref.ptr());
            }
            return ref;
        }

        void set_paused(bool paused) const;
        void stop() const;
        bool is_playing() const;
        void set_volume(float volume) const;
        float get_volume() const;
        float get_pitch() const;
        void set_pitch(float pitch);

        void set_position(const Vector3& position) const;
        Vector3 get_position() const;
        void set_velocity(const Vector3& velocity) const;
        Vector3 get_velocity() const;
        void set_position_2d(const Vector2& position) const;
        Vector2 get_position_2d() const;
        void set_velocity_2d(const Vector2& velocity) const;
        Vector2 get_velocity_2d() const;

        bool is_valid() const;
        void play() const;
        void release() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SOUND_H
