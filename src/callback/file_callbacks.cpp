#include "file_callbacks.h"
#include <OS.hpp>

namespace Callbacks{

    GodotFileRunner* GodotFileRunner::get_singleton() {
        static GodotFileRunner* singleton {new GodotFileRunner()};
        return singleton;
    }

    void GodotFileRunner::queueReadRequest(FMOD_ASYNCREADINFO* request, ReadPriority priority) {
        //High priority requests have to be processed first.
        if(priority == ReadPriority::HIGH) {
            //lock so we add and remove elements from the queue at the same time.
            mut->lock();
            requests.push_front_value(request);
            mut->unlock();
        }
        else{
            //lock so we add and remove elements from the queue at the same time.
            mut->lock();
            requests.push_back_value(request);
            mut->unlock();
        }
    }

    void GodotFileRunner::cancelReadRequest(FMOD_ASYNCREADINFO* request) {
        //lock so we add and remove elements from the queue at the same time.
        mut->lock();
        requests.erase(request);
        mut->unlock();
    }

    void GodotFileRunner::run() {
        while(!stop) {
            while(!requests.empty()) {
                //lock so we add and remove elements from the queue at the same time.
                mut->lock();
                FMOD_ASYNCREADINFO* request {requests.pop_front_value()};
                mut->unlock();

                //We get the Godot File object from the handle
                GodotFileHandle* handle {reinterpret_cast<GodotFileHandle*>(request->handle)};
                godot::Ref<godot::File> file {handle->file};

                //update the position of the cursor
                file->seek(request->offset);

                //We read and store the requested data in an array.
                godot::PoolByteArray buffer {file->get_buffer(request->sizebytes)};
                int size {buffer.size()};
                const uint8_t* data {buffer.read().ptr()};

                //We copy the data to FMOD buffer
                memcpy(request->buffer, data, size * sizeof(uint8_t));
                request->bytesread = size;

                //Don't forget the return an error if end of the file is reached
                FMOD_RESULT result;
                if(file->eof_reached()) {
                    result = FMOD_RESULT::FMOD_ERR_FILE_EOF;
                }
                else {
                    result = FMOD_RESULT::FMOD_OK;
                }
                request->done(request, result);
            }
            //We don't want a busy loop so we add a delay.
            //Todo: Find a better way to handle the tick rate.
            godot::OS::get_singleton()->delay_msec(5);
        }
    }

    void GodotFileRunner::start() {
        stop = false;
        fileThread = std::thread(&GodotFileRunner::run, this);
    }

    void GodotFileRunner::finish(){
        stop = true;
        fileThread.join();
    }


    FMOD_RESULT F_CALLBACK godotFileOpen(
            const char *name,
            unsigned int *filesize,
            void **handle,
            void *userdata
    ) {
        godot::Ref<godot::File> file;
        file.instance();

        GodotFileHandle* fileHandle {new GodotFileHandle{file}};

        godot::Error err = file->open(name, godot::File::ModeFlags::READ);
        *filesize = file->get_len();
        *handle = reinterpret_cast<void *>(fileHandle);

        if(err == godot::Error::OK) {
            return FMOD_RESULT::FMOD_OK;
        }
        return FMOD_RESULT::FMOD_ERR_FILE_BAD;
    }

    FMOD_RESULT F_CALLBACK godotFileClose(
            void *handle,
            void *userdata
    ) {
        godot::Ref<godot::File> file {reinterpret_cast<GodotFileHandle*>(handle)->file};
        file->close();
        free(handle);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK godotSyncRead(
            FMOD_ASYNCREADINFO *info,
            void *userdata
    ) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        int priority {info->priority};

        GodotFileRunner::ReadPriority priorityRank;
        if(priority == 100){
            priorityRank = GodotFileRunner::ReadPriority::HIGH;
        }
        else {
            priorityRank = GodotFileRunner::ReadPriority::NORMAL;
        }
        runner->queueReadRequest(info, priorityRank);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK godotSyncCancel(
            FMOD_ASYNCREADINFO *info,
            void *userdata
    ) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        runner->cancelReadRequest(info);
        return FMOD_RESULT::FMOD_OK;
    }
}