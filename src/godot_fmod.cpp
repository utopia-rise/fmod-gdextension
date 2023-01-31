//
// Created by Pierre-Thomas Meisels on 2019-01-01.
//

#include <classes/node3d.hpp>
#include <godot_fmod.h>

using namespace godot;

Fmod* Fmod::singleton = nullptr;

Fmod::Fmod() : system(nullptr), coreSystem(nullptr), isInitialized(false), isNotinitPrinted(false), distanceScale(1.0) {
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;

    system = nullptr;
    coreSystem = nullptr;
    isInitialized = false;
    isNotinitPrinted = false;
    Callbacks::GodotFileRunner::get_singleton()->start();
    performanceData["CPU"] = Dictionary();
    performanceData["memory"] = Dictionary();
    performanceData["file"] = Dictionary();
    distanceScale = 1.0;
}

Fmod::~Fmod() {
    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}

Fmod* Fmod::get_singleton() {
    return singleton;
}

void Fmod::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "numOfChannels", "studioFlag", "flag"), &Fmod::init);
    ClassDB::bind_method(D_METHOD("shutdown"), &Fmod::shutdown);
    ClassDB::bind_method(D_METHOD("add_listener", "index", "gameObj"), &Fmod::add_listener);
    ClassDB::bind_method(D_METHOD("remove_listener", "index"), &Fmod::remove_listener);
    ClassDB::bind_method(D_METHOD("set_listener_number", "listenerNumber"), &Fmod::set_system_listener_number);
    ClassDB::bind_method(D_METHOD("get_listener_number"), &Fmod::get_system_listener_number);
    ClassDB::bind_method(D_METHOD("get_listener_weight", "index"), &Fmod::get_system_listener_weight);
    ClassDB::bind_method(D_METHOD("set_listener_weight", "index", "weight"), &Fmod::set_system_listener_weight);
    ClassDB::bind_method(D_METHOD("get_listener_3D_attributs", "index"), &Fmod::get_system_listener_3d_attributes);
    ClassDB::bind_method(D_METHOD("get_listener_2D_attributs", "index"), &Fmod::get_system_listener_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_listener_3D_attributs", "index", "transform"), &Fmod::set_system_listener_3d_attributes);
    ClassDB::bind_method(D_METHOD("set_listener_2D_attributs", "index", "transform"), &Fmod::set_system_listener_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_listener_lock", "index", "isLocked"), &Fmod::set_listener_lock);
    ClassDB::bind_method(D_METHOD("get_listener_lock", "index"), &Fmod::get_listener_lock);
    ClassDB::bind_method(D_METHOD("get_object_attached_to_listener", "index"), &Fmod::get_object_attached_to_listener);
    ClassDB::bind_method(D_METHOD("set_software_format", "sampleRate", "speakerMode", "numRawSpeakers"), &Fmod::set_software_format);
    ClassDB::bind_method(D_METHOD("load_bank", "pathToBank", "flag"), &Fmod::load_bank);
    ClassDB::bind_method(D_METHOD("unload_bank", "pathToBank"), &Fmod::unload_bank);
    ClassDB::bind_method(D_METHOD("check_VCA_path", "cvaPath"), &Fmod::check_vca_path);
    ClassDB::bind_method(D_METHOD("check_bus_path", "busPath"), &Fmod::check_bus_path);
    ClassDB::bind_method(D_METHOD("check_event_path", "eventPath"), &Fmod::check_event_path);
    ClassDB::bind_method(D_METHOD("get_bank_loading_state", "pathToBank"), &Fmod::get_bank_loading_state);
    ClassDB::bind_method(D_METHOD("get_bank_bus_count", "pathToBank"), &Fmod::get_bank_bus_count);
    ClassDB::bind_method(D_METHOD("get_bank_event_count", "pathToBank"), &Fmod::get_bank_event_count);
    ClassDB::bind_method(D_METHOD("get_bank_string_count", "pathToBank"), &Fmod::get_bank_string_count);
    ClassDB::bind_method(D_METHOD("get_bank_VCA_count", "pathToBank"), &Fmod::get_bank_vca_count);
    ClassDB::bind_method(D_METHOD("create_event_instance", "eventPath"), &Fmod::create_event_instance);
    ClassDB::bind_method(D_METHOD("get_event_parameter_by_name", "instanceId", "parameterName"), &Fmod::get_event_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_event_parameter_by_name", "instanceId", "parameterName", "value"), &Fmod::set_event_parameter_by_name);
    ClassDB::bind_method(D_METHOD("get_event_parameter_by_id", "instanceId", "idPair"), &Fmod::get_event_parameter_by_id);
    ClassDB::bind_method(D_METHOD("set_event_parameter_by_id", "instanceId", "idPair", "value"), &Fmod::set_event_parameter_by_id);
    ClassDB::bind_method(D_METHOD("release_event", "instanceId"), &Fmod::release_event);
    ClassDB::bind_method(D_METHOD("start_event", "instanceId"), &Fmod::start_event);
    ClassDB::bind_method(D_METHOD("stop_event", "instanceId", "stopMode"), &Fmod::stop_event);
    ClassDB::bind_method(D_METHOD("event_key_off", "instanceId"), &Fmod::event_key_off);
    ClassDB::bind_method(D_METHOD("get_event_playback_state", "instanceId"), &Fmod::get_event_playback_state);
    ClassDB::bind_method(D_METHOD("get_event_paused", "instanceId"), &Fmod::get_event_paused);
    ClassDB::bind_method(D_METHOD("set_event_paused", "instanceId", "paused"), &Fmod::set_event_paused);
    ClassDB::bind_method(D_METHOD("get_event_pitch", "instanceId"), &Fmod::get_event_pitch);
    ClassDB::bind_method(D_METHOD("set_event_pitch", "instanceId", "pitch"), &Fmod::set_event_pitch);
    ClassDB::bind_method(D_METHOD("get_event_volume", "instanceId"), &Fmod::get_event_volume);
    ClassDB::bind_method(D_METHOD("set_event_volume", "instanceId", "volume"), &Fmod::set_event_volume);
    ClassDB::bind_method(D_METHOD("get_event_timeline_position", "instanceId"), &Fmod::get_event_timeline_position);
    ClassDB::bind_method(D_METHOD("set_event_timeline_position", "instanceId", "position"), &Fmod::set_event_timeline_position);
    ClassDB::bind_method(D_METHOD("get_event_reverb_level", "instanceId", "index"), &Fmod::get_event_reverb_level);
    ClassDB::bind_method(D_METHOD("set_event_reverb_level", "instanceId", "index", "level"), &Fmod::set_event_reverb_level);
    ClassDB::bind_method(D_METHOD("is_event_virtual", "instanceId"), &Fmod::is_event_virtual);
    ClassDB::bind_method(D_METHOD("set_event_listener_mask", "instanceId", "mask"), &Fmod::set_event_listener_mask);
    ClassDB::bind_method(D_METHOD("get_event_listener_mask", "instanceId"), &Fmod::get_event_listener_mask);
    ClassDB::bind_method(D_METHOD("set_event_2d_attributes", "instanceId", "position"), &Fmod::set_event_2d_attributes);
    ClassDB::bind_method(D_METHOD("get_event_2d_attributes", "instanceId"), &Fmod::get_event_2d_attributes);
    ClassDB::bind_method(D_METHOD("set_event_3d_attributes", "instanceId", "transform"), &Fmod::set_event_3d_attributes);
    ClassDB::bind_method(D_METHOD("get_event_3d_attributes", "instanceId"), &Fmod::get_event_3d_attributes);
    ClassDB::bind_method(D_METHOD("desc_get_length", "eventPath"), &Fmod::desc_get_length);
    ClassDB::bind_method(D_METHOD("desc_get_instance_list", "eventPath"), &Fmod::desc_get_instance_list);
    ClassDB::bind_method(D_METHOD("desc_get_instance_count", "eventPath"), &Fmod::desc_get_instance_count);
    ClassDB::bind_method(D_METHOD("desc_release_all_instances", "eventPath"), &Fmod::desc_release_all_instances);
    ClassDB::bind_method(D_METHOD("desc_load_sample_data", "eventPath"), &Fmod::desc_load_sample_data);
    ClassDB::bind_method(D_METHOD("desc_unload_sample_data", "eventPath"), &Fmod::desc_unload_sample_data);
    ClassDB::bind_method(D_METHOD("desc_get_sample_loading_state", "eventPath"), &Fmod::desc_get_sample_loading_state);
    ClassDB::bind_method(D_METHOD("desc_is_3d", "eventPath"), &Fmod::desc_is_3d);
    ClassDB::bind_method(D_METHOD("desc_is_one_shot", "eventPath"), &Fmod::desc_is_one_shot);
    ClassDB::bind_method(D_METHOD("desc_is_snapshot", "eventPath"), &Fmod::desc_is_snapshot);
    ClassDB::bind_method(D_METHOD("desc_is_stream", "eventPath"), &Fmod::desc_is_stream);
    ClassDB::bind_method(D_METHOD("desc_has_sustain_point", "eventPath"), &Fmod::desc_has_sustain_point);
    ClassDB::bind_method(D_METHOD("desc_get_min_max_distance", "eventPath"), &Fmod::desc_get_min_max_distance);
    ClassDB::bind_method(D_METHOD("desc_get_sound_size", "eventPath"), &Fmod::desc_get_sound_size);
    ClassDB::bind_method(D_METHOD("desc_get_parameter_description_by_name", "eventPath", "name"), &Fmod::desc_get_parameter_description_by_name);
    ClassDB::bind_method(D_METHOD("desc_get_parameter_description_by_id",
                                  "eventPath"
                                  "idPair"),
                         &Fmod::desc_get_parameter_description_by_id);
    ClassDB::bind_method(D_METHOD("desc_get_parameter_description_count", "eventPath"), &Fmod::desc_get_parameter_description_count);
    ClassDB::bind_method(D_METHOD("desc_get_parameter_description_by_index", "eventPath", "index"), &Fmod::desc_get_parameter_description_by_index);
    ClassDB::bind_method(D_METHOD("desc_get_user_property", "eventPath", "name"), &Fmod::desc_get_user_property);
    ClassDB::bind_method(D_METHOD("desc_get_user_property_count", "eventPath"), &Fmod::desc_get_user_property_count);
    ClassDB::bind_method(D_METHOD("desc_user_property_by_index", "eventPath", "index"), &Fmod::desc_user_property_by_index);
    ClassDB::bind_method(D_METHOD("get_bus_mute", "busPath"), &Fmod::get_bus_mute);
    ClassDB::bind_method(D_METHOD("get_bus_paused", "busPath"), &Fmod::get_bus_paused);
    ClassDB::bind_method(D_METHOD("get_bus_volume", "busPath"), &Fmod::get_bus_volume);
    ClassDB::bind_method(D_METHOD("set_bus_mute", "busPath", "mute"), &Fmod::set_bus_mute);
    ClassDB::bind_method(D_METHOD("set_bus_paused", "busPath", "paused"), &Fmod::set_bus_paused);
    ClassDB::bind_method(D_METHOD("set_bus_volume", "busPath", "volume"), &Fmod::set_bus_volume);
    ClassDB::bind_method(D_METHOD("stop_all_bus_events", "busPath", "stopMode"), &Fmod::stop_all_bus_events);
    ClassDB::bind_method(D_METHOD("get_VCA_volume", "VCAPath"), &Fmod::get_vca_volume);
    ClassDB::bind_method(D_METHOD("set_VCA_volume", "VCAPath", "volume"), &Fmod::set_vca_volume);
    ClassDB::bind_method(D_METHOD("play_one_shot", "eventName", "gameObj"), &Fmod::play_one_shot);
    ClassDB::bind_method(D_METHOD("play_one_shot_with_params", "eventName", "gameObj", "parameters"), &Fmod::play_one_shot_with_params);
    ClassDB::bind_method(D_METHOD("play_one_shot_attached", "eventName", "gameObj"), &Fmod::play_one_shot_attached);
    ClassDB::bind_method(D_METHOD("play_one_shot_attached_with_params", "eventName", "gameObj", "parameters"), &Fmod::play_one_shot_attached_with_params);
    ClassDB::bind_method(D_METHOD("attach_instance_to_node", "instanceId", "gameObj"), &Fmod::attach_instance_to_node);
    ClassDB::bind_method(D_METHOD("detach_instance_from_node", "instanceId"), &Fmod::detach_instance_from_node);
    ClassDB::bind_method(D_METHOD("get_object_attached_to_instance", "instanceId"), &Fmod::get_object_attached_to_instance);
    ClassDB::bind_method(D_METHOD("pause_all_events", "pause"), &Fmod::pause_all_events);
    ClassDB::bind_method(D_METHOD("mute_all_events"), &Fmod::mute_all_events);
    ClassDB::bind_method(D_METHOD("unmute_all_events"), &Fmod::unmute_all_events);
    ClassDB::bind_method(D_METHOD("banks_still_loading"), &Fmod::banks_still_loading);
    ClassDB::bind_method(D_METHOD("load_file_as_sound", "path"), &Fmod::load_file_as_sound);
    ClassDB::bind_method(D_METHOD("load_file_as_music", "path"), &Fmod::load_file_as_music);
    ClassDB::bind_method(D_METHOD("unload_file", "path"), &Fmod::unload_file);
    ClassDB::bind_method(D_METHOD("create_sound_instance", "path"), &Fmod::create_sound_instance);
    ClassDB::bind_method(D_METHOD("check_sound_instance", "instanceId"), &Fmod::check_sound_instance);
    ClassDB::bind_method(D_METHOD("release_sound", "instanceId"), &Fmod::release_sound);
    ClassDB::bind_method(D_METHOD("play_sound", "instanceId"), &Fmod::play_sound);
    ClassDB::bind_method(D_METHOD("stop_sound", "instanceId"), &Fmod::stop_sound);
    ClassDB::bind_method(D_METHOD("set_sound_paused", "instanceId", "paused"), &Fmod::set_sound_paused);
    ClassDB::bind_method(D_METHOD("is_sound_playing", "instanceId"), &Fmod::is_sound_playing);
    ClassDB::bind_method(D_METHOD("set_sound_volume", "instanceId", "volume"), &Fmod::set_sound_volume);
    ClassDB::bind_method(D_METHOD("get_sound_volume", "instanceId"), &Fmod::get_sound_volume);
    ClassDB::bind_method(D_METHOD("set_sound_pitch", "instanceId", "pitch"), &Fmod::set_sound_pitch);
    ClassDB::bind_method(D_METHOD("get_sound_pitch", "instanceId"), &Fmod::get_sound_pitch);
    ClassDB::bind_method(D_METHOD("set_callback", "instanceId", "callbackMask"), &Fmod::set_callback);
    ClassDB::bind_method(D_METHOD("set_sound_3D_settings", "dopplerScale", "distanceFactor", "rollOffScale"), &Fmod::set_sound_3d_settings);
    ClassDB::bind_method(D_METHOD("wait_for_all_loads"), &Fmod::wait_for_all_loads);
    ClassDB::bind_method(D_METHOD("get_available_drivers"), &Fmod::get_available_drivers);
    ClassDB::bind_method(D_METHOD("get_driver"), &Fmod::get_driver);
    ClassDB::bind_method(D_METHOD("set_driver", "id"), &Fmod::set_driver);
    ClassDB::bind_method(D_METHOD("get_performance_data"), &Fmod::get_performance_data);
    ClassDB::bind_method(D_METHOD("set_global_parameter_by_name", "parameterName", "value"), &Fmod::set_global_parameter_by_name);
    ClassDB::bind_method(D_METHOD("get_global_parameter_by_name", "parameterName"), &Fmod::get_global_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_global_parameter_by_id", "idPair", "value"), &Fmod::set_global_parameter_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_by_id", "idPair"), &Fmod::get_global_parameter_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_by_name", "parameterName"), &Fmod::get_global_parameter_desc_by_name);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_by_id", "idPair"), &Fmod::get_global_parameter_desc_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_count"), &Fmod::get_global_parameter_desc_count);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_list"), &Fmod::get_global_parameter_desc_list);
    ClassDB::bind_method(D_METHOD("set_system_dsp_buffer_size", "bufferlength", "numbuffers"), &Fmod::set_system_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_system_dsp_buffer_size"), &Fmod::get_system_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_system_dsp_buffer_length"), &Fmod::get_system_dsp_buffer_length);
    ClassDB::bind_method(D_METHOD("get_system_dsp_num_buffers"), &Fmod::get_system_dsp_num_buffers);
    ClassDB::bind_method(D_METHOD("update"), &Fmod::update);

    ADD_SIGNAL(MethodInfo("timeline_beat", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("timeline_marker", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_played", PropertyInfo(Variant::DICTIONARY, "params")));
    ADD_SIGNAL(MethodInfo("sound_stopped", PropertyInfo(Variant::DICTIONARY, "params")));

    REGISTER_ALL_CONSTANTS
}

bool Fmod::checkErrors(FMOD_RESULT result, const char* function, const char* file, int line) {
    if (result != FMOD_OK) {
        UtilityFunctions::push_error(FMOD_ErrorString(result), function, file, line);
        return false;
    }
    return true;
}

void Fmod::init(int numOfChannels, const unsigned int studioFlag, const unsigned int flag) {
    // initialize FMOD Studio and FMOD Core System with provided flags
    if (system == nullptr && coreSystem == nullptr) {
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }

    if (ERROR_CHECK(system->initialize(numOfChannels, studioFlag, flag, nullptr))) {
        isInitialized = true;
        GODOT_LOG(0, "FMOD Sound System: Successfully initialized")
        if (studioFlag == FMOD_STUDIO_INIT_LIVEUPDATE) {
            GODOT_LOG(0, "FMOD Sound System: Live update enabled!")
        }
    }

#ifdef CUSTOM_FILESYSTEM
    if (ERROR_CHECK(coreSystem->setFileSystem(
                &Callbacks::godotFileOpen, &Callbacks::godotFileClose, nullptr, nullptr, &Callbacks::godotSyncRead, &Callbacks::godotSyncCancel, -1))) {
        GODOT_LOG(0, "Custom File System enabled.")
    }
#endif
}

void Fmod::update() {
    if (!isInitialized) {
        if (!isNotinitPrinted) {
            GODOT_LOG(2, "FMOD Sound System: Fmod should be initialized before calling update")
            isNotinitPrinted = true;
        }
        return;
    }

    // Check if bank are loaded, load buses, vca and event descriptions.
    _check_loading_banks();

    for (int i = 0; i < events.size(); i++) {
        FMOD::Studio::EventInstance* eventInstance = events.get(i);
        if (eventInstance) {
            EventInfo* eventInfo = _get_event_info(eventInstance);
            if (eventInfo) {
                if (eventInfo->gameObj) {
                    if (_is_dead(eventInfo->gameObj)) {
                        FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
                        ERROR_CHECK(eventInstance->stop(m));
                        _release_one_event(eventInstance);
                        i--;
                        continue;
                    }
                    if (eventInfo->isOneShot) {
                        FMOD_STUDIO_PLAYBACK_STATE s;
                        ERROR_CHECK(eventInstance->getPlaybackState(&s));
                        if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
                            _release_one_event(eventInstance);
                            i--;
                            continue;
                        }
                    }
                    _update_instance_3d_attributes(eventInstance, eventInfo->gameObj);
                }
            } else {
                GODOT_LOG(2, "A managed event doesn't have an EventInfoStructure")
            }
        }
    }

    for (int i = 0; i < channels.size(); i++) {
        FMOD::Channel* channel = channels.get(i);
        if (channel && !_is_channel_valid(channel)) {
            channels.erase(channel);
            i--;
        }
    }

    // update listener position
    _set_listener_attributes();

    // run callback events
    _run_callbacks();

    // dispatch update to FMOD
    ERROR_CHECK(system->update());
}

void Fmod::_check_loading_banks() {
    for (int i = 0; i < loadingBanks.size(); i++) {
        auto loadingBank = loadingBanks.pop_front_value();
        auto bank = loadingBank->bank;
        FMOD_STUDIO_LOADING_STATE* loading_state = nullptr;
        ERROR_CHECK(bank->getLoadingState(loading_state));
        if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _load_bank_data(loadingBank);
            delete loadingBank;
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADING) {
            loadingBanks.push_back_value(loadingBank);
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            bank->unload();
            delete loadingBank;
            GODOT_LOG(2, "Fmod Sound System: Error loading bank.")
        }
    }
}

