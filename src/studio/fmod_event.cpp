#include "fmod.h"
#include "fmod_server.h"
#include "helpers/common.h"
#include "helpers/maths.h"
#include "fmod_event.h"

using namespace godot;

void FmodEvent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_parameter_by_name", "parameter_name"), &FmodEvent::get_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_parameter_by_name", "parameter_name", "value"), &FmodEvent::set_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_parameter_by_name_with_label", "parameter_name", "label", "ignoreseekspeed"), &FmodEvent::set_parameter_by_name_with_label);
    ClassDB::bind_method(D_METHOD("get_parameter_by_id", "parameter_id"), &FmodEvent::get_parameter_by_id);
    ClassDB::bind_method(D_METHOD("set_parameter_by_id", "parameter_id", "value"), &FmodEvent::set_parameter_by_id);
    ClassDB::bind_method(D_METHOD("set_parameter_by_id_with_label", "parameter_id", "label", "ignoreseekspeed"), &FmodEvent::set_parameter_by_id_with_label);
    ClassDB::bind_method(D_METHOD("start"), &FmodEvent::start);
    ClassDB::bind_method(D_METHOD("stop", "stopMode"), &FmodEvent::stop);
    ClassDB::bind_method(D_METHOD("event_key_off"), &FmodEvent::event_key_off);
    ClassDB::bind_method(D_METHOD("get_playback_state"), &FmodEvent::get_playback_state);
    ClassDB::bind_method(D_METHOD("get_paused"), &FmodEvent::get_paused);
    ClassDB::bind_method(D_METHOD("set_paused", "paused"), &FmodEvent::set_paused);
    ClassDB::bind_method(D_METHOD("get_pitch"), &FmodEvent::get_pitch);
    ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &FmodEvent::set_pitch);
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodEvent::get_volume);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &FmodEvent::set_volume);
    ClassDB::bind_method(D_METHOD("get_timeline_position"), &FmodEvent::get_timeline_position);
    ClassDB::bind_method(D_METHOD("set_timeline_position", "position"), &FmodEvent::set_timeline_position);
    ClassDB::bind_method(D_METHOD("get_reverb_level", "index"), &FmodEvent::get_reverb_level);
    ClassDB::bind_method(D_METHOD("set_reverb_level", "index", "level"), &FmodEvent::set_reverb_level);
    ClassDB::bind_method(D_METHOD("is_virtual"), &FmodEvent::is_virtual);
    ClassDB::bind_method(D_METHOD("set_listener_mask", "mask"), &FmodEvent::set_listener_mask);
    ClassDB::bind_method(D_METHOD("get_listener_mask"), &FmodEvent::get_listener_mask);
    ClassDB::bind_method(D_METHOD("set_2d_attributes", "position"), &FmodEvent::set_2d_attributes);
    ClassDB::bind_method(D_METHOD("get_2d_attributes"), &FmodEvent::get_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_3d_attributes", "transform"), &FmodEvent::set_3d_attributes);
    ClassDB::bind_method(D_METHOD("get_3d_attributes"), &FmodEvent::get_3d_attributes);
    ClassDB::bind_method(D_METHOD("set_3d_attributes_with_velocity", "transform", "velocity"), &FmodEvent::set_3d_attributes_with_velocity);
    ClassDB::bind_method(D_METHOD("set_node_attributes", "transform"), &FmodEvent::set_node_attributes);
    ClassDB::bind_method(D_METHOD("set_callback", "callback", "callbackMask"), &FmodEvent::set_callback);
    ClassDB::bind_method(D_METHOD("set_programmer_callback", "p_programmers_callback_sound_key"), &FmodEvent::set_programmer_callback);
    ClassDB::bind_method(D_METHOD("get_programmer_callback_sound_key"), &FmodEvent::get_programmers_callback_sound_key);
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodEvent::is_valid);
    ClassDB::bind_method(D_METHOD("release"), &FmodEvent::release);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_paused", "get_paused");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_pitch", "get_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_volume", "get_volume");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "position",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_timeline_position", "get_timeline_position");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "listener_mask",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_listener_mask", "get_listener_mask");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform_2d",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_2d_attributes", "get_2d_attributes");
    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "transform_3d",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_node_attributes", "get_3d_attributes");

    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_PLAYING);
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_SUSTAINING);
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPED);
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STARTING);
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPING);
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_FORCEINT);
}

