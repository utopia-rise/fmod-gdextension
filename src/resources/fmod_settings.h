#ifndef GODOTFMOD_FMOD_SETTINGS_H
#define GODOTFMOD_FMOD_SETTINGS_H

#include "fmod_common.h"
#include "fmod_plugins_settings.h"

#include <classes/resource.hpp>
#include <core/binder_common.hpp>

namespace godot {
    // A Project Setting of FMOD: its path below "Fmod/", the FmodSettings property it sets, whether it shows without
    // Advanced Settings, and the path an older version stored it at. Its type and hint are those of the property.
    struct FmodProjectSetting {
        const char* path;
        const char* property;
        bool basic;
        const char* old_path;
    };

    // Everything FMOD starts with. FmodServer.init() takes one, or reads it from the Project Settings; a new one holds
    // the defaults of the platform the game runs on.
    class FmodSettings : public Resource {
        GDCLASS(FmodSettings, Resource)

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

        enum OutputRouting {
            DEVICE,
            GODOT_BUS,
        };

        enum GodotBusMixing {
            MIX_AHEAD,
            ON_DEMAND,
        };

        enum StudioProcessing {
            BACKGROUND_THREAD,
            MAIN_THREAD,
        };

        enum LogLevel {
            LOG_LEVEL_INHERIT,
            LOG_LEVEL_NONE,
            LOG_LEVEL_ERROR,
            LOG_LEVEL_WARNING,
            LOG_LEVEL_LOG,
            LOG_LEVEL_VERBOSE,
        };

        enum LogOutput {
            LOG_OUTPUT_TTY = FMOD_DEBUG_MODE_TTY,
            LOG_OUTPUT_FILE = FMOD_DEBUG_MODE_FILE,
            LOG_OUTPUT_GODOT = FMOD_DEBUG_MODE_CALLBACK,
        };

        static constexpr const char* BANKS_PATH_OPTION = "General/banks/path";
        static constexpr const char* LOAD_BY_NAME_OPTION = "General/banks/load_by_name";
        static constexpr const char* OUTPUT_ROUTING_OPTION = "Mixer/output/routing";
        static constexpr const char* GODOT_AUDIO_BUS_OPTION = "Mixer/output/godot_audio_bus";
        static constexpr const char* GODOT_BUS_MIXING_OPTION = "Mixer/output/godot_bus_mixing";
        static constexpr const char* SYNC_TO_GODOT_MIX_RATE_OPTION = "Mixer/output/sync_to_godot_mix_rate";

        static constexpr FmodProjectSetting PROJECT_SETTINGS[] = {
            {BANKS_PATH_OPTION, "banks_path", true, "General/banks_path"},
            {LOAD_BY_NAME_OPTION, "load_by_name", true, "General/should_load_by_name"},
            {"Mixer/voices/channel_count", "channel_count", true, "General/channel_count"},
            {"Mixer/voices/real_channel_count", "real_channel_count", false, nullptr},
            {"Mixer/voices/per_codec_channel_count", "per_codec_channel_count", false, nullptr},
            {"Mixer/format/sample_rate", "sample_rate", true, "Software Format/sample_rate"},
            {"Mixer/format/speaker_mode", "speaker_mode", true, "Software Format/speaker_mode"},
            {"Mixer/format/raw_speaker_count", "raw_speaker_count", false, "Software Format/raw_speaker_count"},
            {OUTPUT_ROUTING_OPTION, "output_routing", true, nullptr},
            {GODOT_AUDIO_BUS_OPTION, "godot_audio_bus", true, nullptr},
            {GODOT_BUS_MIXING_OPTION, "godot_bus_mixing", true, nullptr},
            {SYNC_TO_GODOT_MIX_RATE_OPTION, "sync_to_godot_mix_rate", false, nullptr},
            {"Mixer/latency/buffer_size", "dsp_buffer_size", true, "DSP/dsp_buffer_size"},
            {"Mixer/latency/buffer_count", "dsp_buffer_count", true, "DSP/dsp_buffer_count"},
            {"Mixer/latency/studio_processing", "studio_processing", false, nullptr},
            {"Spatialization/listener_count", "listener_count", true, "General/default_listener_count"},
            {"Spatialization/distance_factor_2d", "distance_factor_2d", true, nullptr},
            {"Spatialization/distance_factor_3d", "distance_factor_3d", true, nullptr},
            {"Spatialization/rolloff_scale", "rolloff_scale", true, "3D Settings/rolloff_scale"},
            {"Spatialization/doppler_scale", "doppler_scale", true, "3D Settings/doppler_scale"},
            {"Debugging/live_update/enabled", "live_update", true, "General/is_live_update_enabled"},
            {"Debugging/live_update/memory_tracking", "memory_tracking", false, "General/is_memory_tracking_enabled"},
            {"Debugging/logging/level", "log_level", true, "Logging/debug_level"},
            {"Debugging/logging/output", "log_output", true, "Logging/log_output"},
            {"Debugging/logging/file_path", "log_file_path", false, "Logging/log_file_path"},
        };