void Fmod::_set_listener_attributes() {
    if (actualListenerNumber == 0) {
        if (listenerWarning) {
            GODOT_LOG(1, "FMOD Sound System: No listeners are set!")
            listenerWarning = false;
        }
        return;
    }

    for (int i = 0; i < systemListenerNumber; i++) {
        Listener* listener = &listeners[i];
        if (listener->listenerLock) {
            continue;
        }
        if (_is_dead(listener->gameObj)) {
            listener->gameObj = nullptr;
            ERROR_CHECK(system->setListenerWeight(i, 0));
            continue;
        }

        Node* node {Object::cast_to<Node>(listener->gameObj)};
        if (!node->is_inside_tree()) {
            return;
        }

        if (auto* ci {Node::cast_to<CanvasItem>(node)}) {
            auto attr = _get_3d_attributes_from_transform_2d(ci->get_global_transform());
            ERROR_CHECK(system->setListenerAttributes(i, &attr));
            continue;
        }

        if (auto* s {Node::cast_to<Node3D>(node)}) {
            auto attr = _get_3d_attributes_from_transform(s->get_global_transform());
            ERROR_CHECK(system->setListenerAttributes(i, &attr));
            continue;
        }
    }
}

FMOD_VECTOR Fmod::_to_fmod_vector(Vector3& vec) {
    FMOD_VECTOR fv;
    fv.x = vec.x;
    fv.y = vec.y;
    fv.z = vec.z;
    return fv;
}