float FmodEvent::get_parameter_by_name(const String& parameter_name) const {
    float p = -1;
    ERROR_CHECK_WITH_REASON(_wrapped->getParameterByName(parameter_name.utf8().get_data(), &p),
                            vformat("Cannot get parameter %s", parameter_name));
    return p;
}

void FmodEvent::set_parameter_by_name(const String& parameter_name, float value) const {
    ERROR_CHECK_WITH_REASON(_wrapped->setParameterByName(parameter_name.utf8().get_data(), value),
                            vformat("Cannot set parameter %s to value %f", parameter_name, value));
}

void FmodEvent::set_parameter_by_name_with_label(const String& parameter_name, const String& label, bool ignoreseekspeed) const {
    ERROR_CHECK_WITH_REASON(_wrapped->setParameterByNameWithLabel(parameter_name.utf8().get_data(), label.utf8().get_data(), ignoreseekspeed),
                            vformat("Cannot set parameter %s value to %s", parameter_name, label));
}

float FmodEvent::get_parameter_by_id(uint64_t long_id) const {
    return get_parameter_by_fmod_id(ulong_to_fmod_parameter_id(long_id));
}

float FmodEvent::get_parameter_by_fmod_id(const FMOD_STUDIO_PARAMETER_ID& parameter_id) const {
    float value = -1.0f;
    ERROR_CHECK_WITH_REASON(_wrapped->getParameterByID(parameter_id, &value),
                            vformat("Cannot get parameter with id: %d", fmod_parameter_id_to_ulong(parameter_id)));
    return value;
}

void FmodEvent::set_parameter_by_id(uint64_t long_id, float value) const {
    set_parameter_by_fmod_id(ulong_to_fmod_parameter_id(long_id), value);
}

void FmodEvent::set_parameter_by_fmod_id(const FMOD_STUDIO_PARAMETER_ID& parameter_id, float value) const {
    ERROR_CHECK_WITH_REASON(_wrapped->setParameterByID(parameter_id, value),
                            vformat("Cannot set parameter with id %d to value %f", fmod_parameter_id_to_ulong(parameter_id), value));
}

void FmodEvent::set_parameter_by_fmod_id_with_label(const FMOD_STUDIO_PARAMETER_ID& parameter_id, const String& label, bool ignoreseekspeed) const {
    ERROR_CHECK_WITH_REASON(_wrapped->setParameterByIDWithLabel(parameter_id, label.utf8().get_data(), ignoreseekspeed),
                            vformat("Cannot set parameter to id %d to value %s", fmod_parameter_id_to_ulong(parameter_id), label));
}

void FmodEvent::set_parameter_by_id_with_label(uint64_t parameter_id, const String& label, bool ignoreseekspeed) const {
    set_parameter_by_fmod_id_with_label(ulong_to_fmod_parameter_id(parameter_id), label, ignoreseekspeed);
}

void FmodEvent::release() const {
    ERROR_CHECK(_wrapped->release());
}

void FmodEvent::start() const {
    ERROR_CHECK(_wrapped->start());
}

