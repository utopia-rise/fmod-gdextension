#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "helpers/common.h"
#include "helpers/maths.h"

#include <fmod_server.h>

#include <classes/node3d.hpp>

using namespace godot;

FmodServer* FmodServer::singleton = nullptr;

void FmodServer::_bind_methods() {
    // LIFECYCLE
    ClassDB::bind_method(D_METHOD("init", "numOfChannels", "studioFlag", "flag"), &FmodServer::init);
    ClassDB::bind_method(D_METHOD("update"), &FmodServer::update);
    ClassDB::bind_method(D_METHOD("shutdown"), &FmodServer::shutdown);

    // SETTINGS
    ClassDB::bind_method(D_METHOD("set_software_format", "sampleRate", "speakerMode", "numRawSpeakers"), &FmodServer::set_software_format);
    ClassDB::bind_method(D_METHOD("set_sound_3D_settings", "dopplerScale", "distanceFactor", "rollOffScale"), &FmodServer::set_sound_3d_settings);
    ClassDB::bind_method(D_METHOD("set_system_dsp_buffer_size", "bufferlength", "numbuffers"), &FmodServer::set_system_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_system_dsp_buffer_size"), &FmodServer::get_system_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_system_dsp_buffer_length"), &FmodServer::get_system_dsp_buffer_length);
    ClassDB::bind_method(D_METHOD("get_system_dsp_num_buffers"), &FmodServer::get_system_dsp_num_buffers);

    // OBJECT
    ClassDB::bind_method(D_METHOD("check_VCA_path", "cvaPath"), &FmodServer::check_vca_path);
    ClassDB::bind_method(D_METHOD("check_bus_path", "busPath"), &FmodServer::check_bus_path);
    ClassDB::bind_method(D_METHOD("check_event_path", "eventPath"), &FmodServer::check_event_path);
    ClassDB::bind_method(D_METHOD("get_vca", "cvaPath"), &FmodServer::get_vca);
    ClassDB::bind_method(D_METHOD("get_bus", "busPath"), &FmodServer::get_bus);
    ClassDB::bind_method(D_METHOD("get_event", "eventPath"), &FmodServer::get_event);
    ClassDB::bind_method(D_METHOD("get_all_vca"), &FmodServer::get_all_vca);
    ClassDB::bind_method(D_METHOD("get_all_buses"), &FmodServer::get_all_buses);
    ClassDB::bind_method(D_METHOD("get_all_event_descriptions"), &FmodServer::get_all_event_descriptions);
    ClassDB::bind_method(D_METHOD("get_all_banks"), &FmodServer::get_all_banks);

    // DEBUGGING
    ClassDB::bind_method(D_METHOD("get_available_drivers"), &FmodServer::get_available_drivers);
    ClassDB::bind_method(D_METHOD("get_driver"), &FmodServer::get_driver);
    ClassDB::bind_method(D_METHOD("set_driver", "id"), &FmodServer::set_driver);
    ClassDB::bind_method(D_METHOD("get_performance_data"), &FmodServer::get_performance_data);

    // GLOBAL PARAMETERS
    ClassDB::bind_method(D_METHOD("set_global_parameter_by_name", "parameterName", "value"), &FmodServer::set_global_parameter_by_name);
    ClassDB::bind_method(D_METHOD("get_global_parameter_by_name", "parameterName"), &FmodServer::get_global_parameter_by_name);
    ClassDB::bind_method(D_METHOD("set_global_parameter_by_id", "idPair", "value"), &FmodServer::set_global_parameter_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_by_id", "idPair"), &FmodServer::get_global_parameter_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_by_name", "parameterName"), &FmodServer::get_global_parameter_desc_by_name);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_by_id", "idPair"), &FmodServer::get_global_parameter_desc_by_id);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_count"), &FmodServer::get_global_parameter_desc_count);
    ClassDB::bind_method(D_METHOD("get_global_parameter_desc_list"), &FmodServer::get_global_parameter_desc_list);

    // LISTENERS
    ClassDB::bind_method(D_METHOD("add_listener", "index", "gameObj"), &FmodServer::add_listener);
    ClassDB::bind_method(D_METHOD("remove_listener", "index"), &FmodServer::remove_listener);
    ClassDB::bind_method(D_METHOD("set_listener_number", "listenerNumber"), &FmodServer::set_system_listener_number);
    ClassDB::bind_method(D_METHOD("get_listener_number"), &FmodServer::get_system_listener_number);
    ClassDB::bind_method(D_METHOD("get_listener_weight", "index"), &FmodServer::get_system_listener_weight);
    ClassDB::bind_method(D_METHOD("set_listener_weight", "index", "weight"), &FmodServer::set_system_listener_weight);
    ClassDB::bind_method(D_METHOD("get_listener_transform3d", "index"), &FmodServer::get_listener_transform3d);
    ClassDB::bind_method(D_METHOD("get_listener_transform2d", "index"), &FmodServer::get_listener_transform2d);
    ClassDB::bind_method(D_METHOD("get_listener_3d_velocity", "index"), &FmodServer::get_listener_3d_velocity);
    ClassDB::bind_method(D_METHOD("get_listener_2d_velocity", "index"), &FmodServer::get_listener_2d_velocity);
    ClassDB::bind_method(D_METHOD("set_listener_transform3d", "index", "transform"), &FmodServer::set_listener_transform3d);
    ClassDB::bind_method(D_METHOD("set_listener_transform2d", "index", "transform"), &FmodServer::set_listener_transform2d);
    ClassDB::bind_method(D_METHOD("set_listener_lock", "index", "isLocked"), &FmodServer::set_listener_lock);
    ClassDB::bind_method(D_METHOD("get_listener_lock", "index"), &FmodServer::get_listener_lock);
    ClassDB::bind_method(D_METHOD("get_object_attached_to_listener", "index"), &FmodServer::get_object_attached_to_listener);

    ClassDB::bind_method(D_METHOD("load_bank", "pathToBank", "flag"), &FmodServer::load_bank);
    ClassDB::bind_method(D_METHOD("wait_for_all_loads"), &FmodServer::wait_for_all_loads);
    ClassDB::bind_method(D_METHOD("banks_still_loading"), &FmodServer::banks_still_loading);
    ClassDB::bind_method(D_METHOD("unload_bank", "pathToBank"), &FmodServer::unload_bank);

    ClassDB::bind_method(D_METHOD("load_file_as_sound", "path"), &FmodServer::load_file_as_sound);
    ClassDB::bind_method(D_METHOD("load_file_as_music", "path"), &FmodServer::load_file_as_music);
    ClassDB::bind_method(D_METHOD("unload_file", "path"), &FmodServer::unload_file);

    ClassDB::bind_method(D_METHOD("create_event_instance", "eventPath"), &FmodServer::create_event_instance);
    ClassDB::bind_method(D_METHOD("play_one_shot", "eventName", "gameObj"), &FmodServer::play_one_shot);
    ClassDB::bind_method(D_METHOD("play_one_shot_with_params", "eventName", "gameObj", "parameters"), &FmodServer::play_one_shot_with_params);
    ClassDB::bind_method(D_METHOD("play_one_shot_attached", "eventName", "gameObj"), &FmodServer::play_one_shot_attached);
    ClassDB::bind_method(D_METHOD("play_one_shot_attached_with_params", "eventName", "gameObj", "parameters"), &FmodServer::play_one_shot_attached_with_params);
    ClassDB::bind_method(D_METHOD("pause_all_events"), &FmodServer::pause_all_events);
    ClassDB::bind_method(D_METHOD("unpause_all_events"), &FmodServer::unpause_all_events);
    ClassDB::bind_method(D_METHOD("mute_all_events"), &FmodServer::mute_all_events);
    ClassDB::bind_method(D_METHOD("unmute_all_events"), &FmodServer::unmute_all_events);

    ClassDB::bind_method(D_METHOD("create_sound_instance", "path"), &FmodServer::create_sound_instance);
    REGISTER_ALL_CONSTANTS
}

