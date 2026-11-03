#ifndef GODOTFMOD_FMOD_BUS_H
#define GODOTFMOD_FMOD_BUS_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "helpers/constants.h"

namespace godot {
    class FmodBus : public RefCounted {
        FMODCLASSWITHPATH(FmodBus, RefCounted, FMOD::Studio::Bus);

    public:
        FmodBus() = default;
        ~FmodBus() override = default;

        bool get_mute() const;
        bool get_paused() const;
        float get_volume() const;
        void set_mute(bool mute) const;
        void set_paused(bool paused) const;
        void set_volume(float volume) const;
        void stop_all_events(FMOD_STUDIO_STOP_MODE stop_mode);

        protected:
            static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BUS_H