FMOD_3D_ATTRIBUTES Fmod::_get_3d_attributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward, const FMOD_VECTOR& vel) {
    FMOD_3D_ATTRIBUTES
    f3d;
    f3d.forward = forward;
    f3d.position = pos;
    f3d.up = up;
    f3d.velocity = vel;
    return f3d;
}

FMOD_3D_ATTRIBUTES Fmod::_get_3d_attributes_from_transform(const Transform3D& transform) const {
    Vector3 pos = transform.get_origin() / distanceScale;
    Vector3 up = transform.get_basis().rows[1];
    Vector3 forward = transform.get_basis().rows[2];
    Vector3 vel(0, 0, 0);
    return _get_3d_attributes(_to_fmod_vector(pos), _to_fmod_vector(up), _to_fmod_vector(forward), _to_fmod_vector(vel));
}

FMOD_3D_ATTRIBUTES Fmod::_get_3d_attributes_from_transform_2d(const Transform2D& transform) const {
    Vector2 posVector = transform.get_origin() / distanceScale;
    Vector3 pos(posVector.x, 0.0f, posVector.y);
    Vector3 up(0, 1, 0);
    Vector3 forward = Vector3(transform.columns[1].x, 0, transform.columns[1].y).normalized();
    Vector3 vel(0, 0, 0);// TODO: add doppler
    const FMOD_VECTOR& posFmodVector = _to_fmod_vector(pos);
    return _get_3d_attributes(posFmodVector, _to_fmod_vector(up), _to_fmod_vector(forward), _to_fmod_vector(vel));
}

Dictionary Fmod::_get_transform_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attr) const {
    Dictionary _3Dattr;
    Transform3D transform;
    transform.origin = Vector3(attr.position.x, attr.position.y, attr.position.z) * distanceScale;
    const Vector3& upVector = Vector3(attr.up.x, attr.up.y, attr.up.z);
    transform.basis.rows[1] = upVector;
    const Vector3& forwardVector = Vector3(attr.forward.x, attr.forward.y, attr.forward.z);
    transform.basis.rows[2] = forwardVector;
    transform.basis.rows[0] = upVector.cross(forwardVector);
    Vector3 velocity(attr.velocity.x, attr.velocity.y, attr.velocity.z);
    _3Dattr["transform"] = transform;
    _3Dattr["velocity"] = velocity;
    return _3Dattr;
}