FmodServer::FmodServer() :
  system(nullptr),
  coreSystem(nullptr),
  isInitialized(false),
  isNotInitializedPrinted(false),
  distanceScale(1.0),
  cache(nullptr) {
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
    callback_mutex.instantiate();
    performanceData = create_ref<FmodPerformanceData>();
    Callbacks::GodotFileRunner::get_singleton()->start();
}

FmodServer::~FmodServer() {
    callbacks_to_process.clear();

    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}

FmodServer* FmodServer::get_singleton() {
    return singleton;
}

void FmodServer::init(int numOfChannels, const unsigned int studioFlag, const unsigned int flag) {
    // initialize FMOD Studio and FMOD Core System with provided flags
    if (system == nullptr && coreSystem == nullptr) {
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }

    if (ERROR_CHECK(system->initialize(numOfChannels, studioFlag, flag, nullptr))) {
        isInitialized = true;
        GODOT_LOG_INFO("FMOD Sound System: Successfully initialized")
        if (studioFlag == FMOD_STUDIO_INIT_LIVEUPDATE) { GODOT_LOG_INFO("FMOD Sound System: Live update enabled!") }
    }

    if (ERROR_CHECK(
          coreSystem->setFileSystem(&Callbacks::godotFileOpen, &Callbacks::godotFileClose, nullptr, nullptr, &Callbacks::godotSyncRead, &Callbacks::godotSyncCancel, -1)
        )) {
        GODOT_LOG_INFO("Custom File System enabled.")
    }
    cache = new FmodCache(system);
}

