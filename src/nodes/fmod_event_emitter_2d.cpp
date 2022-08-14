//
// Created by bitbrain on 2022-08-14.
//

#include "fmod_event_emitter_2d.h"
#include "../godot_fmod.h"

using namespace godot;

const String EVENT_PREFIX = "event:/";
const int UNDEFINED = -1;

FmodEventEmitter2D::FmodEventEmitter2D() : event_name(EVENT_PREFIX) {

}

FmodEventEmitter2D::~FmodEventEmitter2D() {

}

void FmodEventEmitter2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_param", "key", "value"), &FmodEventEmitter2D::set_param);
    ClassDB::bind_method(D_METHOD("is_paused"), &FmodEventEmitter2D::is_paused);
    ClassDB::bind_method(D_METHOD("play"), &FmodEventEmitter2D::play);
    ClassDB::bind_method(D_METHOD("pause"), &FmodEventEmitter2D::pause);
}

void FmodEventEmitter2D::set_param(const String &key, const float &value) {
    params[key] = value;
    _set_param_internally(key, value);
}

bool FmodEventEmitter2D::is_paused() const {
    if (event_id == UNDEFINED) {
        return false;
    }
    return Fmod::get_singleton()->get_event_paused(event_id);
}

void FmodEventEmitter2D::play() {
    if (is_paused()) {
        _unpause();
    } else if (looped) {
        _play_looped();
    } else {
        _play_one_shot();
    }
}

void FmodEventEmitter2D::pause() {
    if (event_id != UNDEFINED) {
        Fmod::get_singleton()->set_event_paused(event_id, true);
    }
}

void FmodEventEmitter2D::_unpause() {
    if (event_id != UNDEFINED) {
        Fmod::get_singleton()->set_event_paused(event_id, false);
    }
}

void FmodEventEmitter2D::_play_one_shot() {
    if (!attached) {
        if (params.size() > 0) {
            Fmod::get_singleton()->play_one_shot_with_params(event_name, this, params);
        } else {
            Fmod::get_singleton()->play_one_shot(event_name, this);
        }
    } else {
        if (params.size() > 0) {
            Fmod::get_singleton()->play_one_shot_attached_with_params(event_name, this, params);
        } else {
            Fmod::get_singleton()->play_one_shot_attached(event_name, this);
        }
    }
}

void FmodEventEmitter2D::_play_looped() {
    if (event_id != UNDEFINED) {
        return;
    }
    event_id = Fmod::get_singleton()->create_event_instance(event_name);
    Fmod::get_singleton()->start_event(event_id);
    if (attached) {
        Fmod::get_singleton()->attach_instance_to_node(event_id, this);
    }
    for (int i = 0; i < params.keys().size(); i++) {
        auto key = params.keys()[i];
        Fmod::get_singleton()->set_event_parameter_by_name(event_id, key, params[key]);
    }
}

// FIXME: THIS SHOULD BE A SETGET ON EXPORT
void FmodEventEmitter2D::_set_event_name(const String &name) {
    if (name.begins_with(EVENT_PREFIX)) {
        event_name = name;
    } else {
        event_name = EVENT_PREFIX + name;
    }
}

void FmodEventEmitter2D::_set_param_internally(const String &key, const float &value) {
    if (event_id != UNDEFINED) {
        Fmod::get_singleton()->set_event_parameter_by_name(event_id, key, value);
    }
}