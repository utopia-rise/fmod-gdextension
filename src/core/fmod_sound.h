#ifndef GODOTFMOD_FMOD_SOUND_H
#define GODOTFMOD_FMOD_SOUND_H

#include "classes/ref_counted.hpp"
#include "fmod.hpp"
#include "helpers/fmod_object.h"

namespace godot {
    class FmodSound : public RefCounted {
        GDCLASS(FmodSound, RefCounted);

        FMOD::Channel* _wrapped = nullptr;

    public:
        inline static Ref<FmodSound> create_ref(FMOD::Channel* wrapped) {
            Ref<FmodSound> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
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

        bool is_valid() const;
        void play() const;
        void release() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SOUND_H
