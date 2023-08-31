#include "fmod_sound_3d_settings.h"

#include <constants.h>

#include <classes/project_settings.hpp>

using namespace godot;

void FmodSound3DSettings::set_doppler_scale(const float p_doppler_scale) {
    _doppler_scale = p_doppler_scale;
}

float FmodSound3DSettings::get_doppler_scale() const {
    return _doppler_scale;
}

void FmodSound3DSettings::set_distance_factor(const float p_distance_factor) {
    _distance_factor = p_distance_factor;
}

float FmodSound3DSettings::get_distance_factor() const {
    return _distance_factor;
}

void FmodSound3DSettings::set_rolloff_scale(const float p_rolloff_scale) {
    _rolloff_scale = p_rolloff_scale;
}

float FmodSound3DSettings::get_rolloff_scale() const {
    return _rolloff_scale;
}

Ref<FmodSound3DSettings> FmodSound3DSettings::get_from_project_settings() {
    Ref<FmodSound3DSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String doppler_scale_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, THREE_D_SETTINGS_BASE_PATH, DOPPLER_SCALE_OPTION);
    settings->set_doppler_scale(project_settings->get_setting(doppler_scale_setting_path, DEFAULT_DOPPLER_SCALE));

    String distance_factor_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, THREE_D_SETTINGS_BASE_PATH, DISTANCE_FACTOR_OPTION);
    settings->set_distance_factor(project_settings->get_setting(distance_factor_setting_path, DEFAULT_DISTANCE_FACTOR));

    String rolloff_scale_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, THREE_D_SETTINGS_BASE_PATH, ROLLOFF_SCALE_OPTION);
    settings->set_rolloff_scale(project_settings->get_setting(rolloff_scale_setting_path, DEFAULT_ROLLOFF_SCALE));

    return settings;
}

void FmodSound3DSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_doppler_scale", "p_doppler_scale"), &FmodSound3DSettings::set_doppler_scale);
    ClassDB::bind_method(D_METHOD("get_doppler_scale"), &FmodSound3DSettings::get_doppler_scale);

    ClassDB::bind_method(D_METHOD("set_distance_factor", "p_distance_factor"), &FmodSound3DSettings::set_distance_factor);
    ClassDB::bind_method(D_METHOD("get_distance_factor"), &FmodSound3DSettings::get_distance_factor);

    ClassDB::bind_method(D_METHOD("set_rolloff_scale", "p_rolloff_scale"), &FmodSound3DSettings::set_rolloff_scale);
    ClassDB::bind_method(D_METHOD("get_rolloff_scale"), &FmodSound3DSettings::get_rolloff_scale);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::FLOAT,
        "doppler_scale",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_doppler_scale",
      "get_doppler_scale"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::FLOAT,
        "distance_factor",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_distance_factor",
      "get_distance_factor"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::FLOAT,
        "rolloff_scale",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_rolloff_scale",
      "get_rolloff_scale"
    );
}