        // FMOD's platform guides: mobile CPUs afford fewer voices, iOS needs 4096 samples buffered for lock screen
        // audio, and a single-threaded web build mixes on the main thread, which stutters below 2 blocks of 2048.
#if defined(WEB_ENABLED) && !defined(THREADS_ENABLED)
        static constexpr int DEFAULT_CHANNEL_COUNT = 512;
        static constexpr int DEFAULT_DSP_BUFFER_SIZE = 2048;
        static constexpr int DEFAULT_DSP_BUFFER_COUNT = 2;
        static constexpr StudioProcessing DEFAULT_STUDIO_PROCESSING = MAIN_THREAD;
        static constexpr bool DEFAULT_LIVE_UPDATE = false;
#elif defined(WEB_ENABLED)
        static constexpr int DEFAULT_CHANNEL_COUNT = 512;
        static constexpr int DEFAULT_DSP_BUFFER_SIZE = 1024;
        static constexpr int DEFAULT_DSP_BUFFER_COUNT = 4;
        static constexpr StudioProcessing DEFAULT_STUDIO_PROCESSING = BACKGROUND_THREAD;
        static constexpr bool DEFAULT_LIVE_UPDATE = false;
#elif defined(ANDROID_ENABLED) || defined(IOS_ENABLED)
        static constexpr int DEFAULT_CHANNEL_COUNT = 512;
        static constexpr int DEFAULT_DSP_BUFFER_SIZE = 1024;
        static constexpr int DEFAULT_DSP_BUFFER_COUNT = 4;
        static constexpr StudioProcessing DEFAULT_STUDIO_PROCESSING = BACKGROUND_THREAD;
        static constexpr bool DEFAULT_LIVE_UPDATE = true;
#else
        static constexpr int DEFAULT_CHANNEL_COUNT = 1024;
        static constexpr int DEFAULT_DSP_BUFFER_SIZE = 512;
        static constexpr int DEFAULT_DSP_BUFFER_COUNT = 4;
        static constexpr StudioProcessing DEFAULT_STUDIO_PROCESSING = BACKGROUND_THREAD;
        static constexpr bool DEFAULT_LIVE_UPDATE = true;
#endif

        void set_banks_path(const String& p_banks_path);
        const String& get_banks_path() const;
        void set_load_by_name(bool p_load_by_name);
        bool get_load_by_name() const;