Dictionary Fmod::_get_transform_2d_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attr) const {
    Dictionary _2Dattr;
    Transform2D transform;
    transform.set_origin(Vector2(attr.position.x, attr.position.z) * distanceScale);
    const Vector2& forward = Vector2(attr.forward.x, attr.forward.z);
    transform.columns[1] = forward;
    transform.columns[0] = Vector2(forward.y, -forward.x);
    Vector2 velocity(attr.velocity.x, attr.velocity.z);
    _2Dattr["transform"] = transform;
    _2Dattr["velocity"] = velocity;
    return _2Dattr;
}

bool Fmod::_is_dead(Object* node) {
    if (!node) {
        return true;
    }
    return !UtilityFunctions::is_instance_valid(Object::cast_to<Node>(node)->get_owner());
}

bool Fmod::_is_fmod_valid(Object* node) {
    if (node) {
        bool ret = Node::cast_to<Node3D>(node) || Node::cast_to<CanvasItem>(node);
        if (!ret) {
            GODOT_LOG(2, "Invalid Object. A listener has to be either a Node3D or CanvasItem.")
        }
        return ret;
    }
    GODOT_LOG(2, "Object is null")
    return false;
}

void Fmod::_update_instance_3d_attributes(FMOD::Studio::EventInstance* instance, Object* node) {
    // try to set 3D attributes
    if (instance && _is_fmod_valid(node) && Object::cast_to<Node>(node)->is_inside_tree()) {
        if (auto* ci {Node::cast_to<CanvasItem>(node)}) {
            auto attr = _get_3d_attributes_from_transform_2d(ci->get_global_transform());
            ERROR_CHECK(instance->set3DAttributes(&attr));
            return;
        }
        if (auto* s {Node::cast_to<Node3D>(node)}) {
            auto attr = _get_3d_attributes_from_transform(s->get_global_transform());
            ERROR_CHECK(instance->set3DAttributes(&attr));
            return;
        }
    }
}

void Fmod::shutdown() {
    isInitialized = false;
    isNotinitPrinted = false;
    ERROR_CHECK(system->unloadAll());
    ERROR_CHECK(system->release());
    system = nullptr;
    coreSystem = nullptr;
    GODOT_LOG(0, "FMOD Sound System: System released")
}

void Fmod::set_system_listener_number(int p_listenerNumber) {
    if (p_listenerNumber > 0 && p_listenerNumber <= FMOD_MAX_LISTENERS) {
        if (ERROR_CHECK(system->setNumListeners(p_listenerNumber))) {
            systemListenerNumber = p_listenerNumber;
        }
    } else {
        GODOT_LOG(2, "Number of listeners must be set between 1 and 8")
    }
}

void Fmod::add_listener(int index, Object* gameObj) {
    if (!_is_fmod_valid(gameObj)) {
        return;
    }
    if (index >= 0 && index < systemListenerNumber) {
        Listener* listener = &listeners[index];
        listener->gameObj = gameObj;
        ERROR_CHECK(system->setListenerWeight(index, listener->weight));
        int count = 0;
        for (int i = 0; i < systemListenerNumber; i++) {
            if ((&listeners[i])->gameObj != nullptr) count++;
        }
        actualListenerNumber = count;
        if (actualListenerNumber > 0) listenerWarning = true;
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

void Fmod::remove_listener(int index) {
    if (index >= 0 && index < systemListenerNumber) {
        Listener* listener = &listeners[index];
        listener->gameObj = nullptr;
        ERROR_CHECK(system->setListenerWeight(index, 0));
        int count = 0;
        for (int i = 0; i < systemListenerNumber; i++) {
            if ((&listeners[i])->gameObj != nullptr) count++;
        }
        actualListenerNumber = count;
        if (actualListenerNumber > 0) listenerWarning = true;
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

int Fmod::get_system_listener_number() const {
    return systemListenerNumber;
}

float Fmod::get_system_listener_weight(const int index) {
    if (index >= 0 && index < systemListenerNumber) {
        float weight = 0;
        ERROR_CHECK(system->getListenerWeight(index, &weight));
        listeners[index].weight = weight;
        return weight;
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
        return 0;
    }
}

void Fmod::set_system_listener_weight(const int index, float weight) {
    if (index >= 0 && index < systemListenerNumber) {
        listeners[index].weight = weight;
        ERROR_CHECK(system->setListenerWeight(index, weight));
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

Dictionary Fmod::get_system_listener_3d_attributes(const int index) {
    Dictionary _3Dattr;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES
        attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        _3Dattr = _get_transform_info_from_3d_attribut(attr);
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
    return _3Dattr;
}

Dictionary Fmod::get_system_listener_2d_attributes(int index) {
    Dictionary _2Dattr;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES
        attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        _2Dattr = _get_transform_2d_info_from_3d_attribut(attr);
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
    return _2Dattr;
}

void Fmod::set_system_listener_3d_attributes(int index, const Transform3D& transform) {
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES
        attr = _get_3d_attributes_from_transform(transform);
        ERROR_CHECK(system->setListenerAttributes(index, &attr));
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

void Fmod::set_system_listener_2d_attributes(int index, const Transform2D& transform) {
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES
        attr = _get_3d_attributes_from_transform_2d(transform);
        ERROR_CHECK(system->setListenerAttributes(index, &attr));
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

void Fmod::set_listener_lock(int index, bool isLocked) {
    if (index >= 0 && index < systemListenerNumber) {
        listeners[index].listenerLock = isLocked;
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
    }
}

bool Fmod::get_listener_lock(int index) {
    if (index >= 0 && index < systemListenerNumber) {
        return listeners[index].listenerLock;
    } else {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
        return false;
    }
}

Object* Fmod::get_object_attached_to_listener(int index) {
    if (index < 0 || index >= systemListenerNumber) {
        GODOT_LOG(2, "index of listeners must be set between 0 and the number of listeners set")
        return nullptr;
    } else {
        Object* node = listeners[index].gameObj;
        if (!node) {
            GODOT_LOG(1, "No node was set on listener")
        }
        return node;
    }
}

void Fmod::set_software_format(int sampleRate, const int speakerMode, int numRawSpeakers) {
    if (system == nullptr && coreSystem == nullptr) {
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }
    ERROR_CHECK(coreSystem->setSoftwareFormat(sampleRate, static_cast<FMOD_SPEAKERMODE>(speakerMode), numRawSpeakers));
}

String Fmod::load_bank(const String& pathToBank, unsigned int flag) {
    DRIVE_PATH(pathToBank)
    if (banks.has(pathToBank)) return pathToBank;// bank is already loaded
    FMOD::Studio::Bank* bank = nullptr;
    ERROR_CHECK(system->loadBankFile(pathToBank.utf8().get_data(), flag, &bank));
    if (bank) {
        GODOT_LOG(0, "FMOD Sound System: LOADING BANK " + String(pathToBank))
        auto* loadingBank = new LoadingBank();
        loadingBank->bank = bank;
        loadingBank->godotPath = pathToBank;
        if (flag != FMOD_STUDIO_LOAD_BANK_NONBLOCKING) {
            _load_bank_data(loadingBank);
        } else {
            loadingBanks.append(loadingBank);
        }
    }
    return pathToBank;
}

void Fmod::unload_bank(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks)
    _unload_all_buses(instance);
    _unload_all_vca(instance);
    _unload_all_event_descriptions(instance);
    ERROR_CHECK(instance->unload());
    GODOT_LOG(0, "FMOD Sound System: BANK " + String(pathToBank) + " UNLOADED")
    banks.erase(pathToBank);
}

int Fmod::get_bank_loading_state(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    FMOD_STUDIO_LOADING_STATE state;
    ERROR_CHECK(instance->getLoadingState(&state));
    return state;
}

int Fmod::get_bank_bus_count(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getBusCount(&count));
    return count;
}

int Fmod::get_bank_event_count(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getEventCount(&count));
    return count;
}

int Fmod::get_bank_string_count(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getStringCount(&count));
    return count;
}

int Fmod::get_bank_vca_count(const String& pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getVCACount(&count));
    return count;
}

uint64_t Fmod::create_event_instance(const String& eventPath) {
    FMOD::Studio::EventInstance* instance = _create_instance(eventPath, false, nullptr);
    if (instance) {
        return (uint64_t) instance;
    }
    return 0;
}

float Fmod::get_event_parameter_by_name(uint64_t instanceId, const String& parameterName) {
    float p = -1;
    FIND_AND_CHECK(instanceId, events, p)
    ERROR_CHECK(instance->getParameterByName(parameterName.utf8().get_data(), &p));
    return p;
}

void Fmod::set_event_parameter_by_name(uint64_t instanceId, const String& parameterName, float value) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setParameterByName(parameterName.utf8().get_data(), value));
}

float Fmod::get_event_parameter_by_id(uint64_t instanceId, const Array& idPair) {
    float value = -1.0f;
    FIND_AND_CHECK(instanceId, events, value)
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->getParameterByID(id, &value));
    return value;
}

