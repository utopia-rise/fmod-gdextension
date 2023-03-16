#include "fmod_bus.h"
#include "helpers/common.h"

using namespace godot;

void FmodBus::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_mute"), &FmodBus::get_mute);
    ClassDB::bind_method(D_METHOD("get_paused"), &FmodBus::get_paused);
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodBus::get_volume);
    ClassDB::bind_method(D_METHOD("set_mute", "mute"), &FmodBus::set_mute);
    ClassDB::bind_method(D_METHOD("set_paused", "paused"), &FmodBus::set_paused);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &FmodBus::set_volume);
    ClassDB::bind_method(D_METHOD("stop_all_events", "stopMode"), &FmodBus::stop_all_events);
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodBus::is_valid);
    ClassDB::bind_method(D_METHOD("release"), &FmodBus::release);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mute"), "set_mute", "get_mute");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused"), "set_paused", "get_paused");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
}

bool FmodBus::get_mute() const {
    bool mute = false;
    ERROR_CHECK(wrapped->getMute(&mute));
    return mute;
}

bool FmodBus::get_paused() const {
    bool paused = false;
    ERROR_CHECK(wrapped->getPaused(&paused));
    return paused;
}

float FmodBus::get_volume() const {
    float volume = 0.0f;
    ERROR_CHECK(wrapped->getVolume(&volume));
    return volume;
}

void FmodBus::set_mute(bool mute) const {
    ERROR_CHECK(wrapped->setMute(mute));
}

void FmodBus::set_paused(bool paused) const {
    ERROR_CHECK(wrapped->setPaused(paused));
}

void FmodBus::set_volume(float volume) const {
    ERROR_CHECK(wrapped->setVolume(volume));
}

void FmodBus::stop_all_events(int stopMode) {
    ERROR_CHECK(wrapped->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}