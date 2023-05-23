#include <nodes/fmod_event_emitter_2d.h>

#include <classes/engine.hpp>

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
    ClassDB::bind_method(D_METHOD("set_autoplay", "_autoplay"), &FmodEventEmitter2D::set_autoplay);
    ClassDB::bind_method(D_METHOD("is_autoplay"), &FmodEventEmitter2D::is_autoplay);
    ClassDB::bind_method(D_METHOD("set_looped", "_is_one_shot"), &FmodEventEmitter2D::set_looped);
    ClassDB::bind_method(D_METHOD("is_looped"), &FmodEventEmitter2D::is_looped);
    ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &FmodEventEmitter2D::set_allow_fadeout);
    ClassDB::bind_method(D_METHOD("is_allow_fadeout"), &FmodEventEmitter2D::is_allow_fadeout);
    ClassDB::bind_method(D_METHOD("set_preload_event", "preload_event"), &FmodEventEmitter2D::set_preload_event);
    ClassDB::bind_method(D_METHOD("is_preload_event"), &FmodEventEmitter2D::is_preload_event);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_name",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_event_name", "get_event_name");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "attached",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_attached", "is_attached");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_autoplay", "is_autoplay");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "looped",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_looped", "is_looped");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_allow_fadeout", "is_allow_fadeout");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_event",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_preload_event", "is_preload_event");

    ADD_SIGNAL(MethodInfo("timeline_beat", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("timeline_marker", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_played", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_stopped", PropertyInfo(Variant::DICTIONARY, "params")));
}

void FmodEventEmitter2D::_ready() {
    // ensure we only run FMOD when the game is running!
    if (Engine::get_singleton()->is_editor_hint()) { return; }

    if (_preload_event) {
        Ref<FmodEventDescription> desc = FmodServer::get_singleton()->get_event(_event_name);
        desc->load_sample_data();
    }

    _event = FmodServer::get_singleton()->create_event_instance(_event_name);

    for (int i = 0; i < _params.keys().size(); i++) {
        auto key = _params.keys()[i];
        _event->set_parameter_by_name(key, _params[key]);
    }

    _event->set_2d_attributes(get_transform());
    if (_autoplay) { play(); }
}

void FmodEventEmitter2D::_process(double delta) {
    if (!_event.is_valid() && !_is_one_shot && _autoplay) {
        _event = FmodServer::get_singleton()->create_event_instance(_event_name);
    }

    if (_attached && _event.is_valid()) { _event->set_2d_attributes(get_transform()); }
}

void FmodEventEmitter2D::_notification(int p_what) {
    // ensure we only run FMOD when the game is running!
    if (Engine::get_singleton()->is_editor_hint()) { return; }
    if (p_what == NOTIFICATION_PAUSED) {
        pause();
    } else if (p_what == NOTIFICATION_UNPAUSED) {
        if (is_paused()) { play(); }
    }
}

void FmodEventEmitter2D::_exit_tree() {
    if (!_event.is_valid()) { return; }

    if (_allow_fadeout) {
        _event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    } else {
        _event->stop(FMOD_STUDIO_STOP_IMMEDIATE);
    }
}
