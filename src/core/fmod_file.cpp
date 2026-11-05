#include "fmod_file.h"

#include "core/fmod_sound.h"
#include "fmod_server.h"
#include "helpers/common.h"

using namespace godot;

void FmodFile::_bind_methods() {
    ClassDB::bind_method(D_METHOD("create_instance"), &FmodFile::create_instance);
}

Ref<FmodFile> FmodFile::create_ref(FMOD::Sound* wrapped) {
    Ref<FmodFile> ref;
    if (wrapped) {
        ref.instantiate();
        ref->_wrapped = wrapped;
        wrapped->setUserData(ref.ptr());
    }
    return ref;
}

// FMOD's shutdown releases every sound: a file kept past it has nothing left to release or play.
FmodFile::~FmodFile() {
    if (_wrapped && FmodServer::is_initialized()) { ERROR_CHECK(_wrapped->release()); }
}

Ref<FmodSound> FmodFile::create_instance() {
    if (!_wrapped || !FmodServer::is_initialized()) {
        GODOT_LOG_ERROR("Cannot create an instance of a sound file: FMOD is not running.")
        return {};
    }

    FMOD::System* core_system = nullptr;
    ERROR_CHECK(_wrapped->getSystemObject(&core_system));
    FMOD::Channel* channel = nullptr;
    ERROR_CHECK_WITH_REASON(core_system->playSound(_wrapped, nullptr, true, &channel), "Cannot play sound");
    if (!channel) { return {}; }
#ifdef WEB_ENABLED
    // Once a Studio event has been stopped, FMOD 2.04.00's web libraries mix a stereo sound's channel as a mono one
    // (interleaved samples on one speaker) unless its mix matrix is set: set FMOD's default one, explicitly.
    // TODO: remove once a FMOD release fixes it.
    int sound_channels = 0;
    ERROR_CHECK(_wrapped->getFormat(nullptr, nullptr, &sound_channels, nullptr));
    if (sound_channels == 2) {
        float stereo_matrix[] {1, 0, 0, 1};
        ERROR_CHECK_WITH_REASON(channel->setMixMatrix(stereo_matrix, 2, 2), "Cannot set the mix matrix of sound");
    }
#endif
    return FmodSound::create_ref(channel, Ref<FmodFile>(this));
}
