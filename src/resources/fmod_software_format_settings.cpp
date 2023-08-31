#include "fmod_software_format_settings.h"

#include <constants.h>

#include <classes/project_settings.hpp>

using namespace godot;

void FmodSoftwareFormatSettings::set_sample_rate(const int p_sample_rate) {
    _sample_rate = p_sample_rate;
}

int FmodSoftwareFormatSettings::get_sample_rate() const {
    return _sample_rate;
}

void FmodSoftwareFormatSettings::set_speaker_mode(const int p_speaker_mode) {
    _speaker_mode = p_speaker_mode;
}

int FmodSoftwareFormatSettings::get_speaker_mode() const {
    return _speaker_mode;
}

void FmodSoftwareFormatSettings::set_raw_speakers_count(const int p_raw_speakers_count) {
    _raw_speakers_count = p_raw_speakers_count;
}

int FmodSoftwareFormatSettings::get_raw_speakers_count() const {
    return _raw_speakers_count;
}

Ref<FmodSoftwareFormatSettings> FmodSoftwareFormatSettings::get_from_project_settings() {
    Ref<FmodSoftwareFormatSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String sample_rate_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, SOFTWARE_FORMAT_SETTINGS_BASE_PATH, SAMPLE_RATE_OPTION);
    settings->set_sample_rate(project_settings->get_setting(sample_rate_setting_path, DEFAULT_SAMPLE_RATE));

    String speaker_mode_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, SOFTWARE_FORMAT_SETTINGS_BASE_PATH, SPEAKER_MODE_OPTION);
    settings->set_speaker_mode(project_settings->get_setting(speaker_mode_setting_path, DEFAULT_SPEAKER_MODE));

    String raw_speaker_count_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, SOFTWARE_FORMAT_SETTINGS_BASE_PATH, RAW_SPEAKER_COUNT_OPTION);
    settings->set_raw_speakers_count(project_settings->get_setting(raw_speaker_count_setting_path, DEFAULT_RAW_SPEAKER_COUNT));

    return settings;
}

void FmodSoftwareFormatSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_sample_rate", "p_sample_rate"), &FmodSoftwareFormatSettings::set_sample_rate);
    ClassDB::bind_method(D_METHOD("get_sample_rate"), &FmodSoftwareFormatSettings::get_sample_rate);

    ClassDB::bind_method(D_METHOD("set_speaker_mode", "p_speaker_mode"), &FmodSoftwareFormatSettings::set_speaker_mode);
    ClassDB::bind_method(D_METHOD("get_speaker_mode"), &FmodSoftwareFormatSettings::get_speaker_mode);

    ClassDB::bind_method(D_METHOD("set_raw_speakers_count", "p_raw_speakers_count"), &FmodSoftwareFormatSettings::set_raw_speakers_count);
    ClassDB::bind_method(D_METHOD("get_raw_speakers_count"), &FmodSoftwareFormatSettings::get_raw_speakers_count);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "sample_rate",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_sample_rate",
      "get_sample_rate"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "speaker_mode",
        PROPERTY_HINT_ENUM,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_speaker_mode",
      "get_speaker_mode"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "raw_speakers_count",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_raw_speakers_count",
      "get_raw_speakers_count"
    );
}
