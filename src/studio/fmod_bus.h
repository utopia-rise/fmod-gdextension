#ifndef GODOTFMOD_FMOD_BUS_H
#define GODOTFMOD_FMOD_BUS_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {T
    class FmodBus : public RefCounted {
        GDCLASS(FmodBus, RefCounted);

        FMOD::Studio::Bus* bus = nullptr;

        bool get_mute() const;
        bool get_paused() const;
        float get_volume() const;
        void set_mute(bool mute) const;
        void set_paused(bool paused) const;
        void set_volume(float volume) const;
        void stop_all_events(int stopMode);

    public:
        FmodBus() = default;
        ~FmodBus() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BUS_H
