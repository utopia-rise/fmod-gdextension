#include "fmod_sound.h"

using namespace godot;

void FmodSound::_bind_methods() {

}

void FmodSound::release_sound() {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->stop());
    channels.erase(instance);
}

void FmodSound::play_sound() {
    FIND_AND_CHECK(instanceId, channels)
    set_sound_paused(instanceId, false);
}

void FmodSound::set_sound_paused(, bool paused) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setPaused(paused));
}

void FmodSound::stop_sound() {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->stop());
}

bool FmodSound::is_sound_playing() {
    bool isPlaying = false;
    FIND_AND_CHECK(instanceId, channels, isPlaying)
    ERROR_CHECK(instance->isPlaying(&isPlaying));
    return isPlaying;
}

void FmodSound::set_sound_volume(, float volume) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setVolume(volume));
}

float FmodSound::get_sound_volume() {
    float volume = 0.f;
    FIND_AND_CHECK(instanceId, channels, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

float FmodSound::get_sound_pitch() {
    float pitch = 0.f;
    FIND_AND_CHECK(instanceId, channels, pitch)
    ERROR_CHECK(instance->getPitch(&pitch));
    return pitch;
}

void FmodSound::set_sound_pitch(, float pitch) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setPitch(pitch));
}

bool FmodSound::check_sound_instance() {
    FIND_AND_CHECK(instanceId, channels, false)
    return _is_channel_valid(instance);
}