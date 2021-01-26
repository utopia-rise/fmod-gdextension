#ifndef GODOTFMOD_FILE_CALLBACKS_H
#define GODOTFMOD_FILE_CALLBACKS_H

#include <fmod_common.h>
#include <fmod_studio_common.h>
#include <Mutex.hpp>
#include <File.hpp>
#include "../helpers/containers.h"
#include <thread>

namespace Callbacks {
    struct GodotFileHandle {
        godot::Ref<godot::File> file;
    };

    class GodotFileRunner {
    public:
        static GodotFileRunner* get_singleton();

        enum ReadPriority {
            NORMAL,
            HIGH
        };

    private:

        std::thread fileThread;
        bool stop = false;
        godot::Mutex* mut = godot::Mutex::_new();
        godot::Vector<FMOD_ASYNCREADINFO*> requests = godot::Vector<FMOD_ASYNCREADINFO*>();


        GodotFileRunner() = default;
        ~GodotFileRunner() = default;

        void run();
    public:
        void queueReadRequest(FMOD_ASYNCREADINFO* request, ReadPriority priority);
        void cancelReadRequest(FMOD_ASYNCREADINFO* request);
        void start();
        void finish();

    };

    FMOD_RESULT F_CALLBACK godotFileOpen(
            const char *name,
            unsigned int *filesize,
            void **handle,
            void *userdata
    );

    FMOD_RESULT F_CALLBACK godotFileClose(
            void *handle,
            void *userdata
    );

    FMOD_RESULT F_CALLBACK godotSyncRead(
            FMOD_ASYNCREADINFO *info,
            void *userdata
    );

    FMOD_RESULT F_CALLBACK godotSyncCancel(
            FMOD_ASYNCREADINFO *info,
            void *userdata
    );
}

#endif //GODOTFMOD_FILE_CALLBACKS_H