void FmodEvent::stop(int stopMode) const {
    ERROR_CHECK(_wrapped->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void FmodEvent::event_key_off() const {
    ERROR_CHECK(_wrapped->keyOff());
}

FMOD_STUDIO_PLAYBACK_STATE FmodEvent::get_playback_state() const {
    FMOD_STUDIO_PLAYBACK_STATE playback_state;
    ERROR_CHECK(_wrapped->getPlaybackState(&playback_state));
    return playback_state;
}

bool FmodEvent::get_paused() const {
    bool paused = false;
    ERROR_CHECK(_wrapped->getPaused(&paused));
    return paused;
}

void FmodEvent::set_paused(bool paused) const {
    ERROR_CHECK(_wrapped->setPaused(paused));
}

float FmodEvent::get_pitch() const {
    float pitch = 0.0f;
    ERROR_CHECK(_wrapped->getPitch(&pitch));
    return pitch;
}

void FmodEvent::set_pitch(float pitch) const {
    ERROR_CHECK(_wrapped->setPitch(pitch));
}

float FmodEvent::get_volume() {
    float volume = 0.0f;
    ERROR_CHECK(_wrapped->getVolume(&volume));
    return volume;
}

void FmodEvent::set_volume(float volume) const {
    ERROR_CHECK(_wrapped->setVolume(volume));
}

int FmodEvent::get_timeline_position() const {
    int tp = 0;
    ERROR_CHECK(_wrapped->getTimelinePosition(&tp));
    return tp;
}

void FmodEvent::set_timeline_position(int position) const {
    ERROR_CHECK(_wrapped->setTimelinePosition(position));
}

float FmodEvent::get_reverb_level(int index) const {
    float rvl = 0.0f;
    ERROR_CHECK(_wrapped->getReverbLevel(index, &rvl));
    return rvl;
}

void FmodEvent::set_reverb_level(int index, float level) const {
    ERROR_CHECK(_wrapped->setReverbLevel(index, level));
}

bool FmodEvent::is_virtual() const {
    bool v = false;
    ERROR_CHECK(_wrapped->isVirtual(&v));
    return v;
}

void FmodEvent::set_listener_mask(unsigned int mask) const {
    ERROR_CHECK(_wrapped->setListenerMask(mask));
}

uint32_t FmodEvent::get_listener_mask() const {
    uint32_t mask = 0;
    ERROR_CHECK(_wrapped->getListenerMask(&mask));
    return mask;
}

void FmodEvent::set_2d_attributes(const Transform2D& position) const {
    FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform2d(position, distanceScale);
    ERROR_CHECK(_wrapped->set3DAttributes(&attr));
}

Transform2D FmodEvent::get_2d_attributes() const {
    Transform2D _2Dattr;
    FMOD_3D_ATTRIBUTES attr;
    ERROR_CHECK(_wrapped->get3DAttributes(&attr));
    _2Dattr = get_transform2d_from_3d_attributes(attr, distanceScale);
    return _2Dattr;
}

void FmodEvent::set_3d_attributes(const Transform3D& transform) const {
    FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform3d(transform, distanceScale);
    ERROR_CHECK(_wrapped->set3DAttributes(&attr));
}

Transform3D FmodEvent::get_3d_attributes() const {
    Transform3D _3Dattr;
    FMOD_3D_ATTRIBUTES attr;
    ERROR_CHECK(_wrapped->get3DAttributes(&attr));
    _3Dattr = get_transform3d_from_3d_attributes(attr, distanceScale);
    return _3Dattr;
}

void FmodEvent::set_3d_attributes_with_velocity(const Transform3D& transform, const Vector3& velocity) const {
    FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform3d(transform, velocity, distanceScale);
    ERROR_CHECK(_wrapped->set3DAttributes(&attr));
}

void FmodEvent::set_node_attributes(Node* node) const {
    if (node->is_inside_tree()) {
        if (auto* ci {Node::cast_to<CanvasItem>(node)}) {
            FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform2d(ci->get_global_transform(), distanceScale);
            ERROR_CHECK(_wrapped->set3DAttributes(&attr));
            return;
        }
        if (auto* s {Node::cast_to<Node3D>(node)}) {
            FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform3d(s->get_global_transform(), distanceScale);
            ERROR_CHECK(_wrapped->set3DAttributes(&attr));
            return;
        }
    }
    GODOT_LOG_ERROR("Invalid Object. A Godot object bound to FMOD has to be either a Node3D or CanvasItem.")
}

void FmodEvent::set_callback(const Callable& callback, uint32_t p_callback_mask) {
    eventCallback = callback;
    callback_mask = p_callback_mask;
    ERROR_CHECK(_wrapped->setCallback(Callbacks::event_callback, p_callback_mask));
}

void FmodEvent::set_programmer_callback(const String& p_programmers_callback_sound_key) {
    programmers_callback_sound_key = p_programmers_callback_sound_key;
    ERROR_CHECK(_wrapped->setCallback(Callbacks::event_callback, callback_mask | FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND | FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND));
}

const Callable& FmodEvent::get_callback() const {
    return eventCallback;
}

const String& FmodEvent::get_programmers_callback_sound_key() const {
    return programmers_callback_sound_key;
}

void FmodEvent::set_distance_scale(float scale){
    distanceScale = scale;
}

FmodEvent::~FmodEvent() {
    if (is_valid()) {
        _wrapped->setUserData(nullptr);
    }
}
