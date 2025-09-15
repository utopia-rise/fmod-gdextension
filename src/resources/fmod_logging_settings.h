#ifndef GODOTFMOD_FMOD_LOGGING_SETTINGS_H
#define GODOTFMOD_FMOD_LOGGING_SETTINGS_H

#include "fmod_studio.h"

#include <classes/resource.hpp>

namespace godot {

    class FmodLoggingSettings : public Resource {
        GDCLASS(FmodLoggingSettings, Resource)

    public:
        enum DebugLevel {
            DEBUG_INHERIT,
            DEBUG_NONE,
            DEBUG_ERROR,
            DEBUG_WARNING,
            DEBUG_LOG,
            DEBUG_VERBOSE,
        };

        void set_debug_level(int p_debug_level);
        int get_debug_level() const;
        int _debug_level_to_fmod() const;

        void set_log_output(int p_log_output);
        int get_log_output() const;

        void set_log_file_path(const String& p_log_file_path);
        const String& get_log_file_path() const;

        static Ref<FmodLoggingSettings> get_from_project_settings();

        static constexpr const char* LOGGING_SETTINGS_BASE_PATH = "Logging";

        // Setting keys
        static constexpr const char* DEBUG_LEVEL_OPTION = "debug_level";
        static constexpr const char* LOG_OUTPUT_OPTION = "log_output";
        static constexpr const char* LOG_FILE_PATH_OPTION = "log_file_path";

        // Default values
        static constexpr const int DEFAULT_DEBUG_LEVEL = DEBUG_INHERIT;
        static constexpr const int DEFAULT_LOG_OUTPUT = FMOD_DEBUG_MODE_CALLBACK;
        static constexpr const char* DEFAULT_LOG_FILE_PATH = "user://fmod.log";

    private:
        int _debug_level;
        int _log_output;
        String _log_file_path;

    protected:
        static void _bind_methods();
    };

}// namespace godot

VARIANT_ENUM_CAST(FmodLoggingSettings::DebugLevel);
VARIANT_ENUM_CAST(FMOD_DEBUG_MODE);

#endif// GODOTFMOD_FMOD_LOGGING_SETTINGS_H
