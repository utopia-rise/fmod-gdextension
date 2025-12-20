#ifndef GODOTFMOD_FMOD_EVENT_EMITTER_H
#define GODOTFMOD_FMOD_EVENT_EMITTER_H

#include "classes/object.hpp"

#include <constants.h>
#include <fmod_server.h>
#include <fmod_string_names.h>

#include <classes/engine.hpp>
#include <classes/project_settings.hpp>

static constexpr const char* BEAT_SIGNAL_STRING = "timeline_beat";
static constexpr const char* MARKER_SIGNAL_STRING = "timeline_marker";
static constexpr const char* START_FAILED_SIGNAL_STRING = "start_failed";
static constexpr const char* STARTED_SIGNAL_STRING = "started";
static constexpr const char* RESTARTED_SIGNAL_STRING = "restarted";
static constexpr const char* STOPPED_SIGNAL_STRING = "stopped";

namespace godot {

    template<class Derived, class NodeType>
    class FmodEventEmitter : public NodeType {
        struct Parameter : public FmodServer::ParameterValue {
            String name;
            uint64_t id;
            PackedStringArray labels;

            bool operator==(const Parameter& parameter) const { return id == parameter.id; }
        };

        mutable Ref<FmodEventDescription> _event_description;
        Ref<FmodEvent> _event;

        String _event_name;
        String _programmer_callback_sound_key;
        LocalVector<Parameter> _parameters;
        FMOD_GUID _event_guid;
        float _volume = 1.0;
        bool _attached = true;
        bool _autoplay = false;
        bool _auto_release = false;
        bool _allow_fadeout = true;
        bool _preload_event = true;

        void ready();
        void process();
        void exit_tree();

    public:
        void _notification(int p_what);

        void play(bool restart_if_playing = true);
        void stop();
        void play_one_shot();
        Variant get_parameter(const String& p_name) const;
        void set_parameter(const String& p_name, const Variant& p_property);
        Variant get_parameter_by_id(uint64_t p_id) const;
        void set_parameter_by_id(uint64_t p_id, const Variant& p_property);
        void set_paused(bool p_is_paused);
        const Ref<FmodEvent>& get_event() const;
        bool is_paused();
        void set_event_name(const String& name);
        String get_event_name() const;
        void set_event_guid(const String& guid);
        String get_event_guid() const;
        void set_attached(bool attached);
        bool is_attached() const;
        void set_autoplay(bool autoplay);
        bool is_autoplay() const;
        void set_auto_release(bool auto_release);
        bool is_auto_release() const;
        void set_allow_fadeout(bool allow_fadeout);
        bool is_allow_fadeout() const;
        void set_preload_event(bool preload_event);
        bool is_preload_event() const;
        void set_volume(float volume);
        float get_volume() const;

        void set_programmer_callback(const String& p_programmers_callback_sound_key);

#ifdef TOOLS_ENABLED
        void tool_remove_all_parameters();
        void tool_remove_parameter(uint64_t parameter_id);
#endif

        static const StringName& get_class_static();

    protected:
        void _emit_callbacks(const Dictionary& dict, int type) const;
        bool _set(const StringName& p_name, const Variant& p_property);
        bool _get(const StringName& p_name, Variant& r_property) const;
        bool _property_can_revert(const StringName& p_name) const;
        bool _property_get_revert(const StringName& p_name, Variant& r_property) const;
        void _get_property_list(List<PropertyInfo>* p_list) const;
        static void _bind_methods();

    private:
        template<bool is_one_shot> void _play(bool should_start_event);
        void set_space_attribute(const Ref<FmodEvent>& p_event) const;
        void _set_parameter_value(Parameter* parameter, const Variant& p_property);
        void apply_parameters();
        void free();
        void _load_event_description_if_needed() const;
        void _load_event();
        void _unload_event();
        Ref<FmodEvent> _create_event();
        Parameter* _find_parameter(const String& p_name) const;
        Parameter* _find_parameter(uint64_t p_id) const;
        void _stop_and_restart_if_autoplay();
        Ref<FmodParameterDescription> _get_parameter_description(const Parameter& parameter) const;


