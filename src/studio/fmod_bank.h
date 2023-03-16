#ifndef GODOTFMOD_FMOD_BANK_H
#define GODOTFMOD_FMOD_BANK_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "helpers/fmod_object.h"

namespace godot {
    class FmodBank : public RefCounted, public FmodObject<FmodBank, FMOD::Studio::Bank>  {
        GDCLASS(FmodBank, RefCounted);

    public:
        FmodBank() = default;
        ~FmodBank() = default;

        int get_loading_state();
        int get_event_description_count();
        int get_bus_count();
        int get_vca_count() const;
        int get_string_count() const;

        Array get_description_list() const;
        Array get_bus_list() const;
        Array get_vca_list() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BANK_H
