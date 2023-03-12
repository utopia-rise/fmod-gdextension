#include "fmod_event.h"
#include "godot_fmod.h"
#include "helpers/common.h"

using namespace godot;

void FmodEvent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_event_parameter_by_name",  "parameterName"), &FmodEvent::get_event_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_event_parameter_by_name",  "parameterName", "value"), &FmodEvent::set_event_parameter_by_name);
    ClassDB::bind_method(D_METHOD("get_event_parameter_by_id",  "idPair"), &FmodEvent::get_event_parameter_by_id);
    ClassDB::bind_method(D_METHOD("set_event_parameter_by_id",  "idPair", "value"), &FmodEvent::set_event_parameter_by_id);
    ClassDB::bind_method(D_METHOD("release_event"), &FmodEvent::release_event);
    ClassDB::bind_method(D_METHOD("start_event"), &FmodEvent::start_event);
    ClassDB::bind_method(D_METHOD("stop_event",  "stopMode"), &FmodEvent::stop_event);
    ClassDB::bind_method(D_METHOD("event_key_off"), &FmodEvent::event_key_off);
    ClassDB::bind_method(D_METHOD("get_event_playback_state"), &FmodEvent::get_event_playback_state);
    ClassDB::bind_method(D_METHOD("get_event_paused"), &FmodEvent::get_event_paused);
    ClassDB::bind_method(D_METHOD("set_event_paused",  "paused"), &FmodEvent::set_event_paused);
    ClassDB::bind_method(D_METHOD("get_event_pitch"), &FmodEvent::get_event_pitch);
    ClassDB::bind_method(D_METHOD("set_event_pitch",  "pitch"), &FmodEvent::set_event_pitch);
    ClassDB::bind_method(D_METHOD("get_event_volume"), &FmodEvent::get_event_volume);
    ClassDB::bind_method(D_METHOD("set_event_volume",  "volume"), &FmodEvent::set_event_volume);
    ClassDB::bind_method(D_METHOD("get_event_timeline_position"), &FmodEvent::get_event_timeline_position);
    ClassDB::bind_method(D_METHOD("set_event_timeline_position",  "position"), &FmodEvent::set_event_timeline_position);
    ClassDB::bind_method(D_METHOD("get_event_reverb_level",  "index"), &FmodEvent::get_event_reverb_level);
    ClassDB::bind_method(D_METHOD("set_event_reverb_level",  "index", "level"), &FmodEvent::set_event_reverb_level);
    ClassDB::bind_method(D_METHOD("is_event_virtual"), &FmodEvent::is_event_virtual);
    ClassDB::bind_method(D_METHOD("set_event_listener_mask",  "mask"), &FmodEvent::set_event_listener_mask);
    ClassDB::bind_method(D_METHOD("get_event_listener_mask"), &FmodEvent::get_event_listener_mask);
    ClassDB::bind_method(D_METHOD("set_event_2d_attributes",  "position"), &FmodEvent::set_event_2d_attributes);
    ClassDB::bind_method(D_METHOD("get_event_2d_attributes"), &FmodEvent::get_event_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_event_3d_attributes",  "transform"), &FmodEvent::set_event_3d_attributes);
    ClassDB::bind_method(D_METHOD("get_event_3d_attributes"), &FmodEvent::get_event_3d_attributes);
    ClassDB::bind_method(D_METHOD("attach_instance_to_node", "instanceId", "gameObj"), &FmodEvent::attach_instance_to_node);
    ClassDB::bind_method(D_METHOD("detach_instance_from_node", "instanceId"), &FmodEvent::detach_instance_from_node);
    ClassDB::bind_method(D_METHOD("get_object_attached_to_instance", "instanceId"), &FmodEvent::get_object_attached_to_instance);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused"), "set_event_paused", "get_event_paused");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch"), "set_event_pitch", "get_event_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_event_volume", "get_event_volume");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "position"), "set_event_timeline_position", "get_event_timeline_position");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "listener_mask"), "set_event_listener_mask", "get_event_listener_mask");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform_2d"), "set_event_2d_attributes", "get_event_2d_attributes");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "transform_3d"), "set_event_3d_attributes", "get_event_3d_attributes");
}

float FmodEvent::get_event_parameter_by_name(const String& parameterName) const {
    float p = -1;
    ERROR_CHECK(event->getParameterByName(parameterName.utf8().get_data(), &p));
    return p;
}

void FmodEvent::set_event_parameter_by_name(const String& parameterName, float value) const {
    ERROR_CHECK(event->setParameterByName(parameterName.utf8().get_data(), value));
}

