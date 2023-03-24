#include "fmod_sound.h"

#include "helpers/common.h"

using namespace godot;

void FmodSound::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodSound::is_valid);
    ClassDB::bind_method(D_METHOD("release"), &FmodSound::release);
    ClassDB::bind_method(D_METHOD("play"), &FmodSound::play);
    ClassDB::bind_method(D_METHOD("stop"), &FmodSound::stop);
    ClassDB::bind_method(D_METHOD("set_paused", "paused"), &FmodSound::set_paused);
    ClassDB::bind_method(D_METHOD("is_playing"), &FmodSound::is_playing);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &FmodSound::set_volume);
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodSound::get_volume);
    ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &FmodSound::set_pitch);
    ClassDB::bind_method(D_METHOD("get_pitch"), &FmodSound::get_pitch);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch"), "set_pitch", "get_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
}

void FmodSound::release() const {
    ERROR_CHECK(_wrapped->stop());
}

void FmodSound::play() const {
    set_paused(false);
}

void FmodSound::set_paused(bool paused) const {
    ERROR_CHECK(_wrapped->setPaused(paused));
}

void FmodSound::stop() const {
    ERROR_CHECK(_wrapped->stop());
}

bool FmodSound::is_playing() const {
    bool isPlaying = false;
    ERROR_CHECK(_wrapped->isPlaying(&isPlaying));
    return isPlaying;
}

void FmodSound::set_volume(float volume) const {
    ERROR_CHECK(_wrapped->setVolume(volume));
}

float FmodSound::get_volume() const {
    float volume = 0.f;
    ERROR_CHECK(_wrapped->getVolume(&volume));
    return volume;
}

float FmodSound::get_pitch() const {
    float pitch = 0.f;
    ERROR_CHECK(_wrapped->getPitch(&pitch));
    return pitch;
}

void FmodSound::set_pitch(float pitch) {
    ERROR_CHECK(_wrapped->setPitch(pitch));
}

bool FmodSound::is_valid() const {
    bool isPlaying;
    FMOD_RESULT result = _wrapped->isPlaying(&isPlaying);
    return result != FMOD_ERR_INVALID_HANDLE;
}