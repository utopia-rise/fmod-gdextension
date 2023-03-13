#include "fmod_event_emitter.h"
#include "fmod_server.h"

using namespace godot;

void FmodEventEmitter::set_param(const String& key, const float value) {
    params[key] = value;
    _set_param_internally(key, value);
}

bool FmodEventEmitter::is_paused() {
    if (event == nullptr) {
        return false;
    }
    return event->get_paused();
}

void FmodEventEmitter::play() {
    if (is_paused()) {
        _unpause();
    } else if (looped) {
        _play_looped();
    } else {
        _play_one_shot();
    }
}

void FmodEventEmitter::pause() {
    if (event != nullptr) {
        event->set_paused(true);
    }
}

void FmodEventEmitter::set_event_name(const String& name) {
    if (name.begins_with("event:/")) {
        event_name = name;
    } else {
        event_name = "event:/" + name;
    }
}

String FmodEventEmitter::get_event_name() {
    return event_name;
}

void FmodEventEmitter::set_attached(const bool attached) {
    this->attached = attached;
}

bool FmodEventEmitter::is_attached() const {
    return attached;
}

void FmodEventEmitter::set_autoplay(const bool autoplay) {
    this->autoplay = autoplay;
}

bool FmodEventEmitter::is_autoplay() {
    return autoplay;
}

void FmodEventEmitter::set_looped(const bool looped) {
    this->looped = looped;
}

bool FmodEventEmitter::is_looped() {
    return looped;
}

void FmodEventEmitter::set_allow_fadeout(const bool allow_fadeout) {
    this->allow_fadeout = allow_fadeout;
}

bool FmodEventEmitter::is_allow_fadeout() {
    return allow_fadeout;
}

void FmodEventEmitter::set_preload_event(const bool preload_event) {
    this->preload_event = preload_event;
}

bool FmodEventEmitter::is_preload_event() {
    return preload_event;
}

void FmodEventEmitter::_unpause() {
    if (event != nullptr) {
        event->set_paused(false);
    }
}

void FmodEventEmitter::_play_one_shot() {
    if (!attached) {
        if (params.size() > 0) {
            FmodServer::get_singleton()->play_one_shot_with_params(event_name, this, params);
        } else {
            FmodServer::get_singleton()->play_one_shot(event_name, this);
        }
    } else {
        if (params.size() > 0) {
            FmodServer::get_singleton()->play_one_shot_attached_with_params(event_name, this, params);
        } else {
            FmodServer::get_singleton()->play_one_shot_attached(event_name, this);
        }
    }
}

void FmodEventEmitter::_play_looped() {
    if (event == nullptr) {
        return;
    }
    event = FmodServer::get_singleton()->create_event_instance(event_name);
    event->start();
    if (attached) {
        FmodServer::get_singleton()->attach_instance_to_node(event_id, this);
    }
    for (int i = 0; i < params.keys().size(); i++) {
        auto key = params.keys()[i];
        event->set_parameter_by_name(key, params[key]);
    }
}

void FmodEventEmitter::_set_param_internally(const String& key, const float value) {
    if (event != nullptr) {
        event->set_parameter_by_name(key, params[key]);
    }
}