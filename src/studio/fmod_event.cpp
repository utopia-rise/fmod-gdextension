#include "fmod.h"
#include "fmod_server.h"
#include "helpers/common.h"

using namespace godot;

void FmodEvent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_parameter_by_name",  "parameterName"), &FmodEvent::get_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_parameter_by_name",  "parameterName", "value"), &FmodEvent::set_parameter_by_name);
    ClassDB::bind_method(D_METHOD("get_parameter_by_id",  "idPair"), &FmodEvent::get_parameter_by_id);
    ClassDB::bind_method(D_METHOD("set_parameter_by_id",  "idPair", "value"), &FmodEvent::set_parameter_by_id);
    ClassDB::bind_method(D_METHOD("release"), &FmodEvent::release);
    ClassDB::bind_method(D_METHOD("start"), &FmodEvent::start);
    ClassDB::bind_method(D_METHOD("stop",  "stopMode"), &FmodEvent::stop);
    ClassDB::bind_method(D_METHOD("event_key_off"), &FmodEvent::event_key_off);
    ClassDB::bind_method(D_METHOD("get_playback_state"), &FmodEvent::get_playback_state);
    ClassDB::bind_method(D_METHOD("get_paused"), &FmodEvent::get_paused);
    ClassDB::bind_method(D_METHOD("set_paused",  "paused"), &FmodEvent::set_paused);
    ClassDB::bind_method(D_METHOD("get_pitch"), &FmodEvent::get_pitch);
    ClassDB::bind_method(D_METHOD("set_pitch",  "pitch"), &FmodEvent::set_pitch);
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodEvent::get_volume);
    ClassDB::bind_method(D_METHOD("set_volume",  "volume"), &FmodEvent::set_volume);
    ClassDB::bind_method(D_METHOD("get_timeline_position"), &FmodEvent::get_timeline_position);
    ClassDB::bind_method(D_METHOD("set_timeline_position",  "position"), &FmodEvent::set_timeline_position);
    ClassDB::bind_method(D_METHOD("get_reverb_level",  "index"), &FmodEvent::get_reverb_level);
    ClassDB::bind_method(D_METHOD("set_reverb_level",  "index", "level"), &FmodEvent::set_reverb_level);
    ClassDB::bind_method(D_METHOD("is_virtual"), &FmodEvent::is_virtual);
    ClassDB::bind_method(D_METHOD("set_listener_mask",  "mask"), &FmodEvent::set_listener_mask);
    ClassDB::bind_method(D_METHOD("get_listener_mask"), &FmodEvent::get_listener_mask);
    ClassDB::bind_method(D_METHOD("set_2d_attributes",  "position"), &FmodEvent::set_2d_attributes);
    ClassDB::bind_method(D_METHOD("get_2d_attributes"), &FmodEvent::get_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_3d_attributes",  "transform"), &FmodEvent::set_3d_attributes);
    ClassDB::bind_method(D_METHOD("get_3d_attributes"), &FmodEvent::get_3d_attributes);
    ClassDB::bind_method(D_METHOD("set_callback", "instanceId", "callbackMask"), &FmodServer::set_callback);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused"), "set_paused", "get_paused");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch"), "set_pitch", "get_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "position"), "set_timeline_position", "get_timeline_position");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "listener_mask"), "set_listener_mask", "get_listener_mask");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform_2d"), "set_2d_attributes", "get_2d_attributes");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "transform_3d"), "set_3d_attributes", "get_3d_attributes");

}

float FmodEvent::get_parameter_by_name(const String& parameterName) const {
    float p = -1;
    ERROR_CHECK(instance->getParameterByName(parameterName.utf8().get_data(), &p));
    return p;
}

void FmodEvent::set_parameter_by_name(const String& parameterName, float value) const {
    ERROR_CHECK(instance->setParameterByName(parameterName.utf8().get_data(), value));
}

