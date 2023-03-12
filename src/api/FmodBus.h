#ifndef GODOTFMOD_FMODBUS_H
#define GODOTFMOD_FMODBUS_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodBus : public RefCounted {
        GDCLASS(FmodBus, RefCounted);

        FMOD::Studio::Bus* bus;

        bool get_bus_mute(const String& busPath);
        bool get_bus_paused(const String& busPath);
        float get_bus_volume(const String& busPath);
        void set_bus_mute(const String& busPath, bool mute);
        void set_bus_paused(const String& busPath, bool paused);
        void set_bus_volume(const String& busPath, float volume);
        void stop_all_bus_events(const String& busPath, int stopMode);

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMODBUS_H
