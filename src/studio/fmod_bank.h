#ifndef GODOTFMOD_FMOD_BANK_H
#define GODOTFMOD_FMOD_BANK_H

#include "classes/ref_counted.hpp"
#include "fmod_bus.h"
#include "fmod_event_description.h"
#include "fmod_studio.hpp"
#include "fmod_vca.h"
#include "helpers/common.h"

namespace godot {
    class FmodBank : public RefCounted {
        FMODCLASSWITHPATH(FmodBank, RefCounted, FMOD::Studio::Bank);

        List<Ref<FmodEventDescription>> _event_descriptions;
        List<Ref<FmodBus>> _buses;
        List<Ref<FmodVCA>> _vcas;
        String _godot_res_path;

        void load_all_vca();
        void load_all_buses();
        void load_all_event_descriptions();

    public:
        FmodBank() = default;
        ~FmodBank() override;

        int get_loading_state();

        int64_t get_event_description_count();
        int64_t get_bus_count();
        int64_t get_vca_count() const;
        int get_string_count() const;

        Array get_description_list() const;
        Array get_bus_list() const;
        Array get_vca_list() const;

        void update_bank_data();

        const List<Ref<FmodEventDescription>>& get_event_descriptions() const;
        const List<Ref<FmodBus>>& get_buses() const;
        const List<Ref<FmodVCA>>& get_vcas() const;

        const String& get_godot_res_path() const;

        inline static Ref<FmodBank> create_ref(FMOD::Studio::Bank* wrapped, const String& p_godot_res_path) {
            Ref<FmodBank> ref { create_ref(wrapped) };
            ref->_godot_res_path = p_godot_res_path;
            return ref;
        }

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BANK_H
