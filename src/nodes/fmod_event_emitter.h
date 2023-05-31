#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_H

#include "classes/object.hpp"

#include <fmod_server.h>

namespace godot {

    struct FmodEventEmitter {

        Ref<FmodEvent> _event;

        String _event_name;
        bool _attached = true;
        bool _autoplay = false;
        bool _is_one_shot = false;
        bool _allow_fadeout = true;
        bool _preload_event = true;

        Dictionary _params;

        void set_param(const String& key, const float value) {
            _params[key] = value;
            if (!_event.is_valid()) { return; }
            _event->set_parameter_by_name(key, _params[key]);
        }

        bool is_paused() {
            if (!_event.is_valid()) { return false; }
            return _event->get_paused();
        }

        void play() {
            if (!_event.is_valid()) { return; }
            _event->set_paused(false);
        }

        void pause() {
            if (!_event.is_valid()) { return; }
            _event->set_paused(true);
        }

        void set_event_name(const String& name) {
            if (name.begins_with("event:/")) {
                _event_name = name;
            } else {
                _event_name = "event:/" + name;
            }
        }

        String get_event_name() { return _event_name; }

        void set_attached(const bool attached) { this->_attached = attached; }

        bool is_attached() const { return _attached; }

        void set_autoplay(const bool autoplay) { this->_autoplay = autoplay; }

        bool is_autoplay() const { return _autoplay; }

        void set_looped(const bool looped) { this->_is_one_shot = looped; }

        bool is_looped() const { return _is_one_shot; }

        void set_allow_fadeout(const bool allow_fadeout) { this->_allow_fadeout = allow_fadeout; }

        bool is_allow_fadeout() const { return _allow_fadeout; }

        void set_preload_event(const bool preload_event) { this->_preload_event = preload_event; }

        bool is_preload_event() const { return _preload_event; }
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_EMITTER_H
