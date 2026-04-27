#include "file_callbacks.h"

namespace Callbacks {

    GodotFileRunner* GodotFileRunner::get_singleton() {
        static GodotFileRunner singleton;
        return &singleton;
    }

    void GodotFileRunner::queueReadRequest(FMOD_ASYNCREADINFO* request, ReadPriority priority) {
#if !defined(WEB_ENABLED)
        // High-priority requests have to be processed first.
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
#else
        // On Web, threaded async IO is disabled. These callbacks should not be
        // used because we don't register a custom file system in that build.
        (void)request;
        (void)priority;
#endif
    }

    FMOD_RESULT GodotFileRunner::cancelReadRequest(FMOD_ASYNCREADINFO* request) {
#if !defined(WEB_ENABLED)
        // lock so we can't add and remove elements from the queue at the same time.
        {
            std::lock_guard<std::mutex> lk(read_mut);
            if (requests.erase(request)) {
                request->bytesread = 0;
                request->done(request, FMOD_RESULT::FMOD_ERR_FILE_DISKEJECTED);
                return FMOD_RESULT::FMOD_ERR_FILE_DISKEJECTED;
            }
        }

        // We lock and check if the current request is the one being canceled.
        // In this case, we wait until it's done.
        {
            std::unique_lock<std::mutex> lk(cancel_mut);
            if (request == current_request) { cancel_cv.wait(lk); }
        }

        return FMOD_RESULT::FMOD_OK;
#else
        (void)request;
        return FMOD_RESULT::FMOD_OK;
#endif
    }

// The worker thread entry point exists only on non-web platforms.
#if !defined(WEB_ENABLED)
    void GodotFileRunner::run() {
        while (!stop) {
            // waiting for the container to have one request
            {
                std::unique_lock<std::mutex> lk(read_mut);
                read_cv.wait(lk, [this] { return !requests.is_empty() || stop; });
            }

            while (!requests.is_empty()) {
                // lock so we can't add and remove elements from the queue at the same time.
                // also store the current request so it cannot be canceled during processing.
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

                // Remember to return an error if the end of the file is reached
                FMOD_RESULT result = (size < current_request->sizebytes) ? FMOD_RESULT::FMOD_ERR_FILE_EOF : FMOD_RESULT::FMOD_OK;
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
#endif
    void GodotFileRunner::start() {
#if !defined(WEB_ENABLED)
        stop = false;
        fileThread = std::thread(&GodotFileRunner::run, this);
#endif
    }

    void GodotFileRunner::finish() {
#if !defined(WEB_ENABLED)
        stop = true;
        // we need to notify the loop one last time, otherwise it will stay stuck in the wait method.
        read_cv.notify_one();
        if (fileThread.joinable()) {
            fileThread.join();
        }
#endif
    }

    FMOD_RESULT F_CALL godotFileOpen(const char* name, unsigned int* filesize, void** handle, void* userdata) {
        godot::Ref<godot::FileAccess> access = godot::FileAccess::open(name, godot::FileAccess::ModeFlags::READ);

        if (access->get_error() == godot::Error::OK) {
            *filesize = access->get_length();
            GodotFileHandle* fileHandle {new GodotFileHandle {access}};
            *handle = reinterpret_cast<void*>(fileHandle);
            return FMOD_RESULT::FMOD_OK;
        }
        return FMOD_RESULT::FMOD_ERR_FILE_NOTFOUND;
    }

    FMOD_RESULT F_CALL godotFileClose(void* handle, void* userdata) {
        godot::Ref<godot::FileAccess> file {reinterpret_cast<GodotFileHandle*>(handle)->file};
        delete reinterpret_cast<GodotFileHandle*>(handle);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotFileRead(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata) {
        GodotFileHandle* fileHandle {reinterpret_cast<GodotFileHandle*>(handle)};
        godot::Ref<godot::FileAccess> file {fileHandle->file};

        godot::PackedByteArray data = file->get_buffer(sizebytes);
        const int read_size = static_cast<int>(data.size());

        if (read_size > 0) {
            memcpy(buffer, data.ptr(), read_size);
        }

        if (bytesread) {
            *bytesread = static_cast<unsigned int>(read_size);
        }

        if (read_size < static_cast<int>(sizebytes)) {
            return FMOD_RESULT::FMOD_ERR_FILE_EOF;
        }

        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotFileSeek(void* handle, unsigned int pos, void* userdata) {
        GodotFileHandle* fileHandle {reinterpret_cast<GodotFileHandle*>(handle)};
        godot::Ref<godot::FileAccess> file {fileHandle->file};

        file->seek(pos);

        if (file->get_position() != pos) {
            return FMOD_RESULT::FMOD_ERR_FILE_COULDNOTSEEK;
        }

        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotSyncRead(FMOD_ASYNCREADINFO* info, void* userdata) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        int priority {info->priority};

        GodotFileRunner::ReadPriority priorityRank;
        if (priority >= 50) {
            priorityRank = GodotFileRunner::ReadPriority::HIGH;
        } else {
            priorityRank = GodotFileRunner::ReadPriority::NORMAL;
        }
        runner->queueReadRequest(info, priorityRank);
        return FMOD_RESULT::FMOD_OK;
    }

    FMOD_RESULT F_CALL godotSyncCancel(FMOD_ASYNCREADINFO* info, void* userdata) {
        GodotFileRunner* runner {GodotFileRunner::get_singleton()};
        return runner->cancelReadRequest(info);
    }
}// namespace Callbacks