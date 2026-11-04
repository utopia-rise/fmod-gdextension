#ifndef GODOTFMOD_FMOD_LOGGING_SETTINGS_H
#define GODOTFMOD_FMOD_LOGGING_SETTINGS_H

#include "fmod_studio.h"

#include <classes/resource.hpp>
#include <core/binder_common.hpp>

namespace godot {

    class FmodLoggingSettings : public Resource {
        GDCLASS(FmodLoggingSettings, Resource)

    public:
        enum DebugMode {
            TTY = FMOD_DEBUG_MODE_TTY,
            FILE = FMOD_DEBUG_MODE_FILE,
            CALLBACK = FMOD_DEBUG_MODE_CALLBACK,
        };

        enum DebugLevel {
            DEBUG_INHERIT,
            DEBUG_NONE,
            DEBUG_ERROR,
            DEBUG_WARNING,
            DEBUG_LOG,
            DEBUG_VERBOSE,
        };

        void set_debug_level(DebugLevel p_debug_level);
        DebugLevel get_debug_level() const;
        int _debug_level_to_fmod() const;

        void set_log_output(DebugMode p_log_output);
        DebugMode get_log_output() const;

        void set_log_file_path(const String& p_log_file_path);
        const String& get_log_file_path() const;

        static Ref<FmodLoggingSettings> get_from_project_settings();

        static constexpr const char* LOGGING_SETTINGS_BASE_PATH = "Logging";

        // Setting keys
        static constexpr const char* DEBUG_LEVEL_OPTION = "debug_level";
        static constexpr const char* LOG_OUTPUT_OPTION = "log_output";
        static constexpr const char* LOG_FILE_PATH_OPTION = "log_file_path";

        static constexpr const char* DEBUG_LEVEL_HINT = "Inherit,None,Error,Warning,Log,Verbose";
        static constexpr const char* LOG_OUTPUT_HINT = "TTY,File,Godot";

        // Default values
        static constexpr const DebugLevel DEFAULT_DEBUG_LEVEL = DEBUG_INHERIT;
        static constexpr const DebugMode DEFAULT_LOG_OUTPUT = CALLBACK;
        static constexpr const char* DEFAULT_LOG_FILE_PATH = "user://fmod.log";

    private:
        DebugLevel _debug_level = DEFAULT_DEBUG_LEVEL;
        DebugMode _log_output = DEFAULT_LOG_OUTPUT;
        String _log_file_path;

    protected:
        static void _bind_methods();
    };

}// namespace godot

VARIANT_ENUM_CAST(FmodLoggingSettings::DebugLevel);
VARIANT_ENUM_CAST(FmodLoggingSettings::DebugMode);

#endif// GODOTFMOD_FMOD_LOGGING_SETTINGS_H
