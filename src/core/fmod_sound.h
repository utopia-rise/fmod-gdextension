#ifndef GODOTFMOD_FMOD_SOUND_H
#define GODOTFMOD_FMOD_SOUND_H

#include "classes/ref_counted.hpp"
#include "fmod.hpp"

namespace godot {
    class FmodSound : public RefCounted {
        GDCLASS(FmodSound, RefCounted);

        FMOD::Sound* sound = nullptr;

    public:
        void set_sound_paused(bool paused);
        void stop_sound();
        bool is_sound_playing();
        void set_sound_volume(float volume);
        float get_sound_volume();
        float get_sound_pitch();
        void set_sound_pitch(float pitch);

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SOUND_H
