#include "fmod_logging.h"

#include <classes/project_settings.hpp>
#include <fmod.hpp>
#include <sstream>
#include <variant/string.hpp>
#include <variant/utility_functions.hpp>

namespace godot {

    // initialize FMOD logging based on project settings
    void logging_init() {
        const Ref<FmodLoggingSettings> p_logging_settings = FmodLoggingSettings::get_from_project_settings();

        if (p_logging_settings.is_valid()) {
            unsigned int debug_flags = p_logging_settings->_debug_level_to_fmod();
            FMOD_DEBUG_MODE log_output = static_cast<FMOD_DEBUG_MODE>(p_logging_settings->get_log_output());

            switch (log_output) {
                case FMOD_DEBUG_MODE_TTY: {
                    // Output to terminal/console
                    FMOD::Debug_Initialize(debug_flags, FMOD_DEBUG_MODE_TTY, nullptr, nullptr);
                    break;
                }

                case FMOD_DEBUG_MODE_CALLBACK: {
                    // Output to a callback -> GODOT
                    FMOD::Debug_Initialize(debug_flags, FMOD_DEBUG_MODE_CALLBACK, fmod_debug_callback, nullptr);
                    break;
                }

                case FMOD_DEBUG_MODE_FILE: {
                    UtilityFunctions::push_warning("FMOD log output set to File");
                    // Output to a file
                    String file_path = p_logging_settings->get_log_file_path();
                    CharString file_path_utf8 = file_path.utf8();

                    file_path = ProjectSettings::get_singleton()->globalize_path(file_path);
                    file_path_utf8 = file_path.utf8();

                    FMOD::Debug_Initialize(debug_flags, FMOD_DEBUG_MODE_FILE, nullptr, file_path_utf8);
                    break;
                }

                default: {
                    // Fallback to TTY if somehow an invalid value is set
                    FMOD::Debug_Initialize(debug_flags, FMOD_DEBUG_MODE_TTY, nullptr, nullptr);
                    UtilityFunctions::push_warning("Invalid FMOD log output setting, defaulting to TTY");
                    break;
                }
            }
        }
    }

    // Direct logging function implementation
    void log_fmod_message(FMODLogLevel level, const String& message) {
        switch (level) {
            case LOG_ERROR:
                UtilityFunctions::push_error(message);
                break;
            case LOG_WARNING:
                UtilityFunctions::push_warning(message);
                break;
            case LOG_VERBOSE:
                UtilityFunctions::print_verbose(message);
                break;
            case LOG_INFO:
            default:
                UtilityFunctions::print(message);
                break;
        }
    }

    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message) {
        if (!message) { return FMOD_OK; }

        String debug_message;

        // Determine the log level and prefix
        FMODLogLevel log_level = LOG_INFO;
        if (flags & FMOD_DEBUG_LEVEL_ERROR) {
            debug_message += "[FMOD ERROR]";
            log_level = LOG_ERROR;
        } else if (flags & FMOD_DEBUG_LEVEL_WARNING) {
            debug_message += "[FMOD WARN]";
            log_level = LOG_WARNING;
        } else if (flags & FMOD_DEBUG_LEVEL_LOG) {
            debug_message += "[FMOD INFO]";
        } else {
            debug_message += "[FMOD]";
        }

        // Add type information if available
        if (flags & FMOD_DEBUG_TYPE_MEMORY) { debug_message += "[MEM]"; }
        if (flags & FMOD_DEBUG_TYPE_FILE) { debug_message += "[FILE]"; }
        if (flags & FMOD_DEBUG_TYPE_CODEC) { debug_message += "[CODEC]"; }
        if (flags & FMOD_DEBUG_TYPE_TRACE) { debug_message += "[TRACE]"; }
        if (flags & FMOD_DEBUG_TYPE_VIRTUAL) { debug_message += "[VIRT]"; }

        // Format the message based on display flags
        if ((flags & FMOD_DEBUG_DISPLAY_LINENUMBERS) && file && func) {
            debug_message += String(" ") + file + ":" + String::num_int64(line) + " in " + func + "(): " + message;
        } else if (file && func) {
            debug_message += String(" ") + file + " in " + func + "(): " + message;
        } else if (func) {
            debug_message += String(" ") + func + "(): " + message;
        } else {
            debug_message += String(" ") + message;
        }

        debug_message.strip_edges();

        log_fmod_message(log_level, debug_message);
        return FMOD_OK;
    }
    }

}// namespace godot
