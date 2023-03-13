#ifndef GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
#define GODOTFMOD_FMOD_EVENT_DESCRIPTION_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodEventDescription : public RefCounted {
        GDCLASS(FmodEventDescription, RefCounted);

        FMOD::Studio::EventDescription* description = nullptr;

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
        Dictionary get_parameter_by_name(const String& name);
        Dictionary get_parameter_by_id(const Array& idPair);
        int get_parameter_count();
        Dictionary get_parameter_by_index(int index);
        Dictionary get_user_property(const String& name);
        int get_user_property_count();
        Dictionary user_property_by_index(int index);

    public:
        FmodEventDescription() = default;
        ~FmodEventDescription() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
