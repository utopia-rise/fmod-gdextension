#ifndef GODOTFMOD_FMOD_LOGGING_H
#define GODOTFMOD_FMOD_LOGGING_H

#include "fmod_common.h"

#include <atomic>
#include <mutex>
#include <queue>
#include <string>

namespace godot {

    // Thread-safe message queue for deferred logging
    class FMODLogQueue {
    public:
        enum LogLevel {
            LOG_INFO,
            LOG_WARNING,
            LOG_ERROR
        };

        struct LogMessage {
            LogLevel level;
            std::string message;
        };

        static FMODLogQueue& getInstance() {
            static FMODLogQueue instance;
            return instance;
        }

        void push(LogLevel level, const std::string& message);

        // Process all queued messages (call from main thread only)
        void processQueue();

        void clear();

        void shutdown();

        // Delete copy constructor and assignment operator for singleton
        FMODLogQueue(const FMODLogQueue&) = delete;
        FMODLogQueue& operator=(const FMODLogQueue&) = delete;

    private:
        FMODLogQueue() = default;
        ~FMODLogQueue() = default;

        std::mutex mutex_;
        std::queue<LogMessage> messages_;
        std::atomic<bool> shutting_down_ {false};
    };

    // FMOD debug callback function
    extern "C" {
    FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char* file, int line, const char* func, const char* message);
    }

    // Helper function to process queued messages - call from main thread
    void process_fmod_log_queue();

}// namespace godot

#endif
