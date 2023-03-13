#ifndef GODOTFMOD_FMOD_BANK_H
#define GODOTFMOD_FMOD_BANK_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodBank : public RefCounted {
        GDCLASS(FmodBank, RefCounted);

        FMOD::Studio::Bank* bank = nullptr;

    public:
        FmodBank() = default;
        ~FmodBank() = default;

        int get_loading_state();
        int get_bus_count();
        int get_event_count();
        int get_string_count();
        int get_vca_count();

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BANK_H
