#ifndef GODOTFMOD_FMOD_SOUND_H
#define GODOTFMOD_FMOD_SOUND_H

#include "classes/ref.hpp"
#include "classes/ref_counted.hpp"
#include "core/fmod_file.h"
#include "fmod.hpp"

namespace godot {
    class FmodSound : public RefCounted {
        GDCLASS(FmodSound, RefCounted);

        FMOD::Channel* _wrapped = nullptr;
        Ref<FmodFile> file;

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

        bool is_valid() const;
        void play() const;
        void release() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SOUND_H
