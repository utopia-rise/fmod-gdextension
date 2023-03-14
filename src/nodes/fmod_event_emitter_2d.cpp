#include <classes/engine.hpp>
#include <fmod_server.h>
#include <nodes/fmod_event_emitter_2d.h>

using namespace godot;

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

    ADD_SIGNAL(MethodInfo("timeline_beat", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("timeline_marker", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_played", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_stopped", PropertyInfo(Variant::DICTIONARY, "params")));
}

FmodEventEmitter2D::FmodEventEmitter2D() {
    _init();
}

FmodEventEmitter2D::~FmodEventEmitter2D() {}

void FmodEventEmitter2D::_init() {}

void FmodEventEmitter2D::_ready() {
    // ensure we only run FMOD when the game is running!
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    for (int i = 0; i < params.keys().size(); i++) {
        auto key = params.keys()[i];
        _set_param_internally(key, params[key]);
    }
    if (preload_event) {
        FmodServer::get_singleton()->desc(event_name);
        FmodServer::get_singleton()->desc_load_sample_data(event_name);
    }
    if (autoplay) {
        play();
    }
}

void FmodEventEmitter2D::_notification(int p_what) {
    // ensure we only run FMOD when the game is running!
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (p_what == NOTIFICATION_PAUSED) {
        pause();
    } else if (p_what == NOTIFICATION_UNPAUSED) {
        if (is_paused()) {
            play();
        }
    }
}

void FmodEventEmitter2D::_exit_tree() {
    if (event != nullptr) {
        if (attached) {
            FmodServer::get_singleton()->detach_instance_from_node(event_id);
        }
        if (allow_fadeout) {
            event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        } else {
            event->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        }
    }
}
