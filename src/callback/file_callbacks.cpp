#include "file_callbacks.h"

namespace Callbacks {

    GodotFileRunner* GodotFileRunner::get_singleton() {
        static GodotFileRunner singleton;
        return &singleton;
    }

    void GodotFileRunner::queueReadRequest(FMOD_ASYNCREADINFO* request, ReadPriority priority) {
        // High priority requests have to be processed first.
        if (priority == ReadPriority::HIGH) {
            // lock so we can't add and remove elements from the queue at the same time.
            std::lock_guard<std::mutex> lk(read_mut);
            requests.push_front(request);
        } else {
            // lock so we can't add and remove elements from the queue at the same time.
            std::lock_guard<std::mutex> lk(read_mut);
            requests.push_back(request);
        }
        read_cv.notify_one();
    }

    void GodotFileRunner::cancelReadRequest(FMOD_ASYNCREADINFO* request) {
        // lock so we can't add and remove elements from the queue at the same time.
        {
            std::lock_guard<std::mutex> lk(read_mut);
            requests.erase(request);
        }

        // We lock and check if the current request is the one being canceled.
        // In this case, we wait until it's done.
        {
            std::unique_lock<std::mutex> lk(cancel_mut);
            if (request == current_request) { cancel_cv.wait(lk); }
        }
    }

    void GodotFileRunner::run() {
        while (!stop) {
            // waiting for the container to have one request
            {
                std::unique_lock<std::mutex> lk(read_mut);
                read_cv.wait(lk, [this] { return !requests.is_empty() || stop; });
            }

            while (!requests.is_empty()) {
                // lock so we can't add and remove elements from the queue at the same time.
                // also store the current request so it cannot be cancel during process.
                {
                    std::lock_guard<std::mutex> lk(read_mut);
                    current_request = requests.front()->get();
                    requests.pop_front();
                }

                // We get the Godot File object from the handle
                GodotFileHandle* handle {reinterpret_cast<GodotFileHandle*>(current_request->handle)};
                godot::Ref<godot::FileAccess> file {handle->file};

                // update the position of the cursor
                file->seek(current_request->offset);

                // We read and store the requested data in an array.
                godot::PackedByteArray buffer {file->get_buffer(current_request->sizebytes)};
                int size {static_cast<int>(buffer.size())};
                const uint8_t* data {buffer.ptr()};

                // We copy the data to FMOD buffer
                memcpy(current_request->buffer, data, size * sizeof(uint8_t));
                current_request->bytesread = size;

                // Don't forget the return an error if end of the file is reached
                FMOD_RESULT result;
                if (file->eof_reached()) {
                    result = FMOD_RESULT::FMOD_ERR_FILE_EOF;
                } else {
                    result = FMOD_RESULT::FMOD_OK;
                }
                current_request->done(current_request, result);

                // Request no longer processed
                {
                    std::lock_guard<std::mutex> lk(cancel_mut);
                    current_request = nullptr;
                }
                cancel_cv.notify_one();
            }
        }
    }

    void GodotFileRunner::start() {
        stop = false;
        fileThread = std::thread(&GodotFileRunner::run, this);
    }

    void GodotFileRunner::finish() {
        stop = true;
        // we need to notify the loop one last time, otherwise it will stay stuck in the wait method.
        read_cv.notify_one();
        fileThread.join();
    }

    FMOD_RESULT F_CALL godotFileOpen(const char* name, unsigned int* filesize, void** handle, void* userdata) {
        godot::Ref<godot::FileAccess> access = godot::FileAccess::open(name, godot::FileAccess::ModeFlags::READ);

        if (access->get_error() == godot::Error::OK) {
            *filesize = access->get_length();
            GodotFileHandle* fileHandle {new GodotFileHandle {access}};
            *handle = reinterpret_cast<void*>(fileHandle);
            return FMOD_RESULT::FMOD_OK;
        }
        return FMOD_RESULT::FMOD_ERR_FILE_BAD;
    }

    FMOD_RESULT F_CALL godotFileClose(void* handle, void* userdata) {
        godot::Ref<godot::FileAccess> file {reinterpret_cast<GodotFileHandle*>(handle)->file};
        delete reinterpret_cast<GodotFileHandle*>(handle);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotSyncRead(FMOD_ASYNCREADINFO* info, void* userdata) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        int priority {info->priority};

        GodotFileRunner::ReadPriority priorityRank;
        if (priority == 100) {
            priorityRank = GodotFileRunner::ReadPriority::HIGH;
        } else {
            priorityRank = GodotFileRunner::ReadPriority::NORMAL;
        }
        runner->queueReadRequest(info, priorityRank);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotSyncCancel(FMOD_ASYNCREADINFO* info, void* userdata) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        runner->cancelReadRequest(info);
        return FMOD_RESULT::FMOD_OK;
    }
}// namespace Callbacks