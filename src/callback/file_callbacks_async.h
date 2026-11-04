#ifndef GODOTFMOD_FILE_CALLBACKS_ASYNC_H
#define GODOTFMOD_FILE_CALLBACKS_ASYNC_H

#ifndef WEB_ENABLED

#include <fmod_common.h>

#include <condition_variable>
#include <mutex>
#include <templates/list.hpp>
#include <thread>

namespace Callbacks {
    class GodotFileRunner {
    public:
        static GodotFileRunner* get_singleton();

        enum ReadPriority {
            NORMAL,
            HIGH
        };

        ~GodotFileRunner() = default;

    private:
        std::thread fileThread;

        std::condition_variable read_cv;
        std::mutex read_mut;

        std::condition_variable cancel_cv;
        std::mutex cancel_mut;

        bool stop = false;
        FMOD_ASYNCREADINFO* current_request = nullptr;
        godot::List<FMOD_ASYNCREADINFO*> requests = godot::List<FMOD_ASYNCREADINFO*>();

        GodotFileRunner() = default;
        GodotFileRunner(const GodotFileRunner&) = delete;
        GodotFileRunner& operator=(const GodotFileRunner&) = delete;

        void run();

    public:
        void queueReadRequest(FMOD_ASYNCREADINFO* request, ReadPriority priority);
        FMOD_RESULT cancelReadRequest(FMOD_ASYNCREADINFO* request);
        void start();
        void finish();
    };

    FMOD_RESULT F_CALL godotFileAsyncRead(FMOD_ASYNCREADINFO* info, void* userdata);
    FMOD_RESULT F_CALL godotFileAsyncCancel(FMOD_ASYNCREADINFO* info, void* userdata);
} // namespace Callbacks

#endif

#endif // GODOTFMOD_FILE_CALLBACKS_ASYNC_H