void FmodServer::update() {
    if (!isInitialized) {
        if (!isNotInitializedPrinted) {
            GODOT_LOG_ERROR("FMOD Sound System: Fmod should be initialized before calling update")
            isNotInitializedPrinted = true;
        }
        return;
    }

    // Check if bank are loaded, load buses, vca and event descriptions.
    cache->update_pending();

    callback_mutex->lock();

    for (const Callback& callback : callbacks_to_process) {
        godot::Array args = godot::Array();
        args.append(callback.fmod_callback_properties);
        args.append(callback.type);
        callback.callable.callv(args);
    }

    callbacks_to_process.clear();

    callback_mutex->unlock();

    for (OneShot* oneShot : oneShots) {
        if (!oneShot->instance.is_valid()) {
            oneShots.erase(oneShot);
            continue;
        }

        FMOD_STUDIO_PLAYBACK_STATE s;
        ERROR_CHECK(oneShot->instance->get_wrapped()->getPlaybackState(&s));

        if (s == FMOD_STUDIO_PLAYBACK_STOPPED || is_dead(oneShot->gameObj)) {
            FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
            oneShot->instance->stop(m);
            oneShots.erase(oneShot);
            continue;
        }

        oneShot->instance->set_node_attributes(oneShot->gameObj);
    }

    for (Ref<FmodEvent> event : runningEvents) {
        if (!event->is_valid()) { runningEvents.erase(event); }
    }

    _set_listener_attributes();
    _update_performance_data();

    ERROR_CHECK(system->update());
}

void FmodServer::_set_listener_attributes() {
    if (actualListenerNumber == 0) {
        if (listenerWarning) {
            GODOT_LOG_WARNING("FMOD Sound System: No listeners are set!")
            listenerWarning = false;
        }
        return;
    }

    for (int i = 0; i < systemListenerNumber; ++i) {
        Listener* listener = &listeners[i];
        if (listener->listenerLock) { continue; }
        if (is_dead(listener->gameObj)) {
            listener->gameObj = nullptr;
            ERROR_CHECK(system->setListenerWeight(i, 0));
            continue;
        }

        Node* node {Object::cast_to<Node>(listener->gameObj)};
        if (!node->is_inside_tree()) { return; }

        if (auto* ci {Node::cast_to<CanvasItem>(node)}) {
            FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform2d(ci->get_global_transform(), distanceScale);
            ERROR_CHECK(system->setListenerAttributes(i, &attr));
            continue;
        }

        if (auto* s {Node::cast_to<Node3D>(node)}) {
            FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform3d(s->get_global_transform(), distanceScale);
            ERROR_CHECK(system->setListenerAttributes(i, &attr));
            continue;
        }
    }
}

void FmodServer::shutdown() {
    isInitialized = false;
    isNotInitializedPrinted = false;
    ERROR_CHECK(system->unloadAll());
    ERROR_CHECK(system->release());
    system = nullptr;
    coreSystem = nullptr;
    delete cache;
    cache = nullptr;
    GODOT_LOG_INFO("FMOD Sound System: System released")
}

void FmodServer::set_system_listener_number(int p_listenerNumber) {
    if (p_listenerNumber > 0 && p_listenerNumber <= FMOD_MAX_LISTENERS) {
        if (ERROR_CHECK(system->setNumListeners(p_listenerNumber))) { systemListenerNumber = p_listenerNumber; }
    } else {
        GODOT_LOG_ERROR("Number of listeners must be set between 1 and 8")
    }
}

