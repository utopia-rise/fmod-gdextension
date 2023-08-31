#include "fmod_dsp_settings.h"

#include <constants.h>

#include <classes/project_settings.hpp>

using namespace godot;

void FmodDspSettings::set_dsp_buffer_size(const unsigned int p_dsp_buffer_size) {
    _dsp_buffer_size = p_dsp_buffer_size;
}

unsigned int FmodDspSettings::get_dsp_buffer_size() const {
    return _dsp_buffer_size;
}

void FmodDspSettings::set_dsp_buffer_count(const int p_dsp_buffer_count) {
    _dsp_buffer_count = p_dsp_buffer_count;
}

int FmodDspSettings::get_dsp_buffer_count() const {
    return _dsp_buffer_count;
}

Ref<FmodDspSettings> FmodDspSettings::get_from_project_settings() {
    Ref<FmodDspSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String dsp_buffer_size_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, DSP_SETTINGS_BASE_PATH, DSP_BUFFER_SIZE_OPTION);
    settings->set_dsp_buffer_size(project_settings->get_setting(dsp_buffer_size_setting_path, DEFAULT_DSP_BUFFER_SIZE));

    String dsp_buffer_count_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, DSP_SETTINGS_BASE_PATH, DSP_BUFFER_COUNT_OPTION);
    settings->set_dsp_buffer_count(project_settings->get_setting(dsp_buffer_count_setting_path, DEFAULT_DSP_BUFFER_COUNT));

    return settings;
}

void FmodDspSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_dsp_buffer_size", "p_dsp_buffer_size"), &FmodDspSettings::set_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_dsp_buffer_size"), &FmodDspSettings::get_dsp_buffer_size);

    ClassDB::bind_method(D_METHOD("set_dsp_buffer_count", "p_dsp_buffer_count"), &FmodDspSettings::set_dsp_buffer_count);
    ClassDB::bind_method(D_METHOD("get_dsp_buffer_count"), &FmodDspSettings::get_dsp_buffer_count);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "dsp_buffer_size",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_dsp_buffer_size",
      "get_dsp_buffer_size"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "dsp_buffer_count",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_dsp_buffer_count",
      "get_dsp_buffer_count"
    );
}