void Fmod::set_event_parameter_by_id(uint64_t instanceId, const Array& idPair, float value) {
    FIND_AND_CHECK(instanceId, events)
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->setParameterByID(id, value));
}

void Fmod::release_event(uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    _release_one_event(instance);
}

void Fmod::_release_one_event(FMOD::Studio::EventInstance* eventInstance) {
    std::lock_guard<std::mutex> lk(Callbacks::callback_mut);
    EventInfo* eventInfo = _get_event_info(eventInstance);
    eventInstance->setUserData(nullptr);
    ERROR_CHECK(eventInstance->release());
    events.erase(eventInstance);
    delete eventInfo;
}

void Fmod::start_event(uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->start());
}

void Fmod::stop_event(uint64_t instanceId, int stopMode) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void Fmod::event_key_off(uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->keyOff());
}

int Fmod::get_event_playback_state(uint64_t instanceId) {
    int s = -1;
    FIND_AND_CHECK(instanceId, events, s)
    ERROR_CHECK(instance->getPlaybackState((FMOD_STUDIO_PLAYBACK_STATE*) &s));
    return s;
}

bool Fmod::get_event_paused(uint64_t instanceId) {
    bool paused = false;
    FIND_AND_CHECK(instanceId, events, paused)
    ERROR_CHECK(instance->getPaused(&paused));
    return paused;
}

void Fmod::set_event_paused(uint64_t instanceId, bool paused) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setPaused(paused));
}

float Fmod::get_event_pitch(uint64_t instanceId) {
    float pitch = 0.0f;
    FIND_AND_CHECK(instanceId, events, pitch)
    ERROR_CHECK(instance->getPitch(&pitch));
    return pitch;
}

void Fmod::set_event_pitch(uint64_t instanceId, float pitch) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setPitch(pitch));
}

float Fmod::get_event_volume(uint64_t instanceId) {
    float volume = 0.0f;
    FIND_AND_CHECK(instanceId, events, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::set_event_volume(uint64_t instanceId, float volume) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setVolume(volume));
}

int Fmod::get_event_timeline_position(uint64_t instanceId) {
    int tp = 0;
    FIND_AND_CHECK(instanceId, events, tp)
    ERROR_CHECK(instance->getTimelinePosition(&tp));
    return tp;
}

void Fmod::set_event_timeline_position(uint64_t instanceId, int position) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setTimelinePosition(position));
}

float Fmod::get_event_reverb_level(uint64_t instanceId, int index) {
    float rvl = 0.0f;
    FIND_AND_CHECK(instanceId, events, rvl)
    ERROR_CHECK(instance->getReverbLevel(index, &rvl));
    return rvl;
}

void Fmod::set_event_reverb_level(uint64_t instanceId, int index, float level) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setReverbLevel(index, level));
}

bool Fmod::is_event_virtual(uint64_t instanceId) {
    bool v = false;
    FIND_AND_CHECK(instanceId, events, v)
    ERROR_CHECK(instance->isVirtual(&v));
    return v;
}

void Fmod::set_event_listener_mask(uint64_t instanceId, unsigned int mask) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setListenerMask(mask));
}

uint32_t Fmod::get_event_listener_mask(uint64_t instanceId) {
    uint32_t mask = 0;
    FIND_AND_CHECK(instanceId, events, mask)
    ERROR_CHECK(instance->getListenerMask(&mask));
    return mask;
}

void Fmod::set_event_2d_attributes(uint64_t instanceId, Transform2D position) {
    FIND_AND_CHECK(instanceId, events)
    auto attr = _get_3d_attributes_from_transform_2d(position);
    ERROR_CHECK(instance->set3DAttributes(&attr));
}

Dictionary Fmod::get_event_2d_attributes(uint64_t instanceId) {
    Dictionary _2Dattr;
    FIND_AND_CHECK(instanceId, events, _2Dattr)
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(instance->get3DAttributes(&attr));
    _2Dattr = _get_transform_2d_info_from_3d_attribut(attr);
    return _2Dattr;
}

void Fmod::set_event_3d_attributes(uint64_t instanceId, const Transform3D& transform) {
    FIND_AND_CHECK(instanceId, events)
    auto attr = _get_3d_attributes_from_transform(transform);
    ERROR_CHECK(instance->set3DAttributes(&attr));
}

Dictionary Fmod::get_event_3d_attributes(uint64_t instanceId) {
    Dictionary _3Dattr;
    FIND_AND_CHECK(instanceId, events, _3Dattr)
    FMOD_3D_ATTRIBUTES
    attr;
    ERROR_CHECK(instance->get3DAttributes(&attr));
    _3Dattr = _get_transform_info_from_3d_attribut(attr);
    return _3Dattr;
}

int Fmod::desc_get_length(const String& eventPath) {
    int length = -1;
    FIND_AND_CHECK(eventPath, eventDescriptions, length)
    ERROR_CHECK(instance->getLength(&length));
    return length;
}

Array Fmod::desc_get_instance_list(const String& eventPath) {
    Array array;
    FIND_AND_CHECK(eventPath, eventDescriptions, array)
    FMOD::Studio::EventInstance* instances[MAX_EVENT_INSTANCE];
    int count = 0;
    ERROR_CHECK(instance->getInstanceList(instances, MAX_EVENT_INSTANCE, &count));
    CHECK_SIZE(MAX_EVENT_INSTANCE, count, events)
    for (int i = 0; i < count; i++) {
        array.append((uint64_t) instances[i]);
    }
    return array;
}

int Fmod::desc_get_instance_count(const String& eventPath) {
    int count = -1;
    FIND_AND_CHECK(eventPath, eventDescriptions, count)
    ERROR_CHECK(instance->getInstanceCount(&count));
    return count;
}

void Fmod::desc_release_all_instances(const String& eventPath) {
    std::lock_guard<std::mutex> lk(Callbacks::callback_mut);
    FIND_AND_CHECK(eventPath, eventDescriptions)
    FMOD::Studio::EventInstance* instances[MAX_EVENT_INSTANCE];
    int count = 0;
    ERROR_CHECK(instance->getInstanceList(instances, MAX_EVENT_INSTANCE, &count));
    CHECK_SIZE(MAX_EVENT_INSTANCE, count, events)
    for (int i = 0; i < count; i++) {
        FMOD::Studio::EventInstance* it = instances[i];
        if (events.has(it)) {
            EventInfo* eventInfo = _get_event_info(it);
            it->setUserData(nullptr);
            events.erase(it);
            delete eventInfo;
        }
    }
    ERROR_CHECK(instance->releaseAllInstances());
}

void Fmod::desc_load_sample_data(const String& eventPath) {
    FIND_AND_CHECK(eventPath, eventDescriptions)
    ERROR_CHECK(instance->loadSampleData());
}

void Fmod::desc_unload_sample_data(const String& eventPath) {
    FIND_AND_CHECK(eventPath, eventDescriptions)
    ERROR_CHECK(instance->unloadSampleData());
}

int Fmod::desc_get_sample_loading_state(const String& eventPath) {
    FIND_AND_CHECK(eventPath, eventDescriptions, -1)
    FMOD_STUDIO_LOADING_STATE s;
    ERROR_CHECK(instance->getSampleLoadingState(&s));
    return s;
}

bool Fmod::desc_is_3d(const String& eventPath) {
    bool is3D = false;
    FIND_AND_CHECK(eventPath, eventDescriptions, is3D)
    ERROR_CHECK(instance->is3D(&is3D));
    return is3D;
}