void FmodServer::add_listener(int index, Object* gameObj) {
    if (!is_fmod_valid(gameObj)) { return; }
    if (index >= 0 && index < systemListenerNumber) {
        Listener* listener = &listeners[index];
        listener->gameObj = gameObj;
        ERROR_CHECK(system->setListenerWeight(index, listener->weight));
        int count = 0;
        for (int i = 0; i < systemListenerNumber; ++i) {
            if ((&listeners[i])->gameObj != nullptr) count++;
        }
        actualListenerNumber = count;
        if (actualListenerNumber > 0) listenerWarning = true;
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

void FmodServer::remove_listener(int index) {
    if (index >= 0 && index < systemListenerNumber) {
        Listener* listener = &listeners[index];
        listener->gameObj = nullptr;
        ERROR_CHECK(system->setListenerWeight(index, 0));
        int count = 0;
        for (int i = 0; i < systemListenerNumber; ++i) {
            if ((&listeners[i])->gameObj != nullptr) count++;
        }
        actualListenerNumber = count;
        if (actualListenerNumber > 0) listenerWarning = true;
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

int FmodServer::get_system_listener_number() const {
    return systemListenerNumber;
}

float FmodServer::get_system_listener_weight(const int index) {
    if (index >= 0 && index < systemListenerNumber) {
        float weight = 0;
        ERROR_CHECK(system->getListenerWeight(index, &weight));
        listeners[index].weight = weight;
        return weight;
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
        return 0;
    }
}

void FmodServer::set_system_listener_weight(const int index, float weight) {
    if (index >= 0 && index < systemListenerNumber) {
        listeners[index].weight = weight;
        ERROR_CHECK(system->setListenerWeight(index, weight));
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

Transform3D FmodServer::get_listener_transform3d(int index) {
    Transform3D transform;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        transform = get_transform3d_from_3d_attributes(attr, distanceScale);
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
    return transform;
}

Transform2D FmodServer::get_listener_transform2d(int index) {
    Transform2D transform;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        transform = get_transform2d_from_3d_attributes(attr, distanceScale);
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
    return transform;
}

Vector3 FmodServer::get_listener_3d_velocity(int index) {
    Vector3 velocity;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        velocity = get_velocity3d_from_3d_attributes(attr, distanceScale);
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
    return velocity;
}

Vector2 FmodServer::get_listener_2d_velocity(int index) {
    Vector2 velocity;
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr;
        ERROR_CHECK(system->getListenerAttributes(index, &attr));
        velocity = get_velocity2d_from_3d_attributes(attr, distanceScale);
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
    return velocity;
}

void FmodServer::set_listener_transform3d(int index, const Transform3D& transform) {
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform3d(transform, distanceScale);
        ERROR_CHECK(system->setListenerAttributes(index, &attr));
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

void FmodServer::set_listener_transform2d(int index, const Transform2D& transform) {
    if (index >= 0 && index < systemListenerNumber) {
        FMOD_3D_ATTRIBUTES attr = get_3d_attributes_from_transform2d(transform, distanceScale);
        ERROR_CHECK(system->setListenerAttributes(index, &attr));
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

void FmodServer::set_listener_lock(int index, bool isLocked) {
    if (index >= 0 && index < systemListenerNumber) {
        listeners[index].listenerLock = isLocked;
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
    }
}

bool FmodServer::get_listener_lock(int index) {
    if (index >= 0 && index < systemListenerNumber) {
        return listeners[index].listenerLock;
    } else {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
        return false;
    }
}

Object* FmodServer::get_object_attached_to_listener(int index) {
    if (index < 0 || index >= systemListenerNumber) {
        GODOT_LOG_ERROR("index of listeners must be set between 0 and the number of listeners set")
        return nullptr;
    } else {
        Object* node = listeners[index].gameObj;
        if (!node) { GODOT_LOG_WARNING("No node was set on listener") }
        return node;
    }
}

void FmodServer::set_software_format(int sampleRate, const int speakerMode, int numRawSpeakers) {
    if (system == nullptr && coreSystem == nullptr) {
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }
    ERROR_CHECK(coreSystem->setSoftwareFormat(sampleRate, static_cast<FMOD_SPEAKERMODE>(speakerMode), numRawSpeakers));
}

Ref<FmodBank> FmodServer::load_bank(const String& pathToBank, unsigned int flag) {
    if (cache->has_bank(pathToBank)) return {cache->get_bank(pathToBank)};// bank is already loaded
    return cache->add_bank(pathToBank, flag);
}

void FmodServer::unload_bank(const String& pathToBank) {
    cache->remove_bank(pathToBank);
}

bool FmodServer::banks_still_loading() {
    return cache->is_loading();
}

bool FmodServer::check_vca_path(const String& vcaPath) {
    return cache->has_vca_path(vcaPath);
}

bool FmodServer::check_bus_path(const String& busPath) {
    return cache->has_bus_path(busPath);
}

bool FmodServer::check_event_path(const String& eventPath) {
    return cache->has_event_path(eventPath);
}

Ref<FmodVCA> FmodServer::get_vca(const String& vcaPath) {
    return cache->get_vca(vcaPath);
}

Ref<FmodBus> FmodServer::get_bus(const String& busPath) {
    return cache->get_bus(busPath);
}

Ref<FmodEventDescription> FmodServer::get_event(const String& eventPath) {
    return cache->get_event(eventPath);
}

Array FmodServer::get_all_vca() {
    Array array;
    for (KeyValue<String, Ref<FmodVCA>>& entry : cache->vcas) {
        array.append(entry.value);
    }
    return array;
}

Array FmodServer::get_all_buses() {
    Array array;
    for (KeyValue<String, Ref<FmodBus>>& entry : cache->buses) {
        array.append(entry.value);
    }
    return array;
}

Array FmodServer::get_all_event_descriptions() {
    Array array;
    for (KeyValue<String, Ref<FmodEventDescription>>& entry : cache->eventDescriptions) {
        array.append(entry.value);
    }
    return array;
}

Array FmodServer::get_all_banks() {
    Array array;
    for (KeyValue<String, Ref<FmodBank>>& entry : cache->banks) {
        array.append(entry.value);
    }
    return array;
}

Ref<FmodEvent> FmodServer::create_event_instance(const String& eventPath) {
    Ref<FmodEvent> ref = _create_instance(eventPath, false, nullptr);
    ref->get_wrapped()->setUserData(ref.ptr());
    ref->set_distance_scale(distanceScale);
    return ref;
}

Ref<FmodEvent> FmodServer::_create_instance(const String& eventName, bool isOneShot, Node* gameObject) {
    bool found = cache->has_event_path(eventName);
    if (!found) {
        GODOT_LOG_WARNING("Event " + eventName + " can't be found. Check if the path is correct or the bank properly loaded.")
    }

    Ref<FmodEventDescription> desc = cache->get_event(eventName);
    FMOD::Studio::EventInstance* eventInstance = nullptr;
    ERROR_CHECK(desc->get_wrapped()->createInstance(&eventInstance));
    Ref<FmodEvent> ref = FmodEvent::create_ref(eventInstance);
    if (ref.is_valid()) {
        runningEvents.push_back(ref);
        if (isOneShot || gameObject) {
            auto* oneShot = new OneShot();
            oneShot->gameObj = gameObject;
            oneShots.push_back(oneShot);
        }
    }

    return ref;
}

void FmodServer::play_one_shot(const String& eventName, Node* gameObj) {
    Ref<FmodEvent> ref = _create_instance(eventName, true, nullptr);
    if (!ref.is_valid()) { return; }

    ref->set_node_attributes(gameObj);
    ref->start();
    ref->release();
}

void FmodServer::play_one_shot_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters) {
    Ref<FmodEvent> ref = _create_instance(eventName, true, nullptr);
    if (!ref.is_valid()) { return; }

    ref->set_node_attributes(gameObj);
    // set the initial parameter values
    auto keys = parameters.keys();
    for (int i = 0; i < keys.size(); ++i) {
        String k = keys[i];
        float v = parameters[keys[i]];
        ref->set_parameter_by_name(k.utf8().get_data(), v);
    }
    ref->start();
    ref->release();
}

void FmodServer::play_one_shot_attached(const String& eventName, Node* gameObj) {
    if (!is_fmod_valid(gameObj)) { return; }

    Ref<FmodEvent> ref = _create_instance(eventName, true, gameObj);
    if (!ref.is_valid()) { return; }
    ref->start();
    ref->release();
}

void FmodServer::play_one_shot_attached_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters) {
    if (!is_fmod_valid(gameObj)) { return; }

    Ref<FmodEvent> ref = _create_instance(eventName, true, gameObj);
    if (!ref.is_valid()) { return; }

    // set the initial parameter values
    auto keys = parameters.keys();
    for (int i = 0; i < keys.size(); ++i) {
        String k = keys[i];
        float v = parameters[keys[i]];
        ref->set_parameter_by_name(k.utf8().get_data(), v);
    }
    ref->start();
    ref->release();
}

void FmodServer::set_system_dsp_buffer_size(unsigned int bufferLength, int numBuffers) {
    if (bufferLength > 0 && numBuffers > 0 && ERROR_CHECK(coreSystem->setDSPBufferSize(bufferLength, numBuffers))) {
        GODOT_LOG_INFO("FMOD Sound System: Successfully set DSP buffer size")
    } else {
        GODOT_LOG_ERROR("FMOD Sound System: Failed to set DSP buffer size :|")
    }
}

Array FmodServer::get_system_dsp_buffer_size() {
    unsigned int bufferLength;
    int numBuffers;
    Array a;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferLength, &numBuffers));
    a.append(bufferLength);
    a.append(numBuffers);
    return a;
}

unsigned int FmodServer::get_system_dsp_buffer_length() {
    unsigned int bufferLength;
    int numBuffers;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferLength, &numBuffers));
    return bufferLength;
}

int FmodServer::get_system_dsp_num_buffers() {
    unsigned int bufferLength;
    int numBuffers;
    ERROR_CHECK(coreSystem->getDSPBufferSize(&bufferLength, &numBuffers));
    return numBuffers;
}

void FmodServer::pause_all_events() {
    for (Ref<FmodEvent> eventInstance : runningEvents) {
        eventInstance->set_paused(true);
    }
}

void FmodServer::unpause_all_events() {
    for (Ref<FmodEvent> eventInstance : runningEvents) {
        eventInstance->set_paused(false);
    }
}

void FmodServer::mute_all_events() {
    if (cache->is_master_loaded()) {
        FMOD::Studio::Bus* masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) { masterBus->setMute(true); }
    }
}

void FmodServer::unmute_all_events() {
    if (cache->is_master_loaded()) {
        FMOD::Studio::Bus* masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) { masterBus->setMute(false); }
    }
}

Ref<FmodFile> FmodServer::load_file_as_sound(const String& path) {
    if (cache->has_file(path)) {
        GODOT_LOG_WARNING("FMOD Sound System: FILE ALREADY LOADED AS SOUND" + String(path))
        return cache->get_file(path);
    }
    return cache->add_file(path, FMOD_CREATESAMPLE);
}

Ref<FmodFile> FmodServer::load_file_as_music(const String& path) {
    if (cache->has_file(path)) {
        GODOT_LOG_WARNING("FMOD Sound System: FILE ALREADY LOADED AS MUSIC" + String(path))
        return cache->get_file(path);
    }
    return cache->add_file(path, (FMOD_CREATESTREAM | FMOD_LOOP_NORMAL));
}

void FmodServer::unload_file(const String& path) {
    if (!cache->has_file(path)) {
        GODOT_LOG_WARNING("File " + path + " can't be found. Check if it was properly loaded or already unloaded.")
        return;
    }
    cache->remove_file(path);
    GODOT_LOG_INFO("FMOD Sound System: UNLOADING FILE" + String(path))
}

Ref<FmodSound> FmodServer::create_sound_instance(const String& path) {
    if (!cache->has_file(path)) {
        GODOT_LOG_WARNING("File " + path + " can't be found. Check if it was properly loaded.")
        return {};
    }

    Ref<FmodFile> file = cache->get_file(path);
    FMOD::Channel* channel = nullptr;
    ERROR_CHECK(coreSystem->playSound(file->get_wrapped(), nullptr, true, &channel));
    if (channel) {
        Ref<FmodSound> ref = FmodSound::create_ref(channel);
        return ref;
    }
    return {};
}

void FmodServer::set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale) {
    if (distanceFactor <= 0) {
        GODOT_LOG_ERROR("FMOD Sound System: Failed to set 3D settings - invalid distance factor!")
    } else if (ERROR_CHECK(coreSystem->set3DSettings(dopplerScale, distanceFactor, rollOffScale))) {
        distanceScale = distanceFactor;
        GODOT_LOG_INFO("FMOD Sound System: Successfully set global 3D settings")
    } else {
        GODOT_LOG_ERROR("FMOD Sound System: Failed to set 3D settings")
    }
}

