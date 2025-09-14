#include "fmod_logging.h"

#include <fmod.hpp>
#include <sstream>
#include <variant/string.hpp>
#include <variant/utility_functions.hpp>

namespace godot {

    // Direct logging function implementation
    void log_fmod_message(FMODLogLevel level, const std::string& message) {
        String godot_message(message.c_str());

        switch (level) {
            case LOG_ERROR:
                UtilityFunctions::push_error(godot_message);
                break;
            case LOG_WARNING:
                UtilityFunctions::push_warning(godot_message);
                break;
            case LOG_VERBOSE:
                UtilityFunctions::print_verbose(godot_message);
                break;
            case LOG_INFO:
            default:
                UtilityFunctions::print(godot_message);
                break;
        }
    }

    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message) {
        if (!message) { return FMOD_OK; }

        std::stringstream ss;

        // Determine the log level and prefix
        FMODLogLevel log_level = LOG_INFO;
        if (flags & FMOD_DEBUG_LEVEL_ERROR) {
            ss << "[FMOD ERROR]";
            log_level = LOG_ERROR;
        } else if (flags & FMOD_DEBUG_LEVEL_WARNING) {
            ss << "[FMOD WARN]";
            log_level = LOG_WARNING;
        } else if (flags & FMOD_DEBUG_LEVEL_LOG) {
            ss << "[FMOD INFO]";
        } else {
            ss << "[FMOD]";
        }

        // Add type information if available
        if (flags & FMOD_DEBUG_TYPE_MEMORY) { ss << "[MEM]"; }
        if (flags & FMOD_DEBUG_TYPE_FILE) { ss << "[FILE]"; }
        if (flags & FMOD_DEBUG_TYPE_CODEC) { ss << "[CODEC]"; }
        if (flags & FMOD_DEBUG_TYPE_TRACE) { ss << "[TRACE]"; }
        if (flags & FMOD_DEBUG_TYPE_VIRTUAL) { ss << "[VIRT]"; }

        // Format the message based on display flags
        if ((flags & FMOD_DEBUG_DISPLAY_LINENUMBERS) && file && func) {
            ss << " " << file << ":" << line << " in " << func << "(): " << message;
        } else if (file && func) {
            ss << " " << file << " in " << func << "(): " << message;
        } else if (func) {
            ss << " " << func << "(): " << message;
        } else {
            ss << " " << message;
        }

        std::string debug_message = ss.str();

        // Remove trailing newlines (Godot adds its own)
        while (!debug_message.empty() && debug_message.back() == '\n') {
            debug_message.pop_back();
        }

        log_fmod_message(log_level, debug_message);
        return FMOD_OK;
    }
    }

}// namespace godot