bool Fmod::desc_is_one_shot(const String& eventPath) {
    bool isOneShot = false;
    FIND_AND_CHECK(eventPath, eventDescriptions, isOneShot)
    ERROR_CHECK(instance->isOneshot(&isOneShot));
    return isOneShot;
}

bool Fmod::desc_is_snapshot(const String& eventPath) {
    bool isSnapshot = false;
    FIND_AND_CHECK(eventPath, eventDescriptions, isSnapshot)
    ERROR_CHECK(instance->isSnapshot(&isSnapshot));
    return isSnapshot;
}

bool Fmod::desc_is_stream(const String& eventPath) {
    bool isStream = false;
    FIND_AND_CHECK(eventPath, eventDescriptions, isStream)
    ERROR_CHECK(instance->isStream(&isStream));
    return isStream;
}

bool Fmod::desc_has_sustain_point(const String& eventPath) {
    bool hasSustainPoint = false;
    FIND_AND_CHECK(eventPath, eventDescriptions, hasSustainPoint)
    ERROR_CHECK(instance->hasSustainPoint(&hasSustainPoint));
    return hasSustainPoint;
}

Array Fmod::desc_get_min_max_distance(const String& eventPath) {
    float minDistance;
    float maxDistance;
    Array ret;

    FIND_AND_CHECK(eventPath, eventDescriptions, ret)
    ERROR_CHECK(instance->getMinMaxDistance(&minDistance, &maxDistance));
    ret.append(minDistance);
    ret.append(maxDistance);
    return ret;
}

float Fmod::desc_get_sound_size(const String& eventPath) {
    float soundSize = 0.f;
    FIND_AND_CHECK(eventPath, eventDescriptions, soundSize)
    ERROR_CHECK(instance->getSoundSize(&soundSize));
    return soundSize;
}

Dictionary Fmod::desc_get_parameter_description_by_name(const String& eventPath, const String& name) {
    Dictionary paramDesc;
    FIND_AND_CHECK(eventPath, eventDescriptions, paramDesc)
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(instance->getParameterDescriptionByName(name.utf8().get_data(), &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

Dictionary Fmod::desc_get_parameter_description_by_id(const String& eventPath, const Array& idPair) {
    Dictionary paramDesc;
    FIND_AND_CHECK(eventPath, eventDescriptions, paramDesc)
    FMOD_STUDIO_PARAMETER_ID paramId;
    paramId.data1 = (unsigned int) idPair[0];
    paramId.data2 = (unsigned int) idPair[1];
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(instance->getParameterDescriptionByID(paramId, &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

int Fmod::desc_get_parameter_description_count(const String& eventPath) {
    int count = 0;
    FIND_AND_CHECK(eventPath, eventDescriptions, count)
    ERROR_CHECK(instance->getParameterDescriptionCount(&count));
    return count;
}

Dictionary Fmod::desc_get_parameter_description_by_index(const String& eventPath, int index) {
    Dictionary paramDesc;
    FIND_AND_CHECK(eventPath, eventDescriptions, paramDesc)
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(instance->getParameterDescriptionByIndex(index, &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

Dictionary Fmod::desc_get_user_property(const String& eventPath, const String& name) {
    Dictionary propDesc;
    FIND_AND_CHECK(eventPath, eventDescriptions, propDesc)
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK(instance->getUserProperty(name.utf8().get_data(), &uProp))) {
        FMOD_STUDIO_USER_PROPERTY_TYPE fType = uProp.type;
        if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER) propDesc[String(uProp.name)] = uProp.intvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN)
            propDesc[String(uProp.name)] = (bool) uProp.boolvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT)
            propDesc[String(uProp.name)] = uProp.floatvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_STRING)
            propDesc[String(uProp.name)] = String(uProp.stringvalue);
    }
    return propDesc;
}

int Fmod::desc_get_user_property_count(const String& eventPath) {
    int count = 0;
    FIND_AND_CHECK(eventPath, eventDescriptions, count)
    ERROR_CHECK(instance->getUserPropertyCount(&count));
    return count;
}

Dictionary Fmod::desc_user_property_by_index(const String& eventPath, int index) {
    Dictionary propDesc;
    FIND_AND_CHECK(eventPath, eventDescriptions, propDesc)
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK(instance->getUserPropertyByIndex(index, &uProp))) {
        FMOD_STUDIO_USER_PROPERTY_TYPE fType = uProp.type;
        if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER) propDesc[String(uProp.name)] = uProp.intvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN)
            propDesc[String(uProp.name)] = (bool) uProp.boolvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT)
            propDesc[String(uProp.name)] = uProp.floatvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_STRING)
            propDesc[String(uProp.name)] = String(uProp.stringvalue);
    }

    return propDesc;
}

bool Fmod::get_bus_mute(const String& busPath) {
    bool mute = false;
    FIND_AND_CHECK(busPath, buses, mute)
    ERROR_CHECK(instance->getMute(&mute));
    return mute;
}

bool Fmod::get_bus_paused(const String& busPath) {
    bool paused = false;
    FIND_AND_CHECK(busPath, buses, paused)
    ERROR_CHECK(instance->getPaused(&paused));
    return paused;
}

float Fmod::get_bus_volume(const String& busPath) {
    float volume = 0.0f;
    FIND_AND_CHECK(busPath, buses, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::set_bus_mute(const String& busPath, bool mute) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setMute(mute));
}

void Fmod::set_bus_paused(const String& busPath, bool paused) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setPaused(paused));
}

void Fmod::set_bus_volume(const String& busPath, float volume) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setVolume(volume));
}

void Fmod::stop_all_bus_events(const String& busPath, int stopMode) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void Fmod::_load_bank_data(LoadingBank* loadingBank) {
    char path[MAX_PATH_SIZE];
    auto bank = loadingBank->bank;
    FMOD_RESULT result = bank->getPath(path, MAX_PATH_SIZE, nullptr);
    if (result == FMOD_OK) {
        GODOT_LOG(0, "FMOD Sound System: BANK " + String(path) + " LOADED")
        _load_all_buses(bank);
        _load_all_vca(bank);
        _load_all_event_descriptions(bank);
        banks[loadingBank->godotPath] << bank;
    } else {
        if (result == FMOD_ERR_EVENT_NOTFOUND) {
            GODOT_LOG(0, "FMOD Sound System: BANK " + String(path) + " COULDN'T BE LOADED. Path incorrect or MasterBank not loaded yet.")
        } else {
            ERROR_CHECK(result);
        }
        bank->unload();
    }
}

void Fmod::_load_all_vca(FMOD::Studio::Bank* bank) {
    FMOD::Studio::VCA* array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to VCAs")
            VCAs[path] << vca;
        }
    }
}

void Fmod::_load_all_buses(FMOD::Studio::Bank* bank) {
    FMOD::Studio::Bus* array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to buses")
            buses[path] << bus;
        }
    }
}

void Fmod::_load_all_event_descriptions(FMOD::Studio::Bank* bank) {
    FMOD::Studio::EventDescription* array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to eventDescriptions")
            eventDescriptions[path] << eventDescription;
        }
    }
}

void Fmod::_unload_all_vca(FMOD::Studio::Bank* bank) {
    FMOD::Studio::VCA* array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from VCAs")
            VCAs.erase(path);
        }
    }
}

void Fmod::_unload_all_buses(FMOD::Studio::Bank* bank) {
    FMOD::Studio::Bus* array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from buses")
            buses.erase(path);
        }
    }
}

void Fmod::_unload_all_event_descriptions(FMOD::Studio::Bank* bank) {
    FMOD::Studio::EventDescription* array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from eventDescriptions")
            eventDescriptions.erase(path);
        }
    }
}

bool Fmod::check_vca_path(const String& vcaPath) {
    return VCAs.has(vcaPath);
}

bool Fmod::check_bus_path(const String& busPath) {
    return buses.has(busPath);
}

bool Fmod::check_event_path(const String& eventPath) {
    return eventDescriptions.has(eventPath);
}

