#ifndef GODOTFMOD_FMOD_BANK_H
#define GODOTFMOD_FMOD_BANK_H

#include "classes/ref_counted.hpp"
#include "fmod_bus.h"
#include "fmod_event_description.h"
#include "fmod_object.h"
#include "fmod_studio.hpp"
#include "fmod_vca.h"
#include "helpers/common.h"

namespace godot {
    class FmodBank : public FmodObjectWithPath<FmodBank, FMOD::Studio::Bank> {
        List<Ref<FmodEventDescription>> eventDescriptions;
        List<Ref<FmodBus>> buses;
        List<Ref<FmodVCA>> VCAs;

        void load_all_vca();
        void load_all_buses();
        void load_all_event_descriptions();

    public:
        FmodBank() = default;
        ~FmodBank() override = default;

        int get_loading_state();

        int get_event_description_count();
        int get_bus_count();
        int get_vca_count() const;
        int get_string_count() const;

        Array get_description_list() const;
        Array get_bus_list() const;
        Array get_vca_list() const;

        void update_bank_data();

        const List<Ref<FmodEventDescription>>& getEventDescriptions() const;
        const List<Ref<FmodBus>>& getBuses() const;
        const List<Ref<FmodVCA>>& getVcAs() const;

        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BANK_H
