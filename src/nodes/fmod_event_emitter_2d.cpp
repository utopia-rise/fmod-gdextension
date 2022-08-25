//
// Created by bitbrain on 2022-08-14.
//

#include "fmod_event_emitter_2d.h"
#include "../godot_fmod.h"

using namespace godot;

FmodEventEmitter2D::FmodEventEmitter2D() {
    
}

FmodEventEmitter2D::~FmodEventEmitter2D() {

}

void FmodEventEmitter2D::_init() {

}

void FmodEventEmitter2D::_ready() {
    for (int i = 0; i < params.keys().size(); i++) {
        auto key = params.keys()[i];
        _set_param_internally(key, params[key]);
    }
    if (preload_event) {
        Fmod::get_singleton()->desc_load_sample_data(event_name);
    }
    if (autoplay) {
        play();
    }
}

void FmodEventEmitter2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_param", "key", "value"), &FmodEventEmitter2D::set_param);
    ClassDB::bind_method(D_METHOD("is_paused"), &FmodEventEmitter2D::is_paused);
    ClassDB::bind_method(D_METHOD("play"), &FmodEventEmitter2D::play);
    ClassDB::bind_method(D_METHOD("pause"), &FmodEventEmitter2D::pause);
    ClassDB::bind_method(D_METHOD("set_event_name", "event_name"), &FmodEventEmitter2D::set_event_name);
    ClassDB::bind_method(D_METHOD("get_event_name"), &FmodEventEmitter2D::get_event_name);
    ClassDB::bind_method(D_METHOD("set_attached", "attached"), &FmodEventEmitter2D::set_attached);
    ClassDB::bind_method(D_METHOD("is_attached"), &FmodEventEmitter2D::is_attached);
    ClassDB::bind_method(D_METHOD("set_autoplay", "autoplay"), &FmodEventEmitter2D::set_autoplay);
    ClassDB::bind_method(D_METHOD("is_autoplay"), &FmodEventEmitter2D::is_autoplay);
    ClassDB::bind_method(D_METHOD("set_looped", "looped"), &FmodEventEmitter2D::set_looped);
    ClassDB::bind_method(D_METHOD("is_looped"), &FmodEventEmitter2D::is_looped);
    ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &FmodEventEmitter2D::set_allow_fadeout);
    ClassDB::bind_method(D_METHOD("is_allow_fadeout"), &FmodEventEmitter2D::is_allow_fadeout);
    ClassDB::bind_method(D_METHOD("set_preload_event", "preload_event"), &FmodEventEmitter2D::set_preload_event);
    ClassDB::bind_method(D_METHOD("is_preload_event"), &FmodEventEmitter2D::is_preload_event);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_name"), "set_event_name", "get_event_name");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "attached"), "set_attached", "is_attached");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay"), "set_autoplay", "is_autoplay");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "looped"), "set_looped", "is_looped");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout"), "set_allow_fadeout", "is_allow_fadeout");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_event"), "set_preload_event", "is_preload_event");
}

void FmodEventEmitter2D::_notification(int p_what) {
    switch (p_what) {
        case 14: { // PAUSED
            pause();
        } break;
        case 15: { // UNPAUSED
            play();
        } break;
    }
}

void FmodEventEmitter2D::set_param(const String &key, const float value) {
    params[key] = value;
    _set_param_internally(key, value);
}

bool FmodEventEmitter2D::is_paused() {
    if (event_id == -1) {
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
    if (event_id != -1) {
        Fmod::get_singleton()->set_event_paused(event_id, true);
    }
}

void FmodEventEmitter2D::set_event_name(const String &name) {
    if (name.begins_with("event:/")) {
        event_name = name;
    } else {
        event_name = "event:/" + name;
    }
}

String FmodEventEmitter2D::get_event_name() {
    return event_name;
}

void FmodEventEmitter2D::set_attached(const bool attached) {
    this->attached = attached;
}

bool FmodEventEmitter2D::is_attached() {
    return attached;
}

void FmodEventEmitter2D::set_autoplay(const bool autoplay) {
    this->autoplay = autoplay;
}

bool FmodEventEmitter2D::is_autoplay() {
    return autoplay;
}

void FmodEventEmitter2D::set_looped(const bool looped) {
    this->looped = looped;
}

bool FmodEventEmitter2D::is_looped() {
    return looped;
}

void FmodEventEmitter2D::set_allow_fadeout(const bool allow_fadeout) {
    this->allow_fadeout = allow_fadeout;
}

bool FmodEventEmitter2D::is_allow_fadeout() {
    return allow_fadeout;
}

void FmodEventEmitter2D::set_preload_event(const bool preload_event) {
    this->preload_event = preload_event;
}

bool FmodEventEmitter2D::is_preload_event() {
    return preload_event;
}

void FmodEventEmitter2D::_unpause() {
    if (event_id != -1) {
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
    if (event_id != -1) {
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

void FmodEventEmitter2D::_set_param_internally(const String &key, const float value) {
    if (event_id != -1) {
        Fmod::get_singleton()->set_event_parameter_by_name(event_id, key, value);
    }
}