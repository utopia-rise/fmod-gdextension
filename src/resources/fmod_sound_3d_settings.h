#ifndef GODOTFMOD_FMOD_SOUND_3D_SETTINGS_H
#define GODOTFMOD_FMOD_SOUND_3D_SETTINGS_H

#include <classes/resource.hpp>

namespace godot {
    class FmodSound3DSettings : public Resource {
        GDCLASS(FmodSound3DSettings, Resource)

    public:
        void set_doppler_scale(const float p_doppler_scale);
        float get_doppler_scale() const;

        void set_distance_factor(const float p_distance_factor);
        float get_distance_factor() const;

        void set_rolloff_scale(const float p_rolloff_scale);
        float get_rolloff_scale() const;

        static Ref<FmodSound3DSettings> get_from_project_settings();

        FmodSound3DSettings() = default;
        ~FmodSound3DSettings() = default;

        static constexpr const char* THREE_D_SETTINGS_BASE_PATH = "3D Settings";

        static constexpr const char* DOPPLER_SCALE_OPTION = "doppler_scale";
        static constexpr const char* DISTANCE_FACTOR_OPTION = "distance_factor";
        static constexpr const char* ROLLOFF_SCALE_OPTION = "rolloff_scale";

        static constexpr const float DEFAULT_DOPPLER_SCALE = 1;
        static constexpr const float DEFAULT_DISTANCE_FACTOR = 1;
        static constexpr const float DEFAULT_ROLLOFF_SCALE = 1;

    private:
        float _doppler_scale;
        float _distance_factor;
        float _rolloff_scale;

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_SOUND_3D_SETTINGS_H
