#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_H

#include "classes/object.hpp"

#include <fmod_server.h>

#include <classes/engine.hpp>

static constexpr const char* BEAT_SIGNAL_STRING = "timeline_beat";
static constexpr const char* MARKER_SIGNAL_STRING = "timeline_marker";
static constexpr const char* START_FAILED_SIGNAL_STRING = "start_failed";
static constexpr const char* STARTED_SIGNAL_STRING = "started";
static constexpr const char* RESTARTED_SIGNAL_STRING = "restarted";
static constexpr const char* STOPPED_SIGNAL_STRING = "stopped";

namespace godot {

    template<class Derived, class NodeType>
    class FmodEventEmitter : public NodeType {
        Ref<FmodEvent> _event;

        String _event_name;
        bool _attached = true;
        bool _autoplay = false;
        bool _is_one_shot = false;
        bool _allow_fadeout = true;
        bool _preload_event = true;
        float _volume = true;

        Dictionary _params;
        
    public:
        virtual void _ready() override;
        virtual void _process(double delta) override;
        void _notification(int p_what);
        virtual void _exit_tree() override;
        
        void play();
        void stop();
        void set_paused(bool p_is_paused);
        const Ref<FmodEvent>& get_event() const;
        bool is_paused();
        void set_event_name(const String& name);
        String get_event_name() const;
        void set_attached(const bool attached);
        bool is_attached() const;
        void set_autoplay(const bool autoplay);
        bool is_autoplay() const;
        void set_one_shot(const bool p_is_one_shot);
        bool is_one_shot() const;
        void set_allow_fadeout(const bool allow_fadeout);
        bool is_allow_fadeout() const;
        void set_preload_event(const bool preload_event);
        bool is_preload_event() const;
        void set_volume(const float volume);
        float get_volume() const;
        void set_parameter(const String& key, const float value);
        const Dictionary& get_parameters() const;
        void set_parameters(const Dictionary& p_params);

        static StringName& get_class_static();

    protected:
        void _emit_callbacks(const Dictionary& dict, const int type) const;
        static void _bind_methods();

