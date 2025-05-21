#ifndef GODOTFMOD_FMOD_SETTINGS_H
#define GODOTFMOD_FMOD_SETTINGS_H

#include "fmod_studio.h"

#include <classes/resource.hpp>

namespace godot {
    class FmodGeneralSettings : public Resource {
        GDCLASS(FmodGeneralSettings, Resource)
    public:
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

        void set_plugin_paths(const PackedStringArray& p_paths);
        const PackedStringArray& get_plugin_paths() const;

        static Ref<FmodGeneralSettings> get_from_project_settings();

        static constexpr const char* INITIALIZE_BASE_PATH = "General";
        static constexpr const char* CHANNEL_COUNT_OPTION = "channel_count";
        static constexpr const char* IS_LIVE_UPDATE_ENABLED_OPTION = "is_live_update_enabled";
        static constexpr const char* IS_LIVE_MEMORY_TRACKING_ENABLED_OPTION = "is_memory_tracking_enabled";
        static constexpr const char* DEFAULT_LISTENER_COUNT_OPTION = "default_listener_count";
        static constexpr const char* BANKS_PATH_OPTION = "banks_path";
        static constexpr const char* SHOULD_LOAD_BY_NAME = "should_load_by_name";
        static constexpr const char* PLUGIN_PATHS_OPTION = "plugin_paths";

        static constexpr const int DEFAULT_CHANNEL_COUNT = 1024;
        static constexpr const bool DEFAULT_IS_LIVEUPDATE = true;
        static constexpr const bool DEFAULT_IS_MEMORY_TRACKING = false;
        static constexpr const int DEFAULT_DEFAULT_LISTENER_COUNT = 1;
        static constexpr const char* DEFAULT_BANKS_PATH = "res://";
        static constexpr const bool DEFAULT_SHOULD_LOAD_BY_NAME = false;
        static const PackedStringArray DEFAULT_PLUGIN_PATHS;

    private:
        int _channel_count;
        int _default_listener_count;
        bool _is_live_update_enabled;
        bool _is_memory_tracking_enabled;
        String _banks_path;
        bool _should_load_by_name;
        PackedStringArray _plugin_paths;

    protected:
        static void _bind_methods();

    };
}

#endif// GODOTFMOD_FMOD_SETTINGS_H
