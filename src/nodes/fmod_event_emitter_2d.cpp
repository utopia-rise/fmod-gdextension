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
    ClassDB::bind_method(D_METHOD("_notification", "p_what"), &FmodEventEmitter2D::_notification);

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

    if (internal_emitter._preload_event) {
        Ref<FmodEventDescription> desc = FmodServer::get_singleton()->get_event(internal_emitter._event_name);
        desc->load_sample_data();
    }

    internal_emitter._event = FmodServer::get_singleton()->create_event_instance(internal_emitter._event_name);

    for (int i = 0; i < internal_emitter._params.keys().size(); ++i) {
        auto key = internal_emitter._params.keys()[i];
        internal_emitter._event->set_parameter_by_name(key, internal_emitter._params[key]);
    }

    internal_emitter._event->set_2d_attributes(get_transform());
    if (internal_emitter._autoplay) { play(); }
}

void FmodEventEmitter2D::_process(double delta) {
    if (!internal_emitter._event.is_valid() && !internal_emitter._is_one_shot && internal_emitter._autoplay) {
        internal_emitter._event = FmodServer::get_singleton()->create_event_instance(internal_emitter._event_name);
    }

    if (internal_emitter._attached && internal_emitter._event.is_valid()) { internal_emitter._event->set_2d_attributes(get_transform()); }
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
    if (!internal_emitter._event.is_valid()) { return; }

    if (internal_emitter._allow_fadeout) {
        internal_emitter._event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    } else {
        internal_emitter._event->stop(FMOD_STUDIO_STOP_IMMEDIATE);
    }
}

void FmodEventEmitter2D::set_param(const String& key, const float value) {
    internal_emitter.set_param(key, value);
}

bool FmodEventEmitter2D::is_paused() {
    return internal_emitter.is_paused();
}

void FmodEventEmitter2D::play() {
    internal_emitter.play();
}

void FmodEventEmitter2D::pause() {
    internal_emitter.pause();
}

void FmodEventEmitter2D::set_event_name(const String& name) {
    internal_emitter.set_event_name(name);
}

String FmodEventEmitter2D::get_event_name() { return internal_emitter._event_name; }

void FmodEventEmitter2D::set_attached(const bool attached) { internal_emitter._attached = attached; }

bool FmodEventEmitter2D::is_attached() const { return internal_emitter._attached; }

void FmodEventEmitter2D::set_autoplay(const bool autoplay) { internal_emitter._autoplay = autoplay; }

bool FmodEventEmitter2D::is_autoplay() const { return internal_emitter._autoplay; }

void FmodEventEmitter2D::set_looped(const bool looped) { internal_emitter._is_one_shot = !looped; }

bool FmodEventEmitter2D::is_looped() const { return !internal_emitter._is_one_shot; }

void FmodEventEmitter2D::set_allow_fadeout(const bool allow_fadeout) { internal_emitter._allow_fadeout = allow_fadeout; }

bool FmodEventEmitter2D::is_allow_fadeout() const { return internal_emitter._allow_fadeout; }

void FmodEventEmitter2D::set_preload_event(const bool preload_event) { internal_emitter._preload_event = preload_event; }

bool FmodEventEmitter2D::is_preload_event() const { return internal_emitter._preload_event; }