    private:
        void set_space_attribute() const;
        void apply_parameters();
        void preload_event() const;
        void load_event();

    };

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_space_attribute() const {
        static_cast<const Derived*>(this)->set_space_attribute_impl();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_ready() {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif
        
        if (is_preload_event()) {
            preload_event();
        }

        if (!_is_one_shot) {
            load_event();
            _event->set_volume(_volume);
            apply_parameters();
            set_space_attribute();
        }

        if (_autoplay) {
            set_paused(false);
            play();
        }
    }
    
    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_process(double delta) {
#ifdef TOOLS_ENABLED
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        bool should_restart{ false };
        if (!_event.is_valid() && !_is_one_shot && _autoplay) {
            load_event();
            _event->set_volume(_volume);
            apply_parameters();
            should_restart = true;
        }

        if (!_is_one_shot && _attached && _event.is_valid()) {
            set_space_attribute();
        }

        if (should_restart) {
            set_paused(false);
            play();
        }
    }
    
    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_notification(int p_what) {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        if (p_what == Node::NOTIFICATION_PAUSED) {
            set_paused(true);
        } else if (p_what == Node::NOTIFICATION_UNPAUSED) {
            if (is_paused()) { set_paused(false); }
        }
    }
    
    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_exit_tree() {
#ifdef TOOLS_ENABLED
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        stop();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_parameter(const String& key, const float value) {
        _params[key] = value;
        if (!_event.is_valid()) { return; }
        _event->set_parameter_by_name(key, _params[key]);
    }

    template<class Derived, class NodeType>
    const Dictionary& FmodEventEmitter<Derived, NodeType>::get_parameters() const {
        return _params;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_parameters(const Dictionary& p_params) {
        _params = p_params;
        apply_parameters();
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_paused() {
        if (!_event.is_valid()) { return false; }
        return _event->get_paused();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::play() {
        if (!_event.is_valid()) { return; }

        if (!_is_one_shot) {
            if (_attached) { set_space_attribute(); }
            _event->start();
            return;
        }

        if (_attached) {
            if (!_params.is_empty()) {
                FmodServer::get_singleton()->play_one_shot_attached_with_params(
                  _event_name,
                  this,
                  _params
                );
                return;
            }

            FmodServer::get_singleton()->play_one_shot_attached(_event_name, this);
            return;
        }

        if (!_params.is_empty()) {
            FmodServer::get_singleton()->play_one_shot_with_params(
              _event_name,
              this,
              _params
            );
            return;
        }

        FmodServer::get_singleton()->play_one_shot(_event_name, this);
        return;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::stop() {
        if (!_event.is_valid()) { return; }
        if (_allow_fadeout && !_is_one_shot) {
            _event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        } else {
            _event->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        }
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_paused(bool p_is_paused) {
        if (!_event.is_valid()) { return; }
        _event->set_paused(p_is_paused);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::preload_event() const {
        Ref<FmodEventDescription> desc = FmodServer::get_singleton()->get_event(_event_name);
        desc->load_sample_data();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::load_event() {
        _event = FmodServer::get_singleton()->create_event_instance(_event_name);
        _event->set_callback(Callable(this, "_emit_callbacks"), FMOD_STUDIO_EVENT_CALLBACK_ALL);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::apply_parameters() {
        if (!_event.is_valid()) { return; }
        for (int i = 0; i < _params.keys().size(); ++i) {
            const String& key = _params.keys()[i];
            _event->set_parameter_by_name(key, _params[key]);
        }
    }

    template<class Derived, class NodeType>
    const Ref<FmodEvent>& FmodEventEmitter<Derived, NodeType>::get_event() const {
        return _event;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_event_name(const String& name) {
        _event_name = name;
    }

    template<class Derived, class NodeType>
    String FmodEventEmitter<Derived, NodeType>::get_event_name() const {
        return _event_name;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_attached(const bool attached) {
        _attached = attached;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_attached() const {
        return _attached;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_autoplay(const bool autoplay) {
        _autoplay = autoplay;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_autoplay() const {
        return _autoplay;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_one_shot(const bool p_is_one_shot) {
        _is_one_shot = p_is_one_shot;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_one_shot() const {
        return _is_one_shot;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_allow_fadeout(const bool allow_fadeout) {
        _allow_fadeout = allow_fadeout;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_allow_fadeout() const {
        return _allow_fadeout;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_preload_event(const bool preload_event) {
        _preload_event = preload_event;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_preload_event() const {
        return _preload_event;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_volume(const float volume) {
        _volume = volume;

        if (!_event.is_valid()) {
            return;
        }
        _event->set_volume(volume);
    }

    template<class Derived, class NodeType>
    float FmodEventEmitter<Derived, NodeType>::get_volume() const {
        return _volume;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_emit_callbacks(const Dictionary& dict, const int type) const {
        switch (type) {
            case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(BEAT_SIGNAL_STRING, dict);
                break;
            case FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(MARKER_SIGNAL_STRING, dict);
                break;
            case FMOD_STUDIO_EVENT_CALLBACK_START_FAILED:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(START_FAILED_SIGNAL_STRING);
                break;
            case FMOD_STUDIO_EVENT_CALLBACK_STARTED:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(STARTED_SIGNAL_STRING);
                break;
            case FMOD_STUDIO_EVENT_CALLBACK_RESTARTED:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(RESTARTED_SIGNAL_STRING);
                break;
            case FMOD_STUDIO_EVENT_CALLBACK_STOPPED:
                const_cast<Derived*>(static_cast<const Derived*>(this))->emit_signal(STOPPED_SIGNAL_STRING);
                break;
        }
    }

    template<class Derived, class NodeType>
    StringName& FmodEventEmitter<Derived, NodeType>::get_class_static() {
        return Derived::get_class_static();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_parameter", "key", "value"), &Derived::set_parameter);
        ClassDB::bind_method(D_METHOD("play"), &Derived::play);
        ClassDB::bind_method(D_METHOD("stop"), &Derived::stop);
        ClassDB::bind_method(D_METHOD("is_paused"), &Derived::is_paused);
        ClassDB::bind_method(D_METHOD("set_paused", "p_is_paused"), &Derived::set_paused);
        ClassDB::bind_method(D_METHOD("set_event_name", "event_name"), &Derived::set_event_name);
        ClassDB::bind_method(D_METHOD("get_event_name"), &Derived::get_event_name);
        ClassDB::bind_method(D_METHOD("set_attached", "attached"), &Derived::set_attached);
        ClassDB::bind_method(D_METHOD("is_attached"), &Derived::is_attached);
        ClassDB::bind_method(D_METHOD("set_autoplay", "_autoplay"), &Derived::set_autoplay);
        ClassDB::bind_method(D_METHOD("is_autoplay"), &Derived::is_autoplay);
        ClassDB::bind_method(D_METHOD("set_one_shot", "_is_one_shot"), &Derived::set_one_shot);
        ClassDB::bind_method(D_METHOD("is_one_shot"), &Derived::is_one_shot);
        ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &Derived::set_allow_fadeout);
        ClassDB::bind_method(D_METHOD("is_allow_fadeout"), &Derived::is_allow_fadeout);
        ClassDB::bind_method(D_METHOD("set_preload_event", "preload_event"), &Derived::set_preload_event);
        ClassDB::bind_method(D_METHOD("is_preload_event"), &Derived::is_preload_event);
        ClassDB::bind_method(D_METHOD("_notification", "p_what"), &Derived::_notification);
        ClassDB::bind_method(D_METHOD("get_parameters"), &Derived::get_parameters);
        ClassDB::bind_method(D_METHOD("set_parameters", "p_parameters"), &Derived::set_parameters);
        ClassDB::bind_method(D_METHOD("get_volume"), &Derived::get_volume);
        ClassDB::bind_method(D_METHOD("set_volume", "p_volume"), &Derived::set_volume);
        ClassDB::bind_method(D_METHOD("_emit_callbacks", "dict", "type"), &Derived::_emit_callbacks);

        ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_name",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_event_name", "get_event_name");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "attached",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_attached", "is_attached");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_autoplay", "is_autoplay");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_shot",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_one_shot", "is_one_shot");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_allow_fadeout", "is_allow_fadeout");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_event",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_preload_event", "is_preload_event");
        ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "parameters",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_parameters", "get_parameters");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_volume", "get_volume");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "paused",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_paused", "is_paused");

        ADD_SIGNAL(MethodInfo(BEAT_SIGNAL_STRING, PropertyInfo(Variant::DICTIONARY, "params")));
        ADD_SIGNAL(MethodInfo(MARKER_SIGNAL_STRING, PropertyInfo(Variant::DICTIONARY, "params")));
        ADD_SIGNAL(MethodInfo(START_FAILED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(STARTED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(RESTARTED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(STOPPED_SIGNAL_STRING));
    }
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_EMITTER_H
