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

        static Ref<FmodGeneralSettings> get_from_project_settings();

        static constexpr const char* INITIALIZE_BASE_PATH = "General";
        static constexpr const char* CHANNEL_COUNT_OPTION = "channel_count";
        static constexpr const char* IS_LIVE_UPDATE_ENABLED_OPTION = "is_live_update_enabled";
        static constexpr const char* IS_LIVE_MEMORY_TRACKING_ENABLED_OPTION = "is_memory_tracking_enabled";
        static constexpr const char* DEFAULT_LISTENER_COUNT_OPTION = "default_listener_count";
        static constexpr const char* BANKS_PATH_OPTION = "banks_path";

        static constexpr const int DEFAULT_CHANNEL_COUNT = 1024;
        static constexpr const bool DEFAULT_IS_LIVEUPDATE = true;
        static constexpr const bool DEFAULT_IS_MEMORY_TRACKING = false;
        static constexpr const int DEFAULT_DEFAULT_LISTENER_COUNT = 1;
        static constexpr const char* DEFAULT_BANKS_PATH = "res://";

    private:
        int _channel_count;
        int _default_listener_count;
        bool _is_live_update_enabled;
        bool _is_memory_tracking_enabled;
        String _banks_path;

    protected:
        static void _bind_methods();

    };
}

#endif// GODOTFMOD_FMOD_SETTINGS_H
