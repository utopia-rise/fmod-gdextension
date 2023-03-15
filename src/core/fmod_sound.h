#ifndef GODOTFMOD_FMOD_SOUND_H
#define GODOTFMOD_FMOD_SOUND_H

#include "classes/ref_counted.hpp"
#include "fmod.hpp"

namespace godot {
    class FmodSound : public RefCounted {
        GDCLASS(FmodSound, RefCounted);

        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;

    public:
        void set_paused(bool paused) const;
        void stop() const;
        bool is_playing() const;
        void set_volume(float volume) const;
        float get_volume() const;
        float get_pitch() const;
        void set_pitch(float pitch);

        bool is_valid() const;
        void play();
        void release() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SOUND_H