void FmodServer::wait_for_all_loads() {
    ERROR_CHECK(system->flushSampleLoading());
    cache->update_pending();
}

Array FmodServer::get_available_drivers() {
    Array driverList;
    int numDrivers = 0;

    ERROR_CHECK(coreSystem->getNumDrivers(&numDrivers));

    for (int i = 0; i < numDrivers; ++i) {
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

int FmodServer::get_driver() {
    int driverId = -1;
    ERROR_CHECK(coreSystem->getDriver(&driverId));
    return driverId;
}

void FmodServer::set_driver(const int id) {
    ERROR_CHECK(coreSystem->setDriver(id));
}

void FmodServer::_update_performance_data() {
    // get the CPU usage
    FMOD_STUDIO_CPU_USAGE studioCpuUsage;
    FMOD_CPU_USAGE cpuUsage;
    ERROR_CHECK(system->getCPUUsage(&studioCpuUsage, &cpuUsage));
    performanceData->dsp = cpuUsage.dsp;
    performanceData->geometry = cpuUsage.geometry;
    performanceData->stream = cpuUsage.stream;
    performanceData->update = cpuUsage.update;
    performanceData->convolution1 = cpuUsage.convolution1;
    performanceData->convolution2 = cpuUsage.convolution2;
    performanceData->studio = studioCpuUsage.update;

    // get the memory usage
    int currentAlloc = 0;
    int maxAlloc = 0;
    ERROR_CHECK(FMOD::Memory_GetStats(&currentAlloc, &maxAlloc));
    performanceData->currently_allocated = currentAlloc;
    performanceData->max_allocated = maxAlloc;

    // get the file usage
    long long sampleBytesRead = 0;
    long long streamBytesRead = 0;
    long long otherBytesRead = 0;
    ERROR_CHECK(coreSystem->getFileUsage(&sampleBytesRead, &streamBytesRead, &otherBytesRead));
    performanceData->sample_bytes_read = static_cast<int>(sampleBytesRead);
    performanceData->stream_bytes_read = static_cast<int>(streamBytesRead);
    performanceData->other_bytes_read = static_cast<int>(otherBytesRead);
}

Ref<FmodPerformanceData> FmodServer::get_performance_data() {
    return performanceData;
}

void FmodServer::set_global_parameter_by_name(const String& parameterName, float value) {
    ERROR_CHECK(system->setParameterByName(parameterName.utf8().get_data(), value));
}

float FmodServer::get_global_parameter_by_name(const String& parameterName) {
    float value = 0.f;
    ERROR_CHECK(system->getParameterByName(parameterName.utf8().get_data(), &value));
    return value;
}

void FmodServer::set_global_parameter_by_id(const Array& idPair, const float value) {
    if (idPair.size() != 2) {
        GODOT_LOG_ERROR("FMOD Sound System: Invalid parameter ID")
        return;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(system->setParameterByID(id, value));
}

float FmodServer::get_global_parameter_by_id(const Array& idPair) {
    if (idPair.size() != 2) {
        GODOT_LOG_ERROR("FMOD Sound System: Invalid parameter ID")
        return -1.f;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    float value = -1.f;
    ERROR_CHECK(system->getParameterByID(id, &value));
    return value;
}

Dictionary FmodServer::get_global_parameter_desc_by_name(const String& parameterName) {
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

Dictionary FmodServer::get_global_parameter_desc_by_id(const Array& idPair) {
    if (idPair.size() != 2) {
        GODOT_LOG_ERROR("FMOD Sound System: Invalid parameter ID")
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

int FmodServer::get_global_parameter_desc_count() {
    int count = 0;
    ERROR_CHECK(system->getParameterDescriptionCount(&count));
    return count;
}

Array FmodServer::get_global_parameter_desc_list() {
    Array a;
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    descList[256];
    int count = 0;
    ERROR_CHECK(system->getParameterDescriptionList(descList, 256, &count));
    for (int i = 0; i < count; ++i) {
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

void FmodServer::add_callback(const Callback& callback) {
    callback_mutex->lock();
    callbacks_to_process.push_back(callback);
    callback_mutex->unlock();
}