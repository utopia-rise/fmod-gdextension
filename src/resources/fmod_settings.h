#ifndef GODOTFMOD_FMOD_SETTINGS_H
#define GODOTFMOD_FMOD_SETTINGS_H

#include "fmod_studio.h"

#include <classes/resource.hpp>
#include <core/binder_common.hpp>

namespace godot {
    class FmodGeneralSettings : public Resource {
        GDCLASS(FmodGeneralSettings, Resource)
    public:
        enum StudioProcessing {
            BACKGROUND_THREAD,
            MAIN_THREAD,
        };

        enum OutputRouting {
            DEVICE,
            GODOT_BUS,
        };

        enum GodotBusMixing {
            MIX_AHEAD,
            ON_DEMAND,
        };

        void set_channel_count(const int p_channel_count);
        int get_channel_count() const;

        void set_is_live_update_enabled(const bool p_enable_live_update);
        bool get_is_live_update_enabled() const;

        void set_is_memory_tracking_enabled(const bool p_enable_memory_tracking);
        bool get_is_memory_tracking_enabled() const;

        void set_default_listener_count(int p_listener_count);
        int get_default_listener_count() const;

        void set_banks_path(const String& p_paths);
        const String& get_banks_path() const;

        void set_should_load_by_name(const bool p_should_load_by_name);
        bool get_should_load_by_name() const;

        void set_studio_processing(StudioProcessing p_studio_processing);
        StudioProcessing get_studio_processing() const;

        void set_output_routing(OutputRouting p_output_routing);
        OutputRouting get_output_routing() const;

        void set_godot_bus_mixing(GodotBusMixing p_godot_bus_mixing);
        GodotBusMixing get_godot_bus_mixing() const;

        void set_sync_to_godot_mix_rate(const bool p_sync_to_godot_mix_rate);
        bool get_sync_to_godot_mix_rate() const;

        void set_godot_audio_bus(const String& p_godot_audio_bus);
        const String& get_godot_audio_bus() const;

        static Ref<FmodGeneralSettings> get_from_project_settings();

        static constexpr const char* INITIALIZE_BASE_PATH = "General";
        static constexpr const char* CHANNEL_COUNT_OPTION = "channel_count";
        static constexpr const char* IS_LIVE_UPDATE_ENABLED_OPTION = "is_live_update_enabled";
        static constexpr const char* IS_LIVE_MEMORY_TRACKING_ENABLED_OPTION = "is_memory_tracking_enabled";
        static constexpr const char* DEFAULT_LISTENER_COUNT_OPTION = "default_listener_count";
        static constexpr const char* BANKS_PATH_OPTION = "banks_path";
        static constexpr const char* SHOULD_LOAD_BY_NAME = "should_load_by_name";
        static constexpr const char* STUDIO_PROCESSING_OPTION = "studio_processing";
        static constexpr const char* STUDIO_PROCESSING_HINT = "Background Thread,Main Thread";
        static constexpr const char* OUTPUT_BASE_PATH = "Output";
        static constexpr const char* OUTPUT_ROUTING_OPTION = "output_routing";
        static constexpr const char* OUTPUT_ROUTING_HINT = "Direct to Device,Godot Bus";
        static constexpr const char* GODOT_BUS_MIXING_OPTION = "godot_bus_mixing";
        static constexpr const char* GODOT_BUS_MIXING_HINT = "Mix Ahead,On Demand";
        static constexpr const char* SYNC_TO_GODOT_MIX_RATE_OPTION = "sync_to_godot_mix_rate";
        static constexpr const char* GODOT_AUDIO_BUS_OPTION = "godot_audio_bus";

        static constexpr const int DEFAULT_CHANNEL_COUNT = 1024;
        static constexpr const bool DEFAULT_IS_LIVEUPDATE = true;
        static constexpr const bool DEFAULT_IS_MEMORY_TRACKING = false;
        static constexpr const int DEFAULT_DEFAULT_LISTENER_COUNT = 1;
        static constexpr const char* DEFAULT_BANKS_PATH = "res://";
        static constexpr const bool DEFAULT_SHOULD_LOAD_BY_NAME = false;
        static constexpr const StudioProcessing DEFAULT_STUDIO_PROCESSING = BACKGROUND_THREAD;
        static constexpr const OutputRouting DEFAULT_OUTPUT_ROUTING = DEVICE;
        static constexpr const GodotBusMixing DEFAULT_GODOT_BUS_MIXING = MIX_AHEAD;
        static constexpr const bool DEFAULT_SYNC_TO_GODOT_MIX_RATE = false;
        static constexpr const char* DEFAULT_GODOT_AUDIO_BUS = "Master";

    private:
        int _channel_count = DEFAULT_CHANNEL_COUNT;
        int _default_listener_count = DEFAULT_DEFAULT_LISTENER_COUNT;
        bool _is_live_update_enabled = DEFAULT_IS_LIVEUPDATE;
        bool _is_memory_tracking_enabled = DEFAULT_IS_MEMORY_TRACKING;
        String _banks_path;
        bool _should_load_by_name = DEFAULT_SHOULD_LOAD_BY_NAME;
        StudioProcessing _studio_processing = DEFAULT_STUDIO_PROCESSING;
        OutputRouting _output_routing = DEFAULT_OUTPUT_ROUTING;
        GodotBusMixing _godot_bus_mixing = DEFAULT_GODOT_BUS_MIXING;
        bool _sync_to_godot_mix_rate = DEFAULT_SYNC_TO_GODOT_MIX_RATE;
        String _godot_audio_bus = DEFAULT_GODOT_AUDIO_BUS;

    protected:
        static void _bind_methods();

    };
}

VARIANT_ENUM_CAST(FmodGeneralSettings::StudioProcessing);
VARIANT_ENUM_CAST(FmodGeneralSettings::OutputRouting);
VARIANT_ENUM_CAST(FmodGeneralSettings::GodotBusMixing);

#endif// GODOTFMOD_FMOD_SETTINGS_H
