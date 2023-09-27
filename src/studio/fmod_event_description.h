#ifndef GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
#define GODOTFMOD_FMOD_EVENT_DESCRIPTION_H

#include "classes/ref_counted.hpp"
#include "fmod_parameter_description.h"
#include "fmod_studio.hpp"
#include "helpers/common.h"

namespace godot {
    class FmodEventDescription  : public RefCounted {
        FMODCLASSWITHPATH(FmodEventDescription, RefCounted, FMOD::Studio::EventDescription);

    public:
        FmodEventDescription() = default;
        ~FmodEventDescription() override = default;

        int get_length();
        Array get_instance_list();
        int get_instance_count();
        void release_all_instances();
        void load_sample_data();
        void unload_sample_data();
        int get_sample_loading_state();
        bool is_3d();
        bool is_one_shot();
        bool is_snapshot();
        bool is_stream();
        bool has_sustain_point();
        Array get_min_max_distance();
        float get_sound_size();
        Ref<FmodParameterDescription> get_parameter_by_name(const String& name);
        Ref<FmodParameterDescription> get_parameter_by_id(uint64_t id);
        int get_parameter_count();
        Ref<FmodParameterDescription> get_parameter_by_index(int index);
        Array get_parameters();
        Dictionary get_user_property(const String& name);
        int get_user_property_count();
        Dictionary user_property_by_index(int index);

        protected:
            static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