FMOD::Studio::EventInstance* Fmod::_create_instance(const String& eventName, bool isOneShot, Object* gameObject) {
    FIND_AND_CHECK(eventName, eventDescriptions, nullptr)
    FMOD::Studio::EventInstance* eventInstance = nullptr;
    ERROR_CHECK(instance->createInstance(&eventInstance));
    if (eventInstance && (!isOneShot || gameObject)) {
        auto* eventInfo = new EventInfo();
        eventInfo->gameObj = gameObject;
        eventInfo->isOneShot = isOneShot;
        eventInstance->setUserData(eventInfo);
        events.append(eventInstance);
    }
    return eventInstance;
}

EventInfo* Fmod::_get_event_info(FMOD::Studio::EventInstance* eventInstance) {
    EventInfo* eventInfo;
    ERROR_CHECK(eventInstance->getUserData((void**) &eventInfo));
    return eventInfo;
}

void Fmod::play_one_shot(const String& eventName, Object* gameObj) {
    FMOD::Studio::EventInstance* instance = _create_instance(eventName, true, nullptr);
    if (instance) {
        // set 3D attributes once
        if (_is_fmod_valid(gameObj)) {
            _update_instance_3d_attributes(instance, gameObj);
        }
        ERROR_CHECK(instance->start());
        ERROR_CHECK(instance->release());
    }
}

void Fmod::play_one_shot_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters) {
    FMOD::Studio::EventInstance* instance = _create_instance(eventName, true, nullptr);
    if (instance) {
        // set 3D attributes once
        if (_is_fmod_valid(gameObj)) {
            _update_instance_3d_attributes(instance, gameObj);
        }
        // set the initial parameter values
        auto keys = parameters.keys();
        for (int i = 0; i < keys.size(); i++) {
            String k = keys[i];
            float v = parameters[keys[i]];
            ERROR_CHECK(instance->setParameterByName(k.utf8().get_data(), v));
        }
        ERROR_CHECK(instance->start());
        ERROR_CHECK(instance->release());
    }
}

void Fmod::play_one_shot_attached(const String& eventName, Object* gameObj) {
    if (_is_fmod_valid(gameObj)) {
        FMOD::Studio::EventInstance* instance = _create_instance(eventName, true, gameObj);
        if (instance) {
            ERROR_CHECK(instance->start());
        }
    }
}

void Fmod::play_one_shot_attached_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters) {
    if (_is_fmod_valid(gameObj)) {
        FMOD::Studio::EventInstance* instance = _create_instance(eventName, true, gameObj);
        if (instance) {
            // set the initial parameter values
            auto keys = parameters.keys();
            for (int i = 0; i < keys.size(); i++) {
                String k = keys[i];
                float v = parameters[keys[i]];
                ERROR_CHECK(instance->setParameterByName(k.utf8().get_data(), v));
            }
            ERROR_CHECK(instance->start());
        }
    }
}

void Fmod::attach_instance_to_node(uint64_t instanceId, Object* gameObj) {
    if (!_is_fmod_valid(gameObj)) {
        GODOT_LOG(1, "Trying to attach event instance to null game object or object is not Node3D or CanvasItem")
        return;
    }
    FIND_AND_CHECK(instanceId, events)
    _get_event_info(instance)->gameObj = gameObj;
}

void Fmod::detach_instance_from_node(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    _get_event_info(instance)->gameObj = nullptr;
}

Object* Fmod::get_object_attached_to_instance(uint64_t instanceId) {
    Object* node = nullptr;
    FIND_AND_CHECK(instanceId, events, node)
    EventInfo* eventInfo = _get_event_info(instance);
    if (eventInfo) {
        node = eventInfo->gameObj;
        if (!node) {
            GODOT_LOG(1, "There is no node attached to event instance.")
        }
    }
    return node;
}

void Fmod::set_system_dsp_buffer_size(unsigned int bufferlength, int numbuffers) {
    if (bufferlength > 0 && numbuffers > 0 && ERROR_CHECK(coreSystem->setDSPBufferSize(bufferlength, numbuffers))) {
        GODOT_LOG(0, "FMOD Sound System: Successfully set DSP buffer size")
    } else {
        GODOT_LOG(2, "FMOD Sound System: Failed to set DSP buffer size :|")
    }
}

Array Fmod::get_system_dsp_buffer_size() {
    unsigned int bufferlength;
    int numbuffers;
    Array a;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferlength, &numbuffers));
    a.append(bufferlength);
    a.append(numbuffers);
    return a;
}

unsigned int Fmod::get_system_dsp_buffer_length() {
    unsigned int bufferlength;
    int numbuffers;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferlength, &numbuffers));
    return bufferlength;
}

int Fmod::get_system_dsp_num_buffers() {
    unsigned int bufferlength;
    int numbuffers;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferlength, &numbuffers));
    return numbuffers;
}

void Fmod::pause_all_events(const bool pause) {
    for (int i = 0; i < events.size(); i++) {
        auto eventInstance = events.get(i);
        if (eventInstance) {
            ERROR_CHECK(eventInstance->setPaused(pause));
        }
    }
}

void Fmod::mute_all_events() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus* masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(true);
        }
    }
}

void Fmod::unmute_all_events() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus* masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(false);
        }
    }
}

bool Fmod::banks_still_loading() {
    const godot::Array keys = banks.keys();

    for (int i = 0; i < banks.size(); i++) {
        godot::String key = keys[i];
        FMOD::Studio::Bank* bank = banks.get(key);
        FMOD_STUDIO_LOADING_STATE loadingState;
        ERROR_CHECK(bank->getLoadingState(&loadingState));
        if (loadingState == FMOD_STUDIO_LOADING_STATE_LOADING) {
            return true;
        }
    }
    return false;
}

float Fmod::get_vca_volume(const String& VCAPath) {
    float volume = 0.0f;
    FIND_AND_CHECK(VCAPath, VCAs, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::set_vca_volume(const String& VCAPath, float volume) {
    FIND_AND_CHECK(VCAPath, VCAs)
    ERROR_CHECK(instance->setVolume(volume));
}

void Fmod::load_file_as_sound(const String& path) {
    DRIVE_PATH(path)
    FMOD::Sound* sound = sounds.get(path);
    if (!sound) {
        ERROR_CHECK(coreSystem->createSound(path.utf8().get_data(), FMOD_CREATESAMPLE, nullptr, &sound));
        if (sound) {
            sounds[path] << sound;
            UtilityFunctions::print("FMOD Sound System: LOADING AS SOUND FILE" + String(path));
        }
    }
}

void Fmod::load_file_as_music(const String& path) {
    DRIVE_PATH(path)
    FMOD::Sound* sound = sounds.get(path);
    if (!sound) {
        ERROR_CHECK(coreSystem->createSound(path.utf8().get_data(), (FMOD_CREATESTREAM | FMOD_LOOP_NORMAL), nullptr, &sound));
        if (sound) {
            sounds[path] << sound;
            UtilityFunctions::print("FMOD Sound System: LOADING AS MUSIC FILE" + String(path));
        }
    }
}

void Fmod::unload_file(const String& path) {
    DRIVE_PATH(path)
    FIND_AND_CHECK(path, sounds)
    ERROR_CHECK(instance->release());
    sounds.erase(path);
    UtilityFunctions::print("FMOD Sound System: UNLOADING FILE" + String(path));
}

uint64_t Fmod::create_sound_instance(const String& path) {
    DRIVE_PATH(path)
    FIND_AND_CHECK(path, sounds, 0)
    FMOD::Channel* channel = nullptr;
    ERROR_CHECK(coreSystem->playSound(instance, nullptr, true, &channel));
    if (channel) {
        channels.append(channel);
        return (uint64_t) channel;
    }
    return 0;
}

bool Fmod::check_sound_instance(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, channels, false)
    return _is_channel_valid(instance);
}

bool Fmod::_is_channel_valid(FMOD::Channel* channel) {
    bool isPlaying;
    FMOD_RESULT result = channel->isPlaying(&isPlaying);
    return result != FMOD_ERR_INVALID_HANDLE;
}

void Fmod::release_sound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->stop());
    channels.erase(instance);
}

void Fmod::play_sound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, channels)
    set_sound_paused(instanceId, false);
}

void Fmod::set_sound_paused(const uint64_t instanceId, bool paused) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setPaused(paused));
}

void Fmod::stop_sound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->stop());
}