        void set_channel_count(int p_channel_count);
        int get_channel_count() const;
        void set_real_channel_count(int p_real_channel_count);
        int get_real_channel_count() const;
        void set_per_codec_channel_count(int p_per_codec_channel_count);
        int get_per_codec_channel_count() const;
        void set_sample_rate(int p_sample_rate);
        int get_sample_rate() const;
        void set_speaker_mode(SpeakerMode p_speaker_mode);
        SpeakerMode get_speaker_mode() const;
        void set_raw_speaker_count(int p_raw_speaker_count);
        int get_raw_speaker_count() const;
        void set_output_routing(OutputRouting p_output_routing);
        OutputRouting get_output_routing() const;
        void set_godot_audio_bus(const String& p_godot_audio_bus);
        const String& get_godot_audio_bus() const;
        void set_godot_bus_mixing(GodotBusMixing p_godot_bus_mixing);
        GodotBusMixing get_godot_bus_mixing() const;
        void set_sync_to_godot_mix_rate(bool p_sync_to_godot_mix_rate);
        bool get_sync_to_godot_mix_rate() const;
        void set_dsp_buffer_size(int p_dsp_buffer_size);
        int get_dsp_buffer_size() const;
        void set_dsp_buffer_count(int p_dsp_buffer_count);
        int get_dsp_buffer_count() const;
        void set_studio_processing(StudioProcessing p_studio_processing);
        StudioProcessing get_studio_processing() const;

        void set_listener_count(int p_listener_count);
        int get_listener_count() const;
        void set_distance_factor_2d(float p_distance_factor);
        float get_distance_factor_2d() const;
        void set_distance_factor_3d(float p_distance_factor);
        float get_distance_factor_3d() const;
        void set_rolloff_scale(float p_rolloff_scale);
        float get_rolloff_scale() const;
        void set_doppler_scale(float p_doppler_scale);
        float get_doppler_scale() const;

        void set_live_update(bool p_live_update);
        bool get_live_update() const;
        void set_memory_tracking(bool p_memory_tracking);
        bool get_memory_tracking() const;
        void set_log_level(LogLevel p_log_level);
        LogLevel get_log_level() const;
        unsigned int get_fmod_debug_flags() const;
        void set_log_output(LogOutput p_log_output);
        LogOutput get_log_output() const;
        void set_log_file_path(const String& p_log_file_path);
        const String& get_log_file_path() const;

        void set_plugins(const Ref<FmodPluginsSettings>& p_plugins);
        Ref<FmodPluginsSettings> get_plugins() const;

        static Ref<FmodSettings> get_from_project_settings();
        static void free_project_settings();

    private:
        String _banks_path = "res://";
        bool _load_by_name = false;
        int _channel_count = DEFAULT_CHANNEL_COUNT;
        int _real_channel_count = 64;
        int _per_codec_channel_count = 32;
        int _sample_rate = 48000;
        SpeakerMode _speaker_mode = STEREO;
        int _raw_speaker_count = 0;
        OutputRouting _output_routing = DEVICE;
        String _godot_audio_bus = "Master";
        GodotBusMixing _godot_bus_mixing = MIX_AHEAD;
        bool _sync_to_godot_mix_rate = false;
        int _dsp_buffer_size = DEFAULT_DSP_BUFFER_SIZE;
        int _dsp_buffer_count = DEFAULT_DSP_BUFFER_COUNT;
        StudioProcessing _studio_processing = DEFAULT_STUDIO_PROCESSING;
        int _listener_count = 1;
        float _distance_factor_2d = 64;
        float _distance_factor_3d = 1;
        float _rolloff_scale = 1;
        float _doppler_scale = 1;
        bool _live_update = DEFAULT_LIVE_UPDATE;
        bool _memory_tracking = false;
        LogLevel _log_level = LOG_LEVEL_INHERIT;
        LogOutput _log_output = LOG_OUTPUT_GODOT;
        String _log_file_path = "user://fmod.log";
        Ref<FmodPluginsSettings> _plugins;

        static Ref<FmodSettings> cached_project_settings;

    protected:
        static void _bind_methods();
    };
} // namespace godot

VARIANT_ENUM_CAST(FmodSettings::SpeakerMode);
VARIANT_ENUM_CAST(FmodSettings::OutputRouting);
VARIANT_ENUM_CAST(FmodSettings::GodotBusMixing);
VARIANT_ENUM_CAST(FmodSettings::StudioProcessing);
VARIANT_ENUM_CAST(FmodSettings::LogLevel);
VARIANT_ENUM_CAST(FmodSettings::LogOutput);

#endif // GODOTFMOD_FMOD_SETTINGS_H