float FmodEvent::get_event_parameter_by_id(const Array& idPair) const {
    float value = -1.0f;
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(event->getParameterByID(id, &value));
    return value;
}

void FmodEvent::set_event_parameter_by_id(const Array& idPair, float value) const {
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(event->setParameterByID(id, value));
}



void FmodEvent::release_event() const {
    Fmod::get_singleton()->_release_one_event(event);
}

void FmodEvent::start_event() const {
    ERROR_CHECK(event->start());
}

void FmodEvent::stop_event(int stopMode) const {
    ERROR_CHECK(event->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void FmodEvent::event_key_off() const {
    ERROR_CHECK(event->keyOff());
}

int FmodEvent::get_event_playback_state() const {
    int s = -1;
    ERROR_CHECK(event->getPlaybackState((FMOD_STUDIO_PLAYBACK_STATE*) &s));
    return s;
}

bool FmodEvent::get_event_paused() const {
    bool paused = false;
    ERROR_CHECK(event->getPaused(&paused));
    return paused;
}

void FmodEvent::set_event_paused(bool paused) const {
    ERROR_CHECK(event->setPaused(paused));
}

float FmodEvent::get_event_pitch() const {
    float pitch = 0.0f;
    ERROR_CHECK(event->getPitch(&pitch));
    return pitch;
}

void FmodEvent::set_event_pitch(float pitch) const {
    ERROR_CHECK(event->setPitch(pitch));
}

float FmodEvent::get_event_volume() {
    float volume = 0.0f;
    ERROR_CHECK(event->getVolume(&volume));
    return volume;
}

void FmodEvent::set_event_volume(float volume) const {
    ERROR_CHECK(event->setVolume(volume));
}

int FmodEvent::get_event_timeline_position() const {
    int tp = 0;
    ERROR_CHECK(event->getTimelinePosition(&tp));
    return tp;
}

void FmodEvent::set_event_timeline_position(int position) const {
    ERROR_CHECK(event->setTimelinePosition(position));
}

float FmodEvent::get_event_reverb_level(int index) const {
    float rvl = 0.0f;
    ERROR_CHECK(event->getReverbLevel(index, &rvl));
    return rvl;
}

void FmodEvent::set_event_reverb_level(int index, float level) const {
    ERROR_CHECK(event->setReverbLevel(index, level));
}

bool FmodEvent::is_event_virtual() const {
    bool v = false;
    ERROR_CHECK(event->isVirtual(&v));
    return v;
}

void FmodEvent::set_event_listener_mask(unsigned int mask) const {
    ERROR_CHECK(event->setListenerMask(mask));
}

uint32_t FmodEvent::get_event_listener_mask() const {
    uint32_t mask = 0;
    ERROR_CHECK(event->getListenerMask(&mask));
    return mask;
}

void FmodEvent::set_event_2d_attributes(Transform2D position) const {
    auto attr = Fmod::get_singleton()->get_3d_attributes_from_transform_2d(position);
    ERROR_CHECK(event->set3DAttributes(&attr));
}

Dictionary FmodEvent::get_event_2d_attributes() const {
    Dictionary _2Dattr;
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(event->get3DAttributes(&attr));
    _2Dattr = Fmod::get_singleton()->get_transform_2d_info_from_3d_attribut(attr);
    return _2Dattr;
}

void FmodEvent::set_event_3d_attributes(const Transform3D& transform) const {
    auto attr = Fmod::get_singleton()->get_3d_attributes_from_transform(transform);
    ERROR_CHECK(event->set3DAttributes(&attr));
}

Dictionary FmodEvent::get_event_3d_attributes() const {
    Dictionary _3Dattr;
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(event->get3DAttributes(&attr));
    _3Dattr = Fmod::get_singleton()->get_transform_info_from_3d_attribut(attr);
    return _3Dattr;
}

void FmodEvent::attach_instance_to_node(Object* gameObj) const {
    if (!is_fmod_valid(gameObj)) {
        GODOT_LOG(1, "Trying to attach event instance to null game object or object is not Node3D or CanvasItem")
        return;
    }
    Fmod::_get_event_info(event)->gameObj = gameObj;
}

void FmodEvent::detach_instance_from_node() const{
    Fmod::_get_event_info(event)->gameObj = nullptr;
}

Object* FmodEvent::get_object_attached_to_instance() const{
    Object* node = nullptr;
    EventInfo* eventInfo = Fmod::_get_event_info(event);
    if (eventInfo) {
        node = eventInfo->gameObj;
        if (!node) {
            GODOT_LOG(1, "There is no node attached to event instance.")
        }
    }
    return node;
}