float FmodEvent::get_parameter_by_id(const Array& idPair) const {
    float value = -1.0f;
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->getParameterByID(id, &value));
    return value;
}

void FmodEvent::set_parameter_by_id(const Array& idPair, float value) const {
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->setParameterByID(id, value));
}



void FmodEvent::release() const {
    FmodServer::get_singleton()->_release_one(instance);
}

void FmodEvent::start() const {
    ERROR_CHECK(instance->start());
}

void FmodEvent::stop(int stopMode) const {
    ERROR_CHECK(instance->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void FmodEvent::event_key_off() const {
    ERROR_CHECK(instance->keyOff());
}

int FmodEvent::get_playback_state() const {
    int s = -1;
    ERROR_CHECK(instance->getPlaybackState((FMOD_STUDIO_PLAYBACK_STATE*) &s));
    return s;
}

bool FmodEvent::get_paused() const {
    bool paused = false;
    ERROR_CHECK(instance->getPaused(&paused));
    return paused;
}

void FmodEvent::set_paused(bool paused) const {
    ERROR_CHECK(instance->setPaused(paused));
}

float FmodEvent::get_pitch() const {
    float pitch = 0.0f;
    ERROR_CHECK(instance->getPitch(&pitch));
    return pitch;
}

void FmodEvent::set_pitch(float pitch) const {
    ERROR_CHECK(instance->setPitch(pitch));
}

float FmodEvent::get_volume() {
    float volume = 0.0f;
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void FmodEvent::set_volume(float volume) const {
    ERROR_CHECK(instance->setVolume(volume));
}

int FmodEvent::get_timeline_position() const {
    int tp = 0;
    ERROR_CHECK(instance->getTimelinePosition(&tp));
    return tp;
}

void FmodEvent::set_timeline_position(int position) const {
    ERROR_CHECK(instance->setTimelinePosition(position));
}

float FmodEvent::get_reverb_level(int index) const {
    float rvl = 0.0f;
    ERROR_CHECK(instance->getReverbLevel(index, &rvl));
    return rvl;
}

void FmodEvent::set_reverb_level(int index, float level) const {
    ERROR_CHECK(instance->setReverbLevel(index, level));
}

bool FmodEvent::is_virtual() const {
    bool v = false;
    ERROR_CHECK(instance->isVirtual(&v));
    return v;
}

void FmodEvent::set_listener_mask(unsigned int mask) const {
    ERROR_CHECK(instance->setListenerMask(mask));
}

uint32_t FmodEvent::get_listener_mask() const {
    uint32_t mask = 0;
    ERROR_CHECK(instance->getListenerMask(&mask));
    return mask;
}

void FmodEvent::set_2d_attributes(Transform2D position) const {
    auto attr = FmodServer::get_singleton()->get_3d_attributes_from_transform_2d(position);
    ERROR_CHECK(instance->set3DAttributes(&attr));
}

Dictionary FmodEvent::get_2d_attributes() const {
    Dictionary _2Dattr;
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(instance->get3DAttributes(&attr));
    _2Dattr = FmodServer::get_singleton()->get_transform_2d_info_from_3d_attributes(attr);
    return _2Dattr;
}

void FmodEvent::set_3d_attributes(const Transform3D& transform) const {
    auto attr = FmodServer::get_singleton()->get_3d_attributes_from_transform(transform);
    ERROR_CHECK(instance->set3DAttributes(&attr));
}

Dictionary FmodEvent::get_3d_attributes() const {
    Dictionary _3Dattr;
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(instance->get3DAttributes(&attr));
    _3Dattr = FmodServer::get_singleton()->get_transform_info_from_3d_attributes(attr);
    return _3Dattr;
}


void FmodEvent::set_callback(Callable& callback, int callbackMask) {
    eventCallback = callback;
    ERROR_CHECK(instance->setCallback(Callbacks::eventCallback, callbackMask));
}


Callable FmodEvent::get_callback() const{
    return eventCallback;
}