bool Fmod::is_sound_playing(const uint64_t instanceId) {
    bool isPlaying = false;
    FIND_AND_CHECK(instanceId, channels, isPlaying)
    ERROR_CHECK(instance->isPlaying(&isPlaying));
    return isPlaying;
}

void Fmod::set_sound_volume(const uint64_t instanceId, float volume) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setVolume(volume));
}

float Fmod::get_sound_volume(const uint64_t instanceId) {
    float volume = 0.f;
    FIND_AND_CHECK(instanceId, channels, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

float Fmod::get_sound_pitch(const uint64_t instanceId) {
    float pitch = 0.f;
    FIND_AND_CHECK(instanceId, channels, pitch)
    ERROR_CHECK(instance->getPitch(&pitch));
    return pitch;
}

void Fmod::set_sound_pitch(const uint64_t instanceId, float pitch) {
    FIND_AND_CHECK(instanceId, channels)
    ERROR_CHECK(instance->setPitch(pitch));
}

void Fmod::set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale) {
    if (distanceFactor <= 0) {
        GODOT_LOG(2, "FMOD Sound System: Failed to set 3D settings - invalid distance factor!")
    } else if (ERROR_CHECK(coreSystem->set3DSettings(dopplerScale, distanceFactor, rollOffScale))) {
        distanceScale = distanceFactor;
        GODOT_LOG(0, "FMOD Sound System: Successfully set global 3D settings")
    } else {
        GODOT_LOG(2, "FMOD Sound System: Failed to set 3D settings")
    }
}

void Fmod::wait_for_all_loads() {
    ERROR_CHECK(system->flushSampleLoading());
    _check_loading_banks();
}

Array Fmod::get_available_drivers() {
    Array driverList;
    int numDrivers = 0;

    ERROR_CHECK(coreSystem->getNumDrivers(&numDrivers));

    for (int i = 0; i < numDrivers; i++) {
        char name[MAX_DRIVER_NAME_SIZE];
        int sampleRate;
        FMOD_SPEAKERMODE speakerMode;
        int speakerModeChannels;
        ERROR_CHECK(coreSystem->getDriverInfo(i, name, MAX_DRIVER_NAME_SIZE, nullptr, &sampleRate, &speakerMode, &speakerModeChannels));
        String nameStr(name);

        Dictionary driverInfo;
        driverInfo["id"] = i;
        driverInfo["name"] = nameStr;
        driverInfo["sample_rate"] = sampleRate;
        driverInfo["speaker_mode"] = (int) speakerMode;
        driverInfo["number_of_channels"] = speakerModeChannels;
        driverList.push_back(driverInfo);
    }

    return driverList;
}

int Fmod::get_driver() {
    int driverId = -1;
    ERROR_CHECK(coreSystem->getDriver(&driverId));
    return driverId;
}

void Fmod::set_driver(const int id) {
    ERROR_CHECK(coreSystem->setDriver(id));
}

Dictionary Fmod::get_performance_data() {
    // get the CPU usage
    FMOD_STUDIO_CPU_USAGE studioCpuUsage;
    FMOD_CPU_USAGE cpuUsage;
    ERROR_CHECK(system->getCPUUsage(&studioCpuUsage, &cpuUsage));
    Dictionary cpuPerfData = performanceData["CPU"];
    cpuPerfData["dsp"] = cpuUsage.dsp;
    cpuPerfData["geometry"] = cpuUsage.geometry;
    cpuPerfData["stream"] = cpuUsage.stream;
    cpuPerfData["update"] = cpuUsage.update;
    cpuPerfData["convolution1"] = cpuUsage.convolution1;
    cpuPerfData["convolution2"] = cpuUsage.convolution2;
    cpuPerfData["studio"] = studioCpuUsage.update;

    // get the memory usage
    int currentAlloc = 0;
    int maxAlloc = 0;
    ERROR_CHECK(FMOD::Memory_GetStats(&currentAlloc, &maxAlloc));
    Dictionary memPerfData = performanceData["memory"];
    memPerfData["currently_allocated"] = currentAlloc;
    memPerfData["max_allocated"] = maxAlloc;

    // get the file usage
    long long sampleBytesRead = 0;
    long long streamBytesRead = 0;
    long long otherBytesRead = 0;
    ERROR_CHECK(coreSystem->getFileUsage(&sampleBytesRead, &streamBytesRead, &otherBytesRead));
    Dictionary filePerfData = performanceData["file"];
    filePerfData["sample_bytes_read"] = static_cast<int64_t>(sampleBytesRead);
    filePerfData["stream_bytes_read"] = static_cast<int64_t>(streamBytesRead);
    filePerfData["other_bytes_read"] = static_cast<int64_t>(otherBytesRead);

    return performanceData;
}

void Fmod::set_global_parameter_by_name(const String& parameterName, float value) {
    ERROR_CHECK(system->setParameterByName(parameterName.utf8().get_data(), value));
}

float Fmod::get_global_parameter_by_name(const String& parameterName) {
    float value = 0.f;
    ERROR_CHECK(system->getParameterByName(parameterName.utf8().get_data(), &value));
    return value;
}

void Fmod::set_global_parameter_by_id(const Array& idPair, const float value) {
    if (idPair.size() != 2) {
        GODOT_LOG(2, "FMOD Sound System: Invalid parameter ID")
        return;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(system->setParameterByID(id, value));
}

float Fmod::get_global_parameter_by_id(const Array& idPair) {
    if (idPair.size() != 2) {
        GODOT_LOG(2, "FMOD Sound System: Invalid parameter ID")
        return -1.f;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    float value = -1.f;
    ERROR_CHECK(system->getParameterByID(id, &value));
    return value;
}

Dictionary Fmod::get_global_parameter_desc_by_name(const String& parameterName) {
    Dictionary paramDesc;
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(system->getParameterDescriptionByName(parameterName.utf8().get_data(), &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }

    return paramDesc;
}

Dictionary Fmod::get_global_parameter_desc_by_id(const Array& idPair) {
    if (idPair.size() != 2) {
        GODOT_LOG(2, "FMOD Sound System: Invalid parameter ID")
        return {};
    }
    Dictionary paramDesc;
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(system->getParameterDescriptionByID(id, &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }

    return paramDesc;
}

int Fmod::get_global_parameter_desc_count() {
    int count = 0;
    ERROR_CHECK(system->getParameterDescriptionCount(&count));
    return count;
}

Array Fmod::get_global_parameter_desc_list() {
    Array a;
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    descList[256];
    int count = 0;
    ERROR_CHECK(system->getParameterDescriptionList(descList, 256, &count));
    for (int i = 0; i < count; i++) {
        auto pDesc = descList[i];
        Dictionary paramDesc;
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
        a.append(paramDesc);
    }
    return a;
}

void Fmod::set_callback(const uint64_t instanceId, int callbackMask) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setCallback(Callbacks::eventCallback, callbackMask));
    GODOT_LOG(0, String("CallBack set on event ") + String::num(instanceId, 0))
}

// runs on the game thread
void Fmod::_run_callbacks() {
    std::lock_guard<std::mutex> lk(Callbacks::callback_mut);
    for (int i = 0; i < events.size(); i++) {
        FMOD::Studio::EventInstance* eventInstance = events.get(i);
        auto eventInfo = _get_event_info(eventInstance);
        if (eventInstance && eventInfo) {
            Callbacks::CallbackInfo* cbInfo = &eventInfo->callbackInfo;
            // check for Marker callbacks
            if (!cbInfo->markerSignalEmitted) {
                emit_signal("timeline_marker", cbInfo->markerCallbackInfo);
                cbInfo->markerSignalEmitted = true;
            }

            // check for Beat callbacks
            if (!cbInfo->beatSignalEmitted) {
                emit_signal("timeline_beat", cbInfo->beatCallbackInfo);
                cbInfo->beatSignalEmitted = true;
            }

            // check for Sound callbacks
            if (!cbInfo->soundSignalEmitted) {
                if (cbInfo->soundCallbackInfo["type"] == String("played")) emit_signal("sound_played", cbInfo->soundCallbackInfo);
                else
                    emit_signal("sound_stopped", cbInfo->soundCallbackInfo);
                cbInfo->soundSignalEmitted = true;
            }
        } else {
            GODOT_LOG(2, "A managed event doesn't have an EventInfoStructure")
        }
    }
}