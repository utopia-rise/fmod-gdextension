#ifndef GODOTFMOD_FMOD_LOGGING_H
#define GODOTFMOD_FMOD_LOGGING_H

#include "fmod_common.h"

#include <sstream>
#include <string>

namespace godot {

    // Log level enumeration
    enum FMODLogLevel {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_VERBOSE// Added for verbose logging
    };

    // Core logging function
    void log_fmod_message(FMODLogLevel level, const std::string& message);

    // Helper function to format messages with file/line info
    inline std::string format_log_message(const std::string& message, const char* func = nullptr, const char* file = nullptr, int line = -1) {
        std::stringstream ss;
        ss << message;

        if (file && func && line >= 0) {
            ss << " [" << file << ":" << line << " in " << func << "()]";
        } else if (func) {
            ss << " [in " << func << "()]";
        }

        return ss.str();
    }

    // FMOD debug callback function
    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message);
    }

}// namespace godot

#endif
