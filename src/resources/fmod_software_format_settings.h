#ifndef GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H
#define GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H

#include "fmod_common.h"

#include <classes/resource.hpp>

namespace godot {
    class FmodSoftwareFormatSettings : public Resource {
        GDCLASS(FmodSoftwareFormatSettings, Resource)

    public:
        void set_sample_rate(const int p_sample_rate);
        int get_sample_rate() const;

        void set_speaker_mode(const int p_speaker_mode);
        int get_speaker_mode() const;

        void set_raw_speakers_count(const int p_raw_speakers_count);
        int get_raw_speakers_count() const;

        static Ref<FmodSoftwareFormatSettings> get_from_project_settings();

        FmodSoftwareFormatSettings() = default;
        ~FmodSoftwareFormatSettings() = default;

        static constexpr const char* SOFTWARE_FORMAT_SETTINGS_BASE_PATH = "Software Format";

        static constexpr const char* SAMPLE_RATE_OPTION = "sample_rate";
        static constexpr const char* SPEAKER_MODE_OPTION = "speaker_mode";
        static constexpr const char* RAW_SPEAKER_COUNT_OPTION = "raw_speaker_count";

        static constexpr const int DEFAULT_SAMPLE_RATE = 48000;
        static constexpr const int DEFAULT_SPEAKER_MODE = FMOD_SPEAKERMODE::FMOD_SPEAKERMODE_STEREO;
        static constexpr const int DEFAULT_RAW_SPEAKER_COUNT = 0;

    private:
        int _sample_rate;
        int _speaker_mode;
        int _raw_speakers_count;

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H