        static bool _should_load_by_event_name();
    };

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_space_attribute(const Ref<FmodEvent>& p_event) const {
        static_cast<const Derived*>(this)->set_space_attribute_impl(p_event);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::free() {
        static_cast<Derived*>(this)->free_impl();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::ready() {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        if (_autoplay) {
            play();
        } else if (_preload_event) {
            // No need to preload if autoplay is on because event is loaded anyway.
            _load_event();
        }
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::process() {
#ifdef TOOLS_ENABLED
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        if (_event.is_null()) {
            // No event loaded, nothing to do here
            return;
        }

        if (!_event->is_valid()) {
            // Event was loaded and is done playing.
            if (_auto_release) {
                free();
                return;
            }
            if (_autoplay) {
                play();
            }
        }

        if (_attached && _event->is_valid()) { set_space_attribute(_event); }
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::exit_tree() {
#ifdef TOOLS_ENABLED
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        if (_event.is_null()) {
            // No event loaded, nothing to stop or free.
            return;
        }

        if (_event->is_valid()) {
            _event->release();
            stop();
        }
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_notification(int p_what) {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif
        switch (p_what) {
            case Node::NOTIFICATION_PAUSED:
                set_paused(true);
                break;
            case Node::NOTIFICATION_UNPAUSED:
                if (is_paused()) { set_paused(false); }
                break;
            case Node::NOTIFICATION_READY:
                ready();
                break;
            case Node::NOTIFICATION_PROCESS:
                process();
                break;
            case Node::NOTIFICATION_EXIT_TREE:
                exit_tree();
                break;
            default:
                break;
        }
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_paused() {
        if (_event.is_null() || !_event->is_valid()) { return false; }
        return _event->get_paused();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::play(bool restart_if_playing) {
        _play<false>(restart_if_playing);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::stop() {
        if (_event.is_null() || !_event->is_valid()) { return; }
        if (_allow_fadeout) {
            _event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

            return;
        }
        _event->stop(FMOD_STUDIO_STOP_IMMEDIATE);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::play_one_shot() {
        _play<true>(true);
    }

    template<class Derived, class NodeType>
    template<bool is_one_shot>
    void FmodEventEmitter<Derived, NodeType>::_play(bool should_start_event) {
        Ref<FmodEvent> event;
        if (is_one_shot) {
            event = _create_event();
        } else {
            if (_event.is_null() || !_event->is_valid()) { _load_event(); }

            event = _event;
        }

        if (event.is_null()) {
            // No event loaded, nothing to do here
            return;
        }

        event->set_volume(_volume);
        apply_parameters();

        set_space_attribute(event);
        if (!_programmer_callback_sound_key.is_empty()) {
            event->set_programmer_callback(_programmer_callback_sound_key);
        }

        if (!should_start_event && event->get_playback_state() != FMOD_STUDIO_PLAYBACK_STOPPED) {
            return;
        }

        event->start();
        event->release();
    }

    template<class Derived, class NodeType>
    Variant FmodEventEmitter<Derived, NodeType>::get_parameter(const String& p_name) const {
        if (Parameter* parameter {_find_parameter(p_name)}) {
            return parameter->value;
        }

        return Variant();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_parameter(const String& p_name, const Variant& p_property) {
        Parameter* parameter {_find_parameter(p_name)};

        if (!parameter) {
            Parameter param;

            _parameters.push_back(param);
            parameter = &_parameters[_parameters.size() - 1];
            parameter->name = p_name;
            parameter->identifier.name = &parameter->name;
            parameter->should_load_by_id = false;
        }

        _set_parameter_value(parameter, p_property);
    }

    template<class Derived, class NodeType>
    Variant FmodEventEmitter<Derived, NodeType>::get_parameter_by_id(uint64_t p_id) const {
        if (Parameter* parameter {_find_parameter(p_id)}) {
            return parameter->value;
        }

        return Variant();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_parameter_by_id(const uint64_t p_id, const Variant& p_property) {
        Parameter* parameter {_find_parameter(p_id)};

        if (!parameter) {
            Parameter param;
            _parameters.push_back(param);
            parameter = &_parameters[_parameters.size() - 1];
            parameter->id = p_id;
            parameter->should_load_by_id = true;
            parameter->identifier.id = p_id;
        }

        _set_parameter_value(parameter, p_property);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_paused(bool p_is_paused) {
        if (_event.is_null() || !_event->is_valid()) { return; }
        _event->set_paused(p_is_paused);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_load_event_description_if_needed() const {
        if (!_event_description.is_null()) {
            return;
        }

        if (_should_load_by_event_name()) {
#ifdef DEBUG_ENABLED
            if (FmodServer::get_singleton()->check_event_path(_event_name)) {
#endif
                _event_description = FmodServer::get_singleton()->get_event(_event_name);
#ifdef DEBUG_ENABLED
            } else {
                GODOT_LOG_ERROR(vformat("Cannot find event with path %s, will try with guid", _event_name));
                GODOT_LOG_ERROR("You should fix this before releasing your game, check event exists and fallback is "
                                "only a debug feature");
                if (FmodServer::get_singleton()->check_event_guid_internal(_event_guid)) {
                    _event_description = FmodServer::get_singleton()->get_event_from_guid_internal(_event_guid);
                } else {
                    GODOT_LOG_ERROR(
                      vformat("Cannot find event with guid %s and path %s. Please set right data from editor.", get_event_guid(), _event_name)
                    );
                    GODOT_LOG_ERROR("You should fix this before releasing your game, check event exists and fallback "
                                    "is only a debug feature");
                }
            }
#endif
        } else {
#ifdef DEBUG_ENABLED
            if (FmodServer::get_singleton()->check_event_guid_internal(_event_guid)) {
#endif
                _event_description = FmodServer::get_singleton()->get_event_from_guid_internal(_event_guid);
#ifdef DEBUG_ENABLED
            } else {
                GODOT_LOG_ERROR(vformat("Cannot find event with guid %s, will try with guid", get_event_guid()));
                GODOT_LOG_ERROR("You should fix this before releasing your game, check event exists and fallback is "
                                "only a debug feature");
                if (FmodServer::get_singleton()->check_event_path(_event_name)) {
                    _event_description = FmodServer::get_singleton()->get_event(_event_name);
                } else {
                    GODOT_LOG_ERROR(
                      vformat("Cannot find event with guid %s and path %s. Please set right data from editor.", get_event_guid(), _event_name)
                    );
                    GODOT_LOG_ERROR("You should fix this before releasing your game, check event exists and fallback "
                                    "is only a debug feature");
                }
            }
#endif
        }
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_load_event() {
        _event = _create_event();

        if (_event.is_null()) {
            return;
        }

        _event->set_callback(Callable(this, "_emit_callbacks"), FMOD_STUDIO_EVENT_CALLBACK_ALL);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_unload_event() {
        _event_description = Ref<FmodEventDescription>();
        _event = Ref<FmodEvent>();
    }

    template<class Derived, class NodeType>
    Ref<FmodEvent> FmodEventEmitter<Derived, NodeType>::_create_event() {
        _load_event_description_if_needed();

        if (_event_description.is_null()) {
            return {};
        }

        return FmodServer::get_singleton()->create_event_instance_from_description(_event_description);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_set_parameter_value(FmodEventEmitter::Parameter* parameter,
                                                                   const Variant& p_property) {
        parameter->value = p_property;

        if (_event.is_null() || !_event->is_valid()) { return; }

#ifdef TOOLS_ENABLED
        if (!Engine::get_singleton()->is_editor_hint()) {
#endif
            apply_parameters();
#ifdef TOOLS_ENABLED
        }
#endif
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::apply_parameters() {
        if (_event.is_null() || !_event->is_valid()) { return; }
        FmodServer::get_singleton()->apply_parameter_list_to_event(_event, _parameters);
    }

    template<class Derived, class NodeType>
    const Ref<FmodEvent>& FmodEventEmitter<Derived, NodeType>::get_event() const {
        return _event;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_event_name(const String& name) {
        FMOD_GUID event_guid {FmodServer::get_singleton()->get_event_guid_internal(name)};

        if (equals(_event_guid, event_guid)) {
            return;
        }

        _event_name = name;
        _event_guid = event_guid;

        _stop_and_restart_if_autoplay();
    }

    template<class Derived, class NodeType>
    String FmodEventEmitter<Derived, NodeType>::get_event_name() const {
        return _event_name;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_event_guid(const String& guid) {
        FMOD_GUID event_guid {string_to_fmod_guid(guid.utf8().get_data())};

        if (equals(_event_guid, event_guid)) {
            return;
        }

        _event_guid = event_guid;
        _event_name = FmodServer::get_singleton()->get_event_path_internal(_event_guid);

        _stop_and_restart_if_autoplay();
    }

    template<class Derived, class NodeType>
    String FmodEventEmitter<Derived, NodeType>::get_event_guid() const {
        return fmod_guid_to_string(_event_guid);
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_stop_and_restart_if_autoplay() {
        if (!reinterpret_cast<Derived*>(this)->is_node_ready()) return;

        stop();

        _unload_event();

        if (_preload_event) {
            _load_event();
        }

        _parameters.clear();

        if (!_autoplay) return;

        play();
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
    void FmodEventEmitter<Derived, NodeType>::set_auto_release(const bool auto_release) {
        _auto_release = auto_release;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::is_auto_release() const {
        return _auto_release;
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

        if (_event.is_null() || !_event->is_valid()) { return; }
        _event->set_volume(volume);
    }

    template<class Derived, class NodeType>
    float FmodEventEmitter<Derived, NodeType>::get_volume() const {
        return _volume;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::set_programmer_callback(const String &p_programmers_callback_sound_key) {
        _programmer_callback_sound_key = p_programmers_callback_sound_key;
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
    typename FmodEventEmitter<Derived, NodeType>::Parameter* FmodEventEmitter<Derived, NodeType>::_find_parameter(const String& p_name
    ) const {
        Parameter* parameter {nullptr};
        for (const Parameter& item : _parameters) {
            if (item.name != p_name) { continue; }
            parameter = const_cast<Parameter*>(&item);
            break;
        }
        return parameter;
    }

    template<class Derived, class NodeType>
    typename FmodEventEmitter<Derived, NodeType>::Parameter* FmodEventEmitter<Derived, NodeType>::_find_parameter(uint64_t p_id) const {
        Parameter* parameter {nullptr};
        for (const Parameter& item : _parameters) {
            if (item.id != p_id) { continue; }
            parameter = const_cast<Parameter*>(&item);
            break;
        }
        return parameter;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::_should_load_by_event_name() {
#ifndef TOOLS_ENABLED
        static
#endif
          bool should_load_by_name {ProjectSettings::get_singleton()->get_setting(
            vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::SHOULD_LOAD_BY_NAME)
          )};
        return should_load_by_name;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::_set(const StringName& p_name, const Variant& p_property) {
        if (!p_name.begins_with(FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)) { return false; }
        if (p_name == FmodStringNames::get_instance()->event_parameter_prefix_for_properties) { return false; }

        PackedStringArray parts {p_name.trim_prefix(vformat("%s/", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)).split("/")};

        const String& parameter_name {parts[0]};

        Parameter* parameter {_find_parameter(parameter_name)};

        if (!parameter) {
            Parameter param;

            _parameters.push_back(param);
            parameter = &_parameters[_parameters.size() - 1];
            parameter->name = parameter_name;
            parameter->should_load_by_id = !_should_load_by_event_name();
        }

        if (parts.size() == 1) {
            _set_parameter_value(parameter, p_property);

            return true;
        }

        const String& parameter_end = parts[1];
        if (parameter_end == "id") {
            uint64_t parameter_id = p_property.operator uint64_t();

            parameter->id = parameter_id;

            if (parameter->should_load_by_id) {
                parameter->identifier.id = parameter_id;
                return true;
            }

            parameter->identifier.name = &parameter->name;
            return true;
        }

        if (parameter_end == "variant_type") {
            parameter->variant_type = static_cast<Variant::Type>(p_property.operator int32_t());
            return true;
        }

        if (parameter_end == "labels") {
            parameter->labels = p_property;
            return true;
        }

        return false;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::_get(const StringName& p_name, Variant& r_property) const {
        if (!p_name.begins_with(FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)) { return false; }
        if (p_name == FmodStringNames::get_instance()->event_parameter_prefix_for_properties) { return false; }

        PackedStringArray parts {p_name.trim_prefix(vformat("%s/", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)).split("/")};

        Parameter* parameter {_find_parameter(parts[0])};

        if (!parameter) { return false; }

        if (parts.size() == 1) {
            r_property = parameter->value;
            return true;
        }

        const String& parameter_end = parts[1];
        if (parameter_end == "id") {
            r_property = parameter->id;
            return true;
        }

        if (parameter_end == "variant_type") {
            r_property = parameter->variant_type;
            return true;
        }

        if (parameter_end == "labels") {
            r_property = parameter->labels;
            return true;
        }

        return false;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::_property_can_revert(const StringName& p_name) const {
        if (!p_name.begins_with(FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)) { return false; }
        if (p_name == FmodStringNames::get_instance()->event_parameter_prefix_for_properties) { return false; }

        PackedStringArray parts {p_name.trim_prefix(vformat("%s/", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)).split("/")};

        if (parts.size() == 1) { return true; }

        return false;
    }

    template<class Derived, class NodeType>
    bool FmodEventEmitter<Derived, NodeType>::_property_get_revert(const StringName& p_name, Variant& r_property) const {
        if (!p_name.begins_with(FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)) { return false; }
        if (p_name == FmodStringNames::get_instance()->event_parameter_prefix_for_properties) { return false; }

        PackedStringArray parts {p_name.trim_prefix(vformat("%s/", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)).split("/")};

        Parameter* parameter {_find_parameter(parts[0])};

        if (!parameter) { return false; }

        if (parts.size() == 1) {
            Ref<FmodParameterDescription> desc {_get_parameter_description(*parameter)};
            if (desc.is_null()) { return false; }
            r_property = desc->get_default_value();
            return true;
        }

        return false;
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_get_property_list(List<PropertyInfo>* p_list) const {
        p_list->push_back(
            PropertyInfo(
                Variant::Type::DICTIONARY,
                FmodStringNames::get_instance()->event_parameter_prefix_for_properties,
                PROPERTY_HINT_NONE,
                "",
                PROPERTY_USAGE_NO_EDITOR
            )
        );

        for (const Parameter& parameter : _parameters) {
            const String& parameter_name {parameter.name};

            Ref<FmodParameterDescription> parameter_description{_get_parameter_description(parameter) };
            if (parameter_description.is_null()) {
                // Skip parameters that cannot be resolved (e.g., missing or stale IDs)
                continue;
            }

            const float parameter_min_value {parameter_description->get_minimum()};
            const float parameter_max_value {parameter_description->get_maximum()};
            const Variant::Type parameter_variant_type {parameter.variant_type};

            p_list->push_back(
              PropertyInfo(Variant::Type::INT, vformat("%s/%s/id", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR)
            );

            if (!parameter.labels.is_empty()) {
                p_list->push_back(
                  PropertyInfo(
                    parameter_variant_type,
                    vformat("%s/%s", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name),
                    PROPERTY_HINT_ENUM,
                    vformat(String(",").join(parameter.labels))
                  )
                );
                p_list->push_back(
                  PropertyInfo(
                    Variant::Type::PACKED_STRING_ARRAY,
                    vformat("%s/%s/labels", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name),
                    PROPERTY_HINT_NONE,
                    "",
                    PROPERTY_USAGE_NO_EDITOR
                  )
                );
            } else {
                p_list->push_back(
                  PropertyInfo(
                    parameter_variant_type,
                    vformat("%s/%s", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name),
                    PROPERTY_HINT_RANGE,
                    vformat("%s,%s,0.1", parameter_min_value, parameter_max_value)
                  )
                );
            }

            p_list->push_back(
              PropertyInfo(
                Variant::Type::INT,
                vformat("%s/%s/variant_type", FmodStringNames::EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name),
                PROPERTY_HINT_ENUM,
                "",
                PROPERTY_USAGE_NO_EDITOR
              )
            );
        }
    }

    template<class Derived, class NodeType>
    Ref<FmodParameterDescription>
    FmodEventEmitter<Derived, NodeType>::_get_parameter_description(const FmodEventEmitter::Parameter& parameter) const {
        _load_event_description_if_needed();

        if (_event_description.is_null() || !_event_description->is_valid()) {
            return {};
        }

        return parameter.should_load_by_id
               ? _event_description->get_parameter_by_id(parameter.id)
               : _event_description->get_parameter_by_name(parameter.name);
    }

#ifdef TOOLS_ENABLED
    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::tool_remove_all_parameters() {
        if (!Engine::get_singleton()->is_editor_hint()) { return; }
        _parameters.clear();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::tool_remove_parameter(uint64_t parameter_id) {
        if (!Engine::get_singleton()->is_editor_hint()) { return; }

        for (int i = _parameters.size() - 1; i >= 0; --i) {
            const Parameter& parameter {_parameters[i]};
            if (parameter.id != parameter_id) continue;
            _parameters.erase(parameter);
        }
    }
#endif

    template<class Derived, class NodeType>
    const StringName& FmodEventEmitter<Derived, NodeType>::get_class_static() {
        return Derived::get_class_static();
    }

    template<class Derived, class NodeType>
    void FmodEventEmitter<Derived, NodeType>::_bind_methods() {
        ClassDB::bind_method(D_METHOD("play", "restart_if_playing"), &Derived::play, DEFVAL(true));
        ClassDB::bind_method(D_METHOD("play_one_shot"), &Derived::play_one_shot);
        ClassDB::bind_method(D_METHOD("stop"), &Derived::stop);
        ClassDB::bind_method(D_METHOD("set_parameter", "name", "value"), &Derived::set_parameter);
        ClassDB::bind_method(D_METHOD("get_parameter", "name"), &Derived::get_parameter);
        ClassDB::bind_method(D_METHOD("set_parameter_by_id", "id", "value"), &Derived::set_parameter_by_id);
        ClassDB::bind_method(D_METHOD("get_parameter_by_id", "id"), &Derived::get_parameter_by_id);
        ClassDB::bind_method(D_METHOD("is_paused"), &Derived::is_paused);
        ClassDB::bind_method(D_METHOD("set_paused", "p_is_paused"), &Derived::set_paused);
        ClassDB::bind_method(D_METHOD("set_event_name", "event_name"), &Derived::set_event_name);
        ClassDB::bind_method(D_METHOD("get_event_name"), &Derived::get_event_name);
        ClassDB::bind_method(D_METHOD("set_event_guid", "event_guid"), &Derived::set_event_guid);
        ClassDB::bind_method(D_METHOD("get_event_guid"), &Derived::get_event_guid);
        ClassDB::bind_method(D_METHOD("set_attached", "attached"), &Derived::set_attached);
        ClassDB::bind_method(D_METHOD("is_attached"), &Derived::is_attached);
        ClassDB::bind_method(D_METHOD("set_autoplay", "_autoplay"), &Derived::set_autoplay);
        ClassDB::bind_method(D_METHOD("is_autoplay"), &Derived::is_autoplay);
        ClassDB::bind_method(D_METHOD("set_auto_release", "_autoplay"), &Derived::set_auto_release);
        ClassDB::bind_method(D_METHOD("is_auto_release"), &Derived::is_auto_release);
        ClassDB::bind_method(D_METHOD("set_allow_fadeout", "allow_fadeout"), &Derived::set_allow_fadeout);
        ClassDB::bind_method(D_METHOD("is_allow_fadeout"), &Derived::is_allow_fadeout);
        ClassDB::bind_method(D_METHOD("set_preload_event", "preload_event"), &Derived::set_preload_event);
        ClassDB::bind_method(D_METHOD("is_preload_event"), &Derived::is_preload_event);
        ClassDB::bind_method(D_METHOD("get_volume"), &Derived::get_volume);
        ClassDB::bind_method(D_METHOD("set_volume", "p_volume"), &Derived::set_volume);
        ClassDB::bind_method(D_METHOD("set_programmer_callback", "p_programmers_callback_sound_key"), &Derived::set_programmer_callback);
        ClassDB::bind_method(D_METHOD("_emit_callbacks", "dict", "type"), &Derived::_emit_callbacks);

#ifdef TOOLS_ENABLED
        ClassDB::bind_method(D_METHOD("tool_remove_all_parameters"), &Derived::tool_remove_all_parameters);
        ClassDB::bind_method(D_METHOD("tool_remove_parameter", "parameter_id"), &Derived::tool_remove_parameter);
#endif

        ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_event_name", "get_event_name");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_guid", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_event_guid", "get_event_guid");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "attached", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_attached", "is_attached");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_autoplay", "is_autoplay");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_release", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_auto_release", "is_auto_release");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_fadeout", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_allow_fadeout", "is_allow_fadeout");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preload_event", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_preload_event", "is_preload_event");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "set_volume", "get_volume");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "paused", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_paused", "is_paused");

        ADD_SIGNAL(MethodInfo(BEAT_SIGNAL_STRING, PropertyInfo(Variant::DICTIONARY, "params")));
        ADD_SIGNAL(MethodInfo(MARKER_SIGNAL_STRING, PropertyInfo(Variant::DICTIONARY, "params")));
        ADD_SIGNAL(MethodInfo(START_FAILED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(STARTED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(RESTARTED_SIGNAL_STRING));
        ADD_SIGNAL(MethodInfo(STOPPED_SIGNAL_STRING));
    }
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_EMITTER_H
