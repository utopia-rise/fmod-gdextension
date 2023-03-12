#ifndef GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
#define GODOTFMOD_FMOD_EVENT_DESCRIPTION_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodEventDescription : public RefCounted {
        GDCLASS(FmodEventDescription, RefCounted);

        FMOD::Studio::EventDescription* description;

        int desc_get_length(const String& eventPath);
        Array desc_get_instance_list(const String& eventPath);
        int desc_get_instance_count(const String& eventPath);
        void desc_release_all_instances(const String& eventPath);
        void desc_load_sample_data(const String& eventPath);
        void desc_unload_sample_data(const String& eventPath);
        int desc_get_sample_loading_state(const String& eventPath);
        bool desc_is_3d(const String& eventPath);
        bool desc_is_one_shot(const String& eventPath);
        bool desc_is_snapshot(const String& eventPath);
        bool desc_is_stream(const String& eventPath);
        bool desc_has_sustain_point(const String& eventPath);
        Array desc_get_min_max_distance(const String& eventPath);
        float desc_get_sound_size(const String& eventPath);
        Dictionary desc_get_parameter_description_by_name(const String& eventPath, const String& name);
        Dictionary desc_get_parameter_description_by_id(const String& eventPath, const Array& idPair);
        int desc_get_parameter_description_count(const String& eventPath);
        Dictionary desc_get_parameter_description_by_index(const String& eventPath, int index);
        Dictionary desc_get_user_property(const String& eventPath, const String& name);
        int desc_get_user_property_count(const String& eventPath);
        Dictionary desc_user_property_by_index(const String& eventPath, int index);

    public:
        FmodEventDescription() = default;
        ~FmodEventDescription() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_DESCRIPTION_H
