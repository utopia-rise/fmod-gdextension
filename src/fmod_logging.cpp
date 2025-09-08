#include "fmod_logging.h"

#include <fmod.hpp>
#include <sstream>
#include <variant/string.hpp>
#include <variant/utility_functions.hpp>

namespace godot {

    // FMODLogQueue implementation
    void FMODLogQueue::push(LogLevel level, const std::string& message) {
        if (shutting_down_.load()) { return; }

        std::lock_guard<std::mutex> lock(mutex_);
        messages_.push({level, message});
    }

    void FMODLogQueue::processQueue() {
        std::queue<LogMessage> localQueue;

        // Quickly swap queues under lock to minimize lock time
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::swap(localQueue, messages_);
        }

        // Process messages outside of lock
        while (!localQueue.empty()) {
            const LogMessage& msg = localQueue.front();
            String godot_message(msg.message.c_str());

            switch (msg.level) {
                case LOG_ERROR:
                    UtilityFunctions::push_error(godot_message);
                    break;
                case LOG_WARNING:
                    UtilityFunctions::push_warning(godot_message);
                    break;
                case LOG_INFO:
                default:
                    UtilityFunctions::print(godot_message);
                    break;
            }

            localQueue.pop();
        }
    }

    void FMODLogQueue::clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::queue<LogMessage> empty;
        std::swap(messages_, empty);
    }

    void FMODLogQueue::shutdown() {
        shutting_down_.store(true);
        // Process any remaining messages one last time
        processQueue();

        // Clear anything that might have been added during processing
        clear();
    }

    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message) {
        if (!message) { return FMOD_OK; }

        std::stringstream ss;

        // Determine the log level and prefix
        FMODLogQueue::LogLevel log_level = FMODLogQueue::LOG_INFO;
        if (flags & FMOD_DEBUG_LEVEL_ERROR) {
            ss << "[FMOD ERROR]";
            log_level = FMODLogQueue::LOG_ERROR;
        } else if (flags & FMOD_DEBUG_LEVEL_WARNING) {
            ss << "[FMOD WARN]";
            log_level = FMODLogQueue::LOG_WARNING;
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

        // Queue the message for processing on main thread
        FMODLogQueue::getInstance().push(log_level, debug_message);

        return FMOD_OK;
    }
    }

    void process_fmod_log_queue() {
        FMODLogQueue::getInstance().processQueue();
    }

}// namespace godot
