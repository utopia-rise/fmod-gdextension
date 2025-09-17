#ifndef GODOTFMOD_FMOD_LOGGING_H
#define GODOTFMOD_FMOD_LOGGING_H

#include "fmod_common.h"

#include <sstream>
#include <variant/string.hpp>

namespace godot {

    // Log level enumeration
    enum FMODLogLevel {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_VERBOSE// Added for verbose logging
    };

    // Core logging function
    void log_fmod_message(FMODLogLevel level, const String& message);

    // FMOD debug callback function
    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message);
    }

}// namespace godot

#endif
