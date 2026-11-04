#ifndef GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H
#define GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H

#include "fmod_common.h"

#include <classes/resource.hpp>
#include <core/binder_common.hpp>

namespace godot {
    class FmodSoftwareFormatSettings : public Resource {
        GDCLASS(FmodSoftwareFormatSettings, Resource)

    public:
        enum SpeakerMode {
            DEFAULT = FMOD_SPEAKERMODE_DEFAULT,
            RAW = FMOD_SPEAKERMODE_RAW,
            MONO = FMOD_SPEAKERMODE_MONO,
            STEREO = FMOD_SPEAKERMODE_STEREO,
            QUAD = FMOD_SPEAKERMODE_QUAD,
            SURROUND = FMOD_SPEAKERMODE_SURROUND,
            SPEAKERMODE_5POINT1 = FMOD_SPEAKERMODE_5POINT1,
            SPEAKERMODE_7POINT1 = FMOD_SPEAKERMODE_7POINT1,
            SPEAKERMODE_7POINT1POINT4 = FMOD_SPEAKERMODE_7POINT1POINT4,
        };

        void set_sample_rate(const int p_sample_rate);
        int get_sample_rate() const;

        void set_speaker_mode(SpeakerMode p_speaker_mode);
        SpeakerMode get_speaker_mode() const;

        void set_raw_speakers_count(const int p_raw_speakers_count);
        int get_raw_speakers_count() const;

        static Ref<FmodSoftwareFormatSettings> get_from_project_settings();

        FmodSoftwareFormatSettings() = default;
        ~FmodSoftwareFormatSettings() = default;

        static constexpr const char* SOFTWARE_FORMAT_SETTINGS_BASE_PATH = "Software Format";

        static constexpr const char* SAMPLE_RATE_OPTION = "sample_rate";
        static constexpr const char* SPEAKER_MODE_OPTION = "speaker_mode";
        static constexpr const char* SPEAKER_MODE_HINT = "DEFAULT,RAW,MONO,STEREO,QUAD,SURROUND,5POINT1,7POINT1,7POINT1POINT4";
        static constexpr const char* RAW_SPEAKER_COUNT_OPTION = "raw_speaker_count";

        static constexpr const int DEFAULT_SAMPLE_RATE = 48000;
        static constexpr const SpeakerMode DEFAULT_SPEAKER_MODE = STEREO;
        static constexpr const int DEFAULT_RAW_SPEAKER_COUNT = 0;

    private:
        int _sample_rate = DEFAULT_SAMPLE_RATE;
        SpeakerMode _speaker_mode = DEFAULT_SPEAKER_MODE;
        int _raw_speakers_count = DEFAULT_RAW_SPEAKER_COUNT;

    protected:
        static void _bind_methods();
    };
}
VARIANT_ENUM_CAST(FmodSoftwareFormatSettings::SpeakerMode);

#endif// GODOTFMOD_FMOD_SOFTWARE_FORMAT_SETTINGS_H
