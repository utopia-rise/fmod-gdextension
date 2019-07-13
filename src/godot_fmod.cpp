//
// Created by Pierre-Thomas Meisels on 2019-01-01.
//

#include "godot_fmod.h"

using namespace godot;

Mutex *Callbacks::mut;

Fmod::Fmod() = default;

Fmod::~Fmod() {
    Fmod::shutdown();
    delete Callbacks::mut;
    Callbacks::mut = nullptr;
}

void Fmod::_register_methods() {
    register_method("init", &Fmod::init);
    register_method("shutdown", &Fmod::shutdown);
    register_method("add_listener", &Fmod::addListener);
    register_method("set_software_format", &Fmod::setSoftwareFormat);
    register_method("load_bank", &Fmod::loadBank);
    register_method("unload_bank", &Fmod::unloadBank);
    register_method("check_VCA_path", &Fmod::checkVCAPath);
    register_method("check_bus_path", &Fmod::checkBusPath);
    register_method("check_event_path", &Fmod::checkEventPath);
    register_method("get_bank_loading_state", &Fmod::getBankLoadingState);
    register_method("get_bank_bus_count", &Fmod::getBankBusCount);
    register_method("get_bank_event_count", &Fmod::getBankEventCount);
    register_method("get_bank_string_count", &Fmod::getBankStringCount);
    register_method("get_bank_VCA_count", &Fmod::getBankVCACount);
    register_method("create_event_instance", &Fmod::createEventInstance);
    register_method("get_event_parameter_by_name", &Fmod::getEventParameterByName);
    register_method("set_event_parameter_by_name", &Fmod::setEventParameterByName);
    register_method("get_event_parameter_by_id", &Fmod::getEventParameterByID);
    register_method("set_event_parameter_by_id", &Fmod::setEventParameterByID);
    register_method("release_event", &Fmod::releaseEvent);
    register_method("start_event", &Fmod::startEvent);
    register_method("stop_event", &Fmod::stopEvent);
    register_method("trigger_event_cue", &Fmod::triggerEventCue);
    register_method("get_event_playback_state", &Fmod::getEventPlaybackState);
    register_method("get_event_paused", &Fmod::getEventPaused);
    register_method("set_event_paused", &Fmod::setEventPaused);
    register_method("get_event_pitch", &Fmod::getEventPitch);
    register_method("set_event_pitch", &Fmod::setEventPitch);
    register_method("get_event_volume", &Fmod::getEventVolume);
    register_method("set_event_volume", &Fmod::setEventVolume);
    register_method("get_event_timeline_position", &Fmod::getEventTimelinePosition);
    register_method("set_event_timeline_position", &Fmod::setEventTimelinePosition);
    register_method("get_event_reverb_level", &Fmod::getEventReverbLevel);
    register_method("set_event_reverb_level", &Fmod::setEventReverbLevel);
    register_method("is_event_virtual", &Fmod::isEventVirtual);
    register_method("get_bus_mute", &Fmod::getBusMute);
    register_method("get_bus_paused", &Fmod::getBusPaused);
    register_method("get_bus_volume", &Fmod::getBusVolume);
    register_method("set_bus_mute", &Fmod::setBusMute);
    register_method("set_bus_paused", &Fmod::setBusPaused);
    register_method("set_bus_volume", &Fmod::setBusVolume);
    register_method("stop_all_bus_events", &Fmod::stopAllBusEvents);
    register_method("get_VCA_volume", &Fmod::getVCAVolume);
    register_method("set_VCA_volume", &Fmod::setVCAVolume);
    register_method("play_one_shot", &Fmod::playOneShot);
    register_method("play_one_shot_with_params", &Fmod::playOneShotWithParams);
    register_method("play_one_shot_attached", &Fmod::playOneShotAttached);
    register_method("play_one_shot_attached_with_params", &Fmod::playOneShotAttachedWithParams);
    register_method("attach_instance_to_node", &Fmod::attachInstanceToNode);
    register_method("detach_instance_from_node", &Fmod::detachInstanceFromNode);
    register_method("pause_all_events", &Fmod::pauseAllEvents);
    register_method("mute_all_events", &Fmod::muteAllEvents);
    register_method("unmute_all_events", &Fmod::unmuteAllEvents);
    register_method("banks_still_loading", &Fmod::banksStillLoading);
    register_method("load_sound", &Fmod::loadSound);
    register_method("play_sound", &Fmod::playSound);
    register_method("stop_sound", &Fmod::stopSound);
    register_method("release_sound", &Fmod::releaseSound);
    register_method("set_sound_paused", &Fmod::setSoundPaused);
    register_method("is_sound_playing", &Fmod::isSoundPlaying);
    register_method("set_sound_volume", &Fmod::setSoundVolume);
    register_method("get_sound_volume", &Fmod::getSoundVolume);
    register_method("set_sound_pitch", &Fmod::setSoundPitch);
    register_method("get_sound_pitch", &Fmod::getSoundPitch);
    register_method("set_callback", &Fmod::setCallback);
    register_method("set_sound_3D_settings", &Fmod::setSound3DSettings);
    register_method("wait_for_all_loads", &Fmod::waitForAllLoads);
    register_method("get_available_drivers", &Fmod::getAvailableDrivers);
    register_method("get_driver", &Fmod::getDriver);
    register_method("set_driver", &Fmod::setDriver);
    register_method("get_performance_data", &Fmod::getPerformanceData);
    register_method("set_global_parameter_by_name", &Fmod::setGlobalParameterByName);
    register_method("get_global_parameter_by_name", &Fmod::getGlobalParameterByName);
    register_method("set_global_parameter_by_id", &Fmod::setGlobalParameterByID);
    register_method("get_global_parameter_by_id", &Fmod::getGlobalParameterByID);
    register_method("_process", &Fmod::_process);

    register_signal<Fmod>("timeline_beat", "params", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<Fmod>("timeline_marker", "params", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<Fmod>("sound_played", "params", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<Fmod>("sound_stopped", "params", GODOT_VARIANT_TYPE_DICTIONARY);

    REGISTER_ALL_CONSTANTS
}

bool Fmod::checkErrors(FMOD_RESULT result, const char *function, const char *file, int line){
    if (result != FMOD_OK) {
        godot::Godot::print_error(FMOD_ErrorString(result), function, file, line);
        return false;
    }
    return true;
}

void Fmod::init(int numOfChannels, const unsigned int studioFlag, const unsigned int flag) {
    // initialize FMOD Studio and FMOD Core System with provided flags
    if(system == nullptr && coreSystem == nullptr){
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }

    if (ERROR_CHECK(system->initialize(numOfChannels, studioFlag, flag, nullptr))) {
        isInitialized = true;
        Godot::print("FMOD Sound System: Successfully initialized");
        if (studioFlag == FMOD_STUDIO_INIT_LIVEUPDATE) {
            Godot::print("FMOD Sound System: Live update enabled!");
        }
    }
}

void Fmod::_process(float delta){
    if (!isInitialized) {
        if (!isNotinitPrinted) {
            GODOT_ERROR("FMOD Sound System: Fmod should be initialized before calling update")
            isNotinitPrinted = true;
        }
        return;
    }

    //Check if bank are loaded, load buses, vca and event descriptions.
    checkLoadingBanks();

    for (int i = 0; i < events.size(); i++) {
        FMOD::Studio::EventInstance * eventInstance = events.get(i);
        if (eventInstance) {
            EventInfo *eventInfo = getEventInfo(eventInstance);
            if(eventInfo){
                if (eventInfo->gameObj) {
                    if (isNull(eventInfo->gameObj)) {
                        FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
                        ERROR_CHECK(eventInstance->stop(m));
                        releaseOneEvent(eventInstance);
                        continue;
                    }
                    if (eventInfo->isOneShot) {
                        FMOD_STUDIO_PLAYBACK_STATE s;
                        ERROR_CHECK(eventInstance->getPlaybackState(&s));
                        if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
                            releaseOneEvent(eventInstance);
                            continue;
                        }
                    }
                    updateInstance3DAttributes(eventInstance, eventInfo->gameObj);
                }
            }
            else {
                Godot::GODOT_ERROR("A managed event doesn't have an EventInfoStructure")
            }
        }
    }

    // update listener position
    setListenerAttributes();

    //run callback events
    runCallbacks();

    // dispatch update to FMOD
    ERROR_CHECK(system->update());
}

void Fmod::checkLoadingBanks() {
    for (int i = 0; i < loadingBanks.size(); i++) {
        auto loadingBank = loadingBanks.pop_front_value();
        auto bank = loadingBank->bank;
        FMOD_STUDIO_LOADING_STATE *loading_state = nullptr;
        ERROR_CHECK(bank->getLoadingState(loading_state));
        if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            loadBankData(loadingBank);
            delete loadingBank;
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADING) {
            loadingBanks.push_back_value(loadingBank);
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            bank->unload();
            delete loadingBank;
            GODOT_ERROR("Fmod Sound System: Error loading bank.")
        }
    }
}

void Fmod::setListenerAttributes() {
    if (isNull(listener)) {
        if (nullListenerWarning) {
            GODOT_ERROR("FMOD Sound System: Listener not set!")
            nullListenerWarning = false;
        }
        return;
    }
    auto *ci = Object::cast_to<CanvasItem>(listener);
    if (ci != nullptr) {
        Transform2D t2d = ci->get_transform();
        Vector2 posVector = t2d.get_origin() / distanceScale;
        Vector3 pos(posVector.x, 0.0f, posVector.y),
                up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0); // TODO: add doppler
        const FMOD_VECTOR &posFmodVector = toFmodVector(pos);
        auto attr = get3DAttributes(posFmodVector, toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
        ERROR_CHECK(system->setListenerAttributes(0, &attr));

    } else {
        // needs testing
        auto *s = Object::cast_to<Spatial>(listener);
        Transform t = s->get_transform();
        Vector3 pos = t.get_origin() / distanceScale;
        Vector3 up = t.get_basis().elements[1];
        Vector3 forward = t.get_basis().elements[2];
        Vector3 vel(0, 0, 0);
        auto attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
        ERROR_CHECK(system->setListenerAttributes(0, &attr));
    }
}

FMOD_VECTOR Fmod::toFmodVector(Vector3 &vec) {
    FMOD_VECTOR fv;
    fv.x = vec.x;
    fv.y = vec.y;
    fv.z = vec.z;
    return fv;
}

FMOD_3D_ATTRIBUTES Fmod::get3DAttributes(const FMOD_VECTOR &pos, const FMOD_VECTOR &up, const FMOD_VECTOR &forward,
                                         const FMOD_VECTOR &vel) {
    FMOD_3D_ATTRIBUTES f3d;
    f3d.forward = forward;
    f3d.position = pos;
    f3d.up = up;
    f3d.velocity = vel;
    return f3d;
}

bool Fmod::isNull(Object *o) {
    auto *ci = Object::cast_to<CanvasItem>(o);
    auto *s = Object::cast_to<Spatial>(o);
    return ci == nullptr && s == nullptr;
}

void Fmod::updateInstance3DAttributes(FMOD::Studio::EventInstance *instance, Object *o) {
    // try to set 3D attributes
    if (instance && !isNull(o)) {
        auto *ci = Object::cast_to<CanvasItem>(o);
        if (ci != nullptr) {
            Transform2D t2d = ci->get_transform();
            Vector2 posVector = t2d.get_origin() / distanceScale;
            Vector3 pos(posVector.x, 0.0f, posVector.y),
                    up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0);
            FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
            ERROR_CHECK(instance->set3DAttributes(&attr));
        } else {
            // needs testing
            auto *s = Object::cast_to<Spatial>(o);
            Transform t = s->get_transform();
            Vector3 pos = t.get_origin() / distanceScale;
            Vector3 up = t.get_basis().elements[1];
            Vector3 forward = t.get_basis().elements[2];
            Vector3 vel(0, 0, 0);
            FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
            ERROR_CHECK(instance->set3DAttributes(&attr));
        }
    }
}

void Fmod::shutdown() {
    isInitialized = false;
    isNotinitPrinted = false;
    system = nullptr;
    coreSystem = nullptr;
    ERROR_CHECK(system->unloadAll());
    ERROR_CHECK(system->release());
    Godot::print("FMOD Sound System: System released");

}

void Fmod::addListener(Object *gameObj) {
    listener = gameObj;
}

void Fmod::setSoftwareFormat(int sampleRate, const int speakerMode, int numRawSpeakers) {
    if(system == nullptr && coreSystem == nullptr){
        ERROR_CHECK(FMOD::Studio::System::create(&system));
        ERROR_CHECK(system->getCoreSystem(&coreSystem));
    }
    ERROR_CHECK(coreSystem->setSoftwareFormat(sampleRate, static_cast<FMOD_SPEAKERMODE>(speakerMode), numRawSpeakers));
}

String Fmod::loadBank(String pathToBank, const unsigned int flag) {
    DRIVE_PATH(pathToBank)
    if (banks.has(pathToBank)) return pathToBank; // bank is already loaded
    FMOD::Studio::Bank *bank = nullptr;
    ERROR_CHECK(system->loadBankFile(pathToBank.alloc_c_string(), flag, &bank));
    if (bank) {
        Godot::print("FMOD Sound System: LOADING BANK " + String(pathToBank));
        auto *loadingBank = new LoadingBank();
        loadingBank->bank = bank;
        loadingBank->godotPath = pathToBank;
        if (flag != FMOD_STUDIO_LOAD_BANK_NONBLOCKING){
            loadBankData(loadingBank);
        }
        else{
            loadingBanks.append(loadingBank);
        }
    }
    return pathToBank;
}

void Fmod::unloadBank(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks)
    unloadAllBuses(instance);
    unloadAllVCAs(instance);
    unloadAllEventDescriptions(instance);
    ERROR_CHECK(instance->unload());
    Godot::print("FMOD Sound System: BANK " + String(pathToBank) + " UNLOADED");
    banks.erase(pathToBank);
}

int Fmod::getBankLoadingState(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    FMOD_STUDIO_LOADING_STATE state;
    ERROR_CHECK(instance->getLoadingState(&state));
    return state;
}

int Fmod::getBankBusCount(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getBusCount(&count));
    return count;
}

int Fmod::getBankEventCount(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getEventCount(&count));
    return count;
}

int Fmod::getBankStringCount(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getStringCount(&count));
    return count;
}

int Fmod::getBankVCACount(String pathToBank) {
    DRIVE_PATH(pathToBank)
    FIND_AND_CHECK(pathToBank, banks, -1)
    int count = -1;
    ERROR_CHECK(instance->getVCACount(&count));
    return count;
}

const uint64_t Fmod::createEventInstance(String eventPath) {
    FMOD::Studio::EventInstance *instance = createInstance(eventPath, false, nullptr);
    if (instance) {
        return (uint64_t) instance;
    }
    return 0;
}

float Fmod::getEventParameterByName(uint64_t instanceId, String parameterName) {
    float p = -1;
    FIND_AND_CHECK(instanceId, events, p)
    ERROR_CHECK(instance->getParameterByName(parameterName.utf8().get_data(), &p));
    return p;
}

void Fmod::setEventParameterByName(const uint64_t instanceId, String parameterName, float value) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setParameterByName(parameterName.utf8().get_data(), value));
}

float Fmod::getEventParameterByID(const uint64_t instanceId, const Array idPair) {
    float value = -1.0f;
    FIND_AND_CHECK(instanceId, events, value)
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->getParameterByID(id, &value));
    return value;
}

void Fmod::setEventParameterByID(uint64_t instanceId, const Array idPair, const float value) {
    FIND_AND_CHECK(instanceId, events)
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(instance->setParameterByID(id, value));
}

void Fmod::releaseEvent(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    releaseOneEvent(instance);
}

void Fmod::releaseOneEvent(FMOD::Studio::EventInstance *eventInstance) {
    Callbacks::mut->lock();
    EventInfo *eventInfo = getEventInfo(eventInstance);
    eventInstance->setUserData(nullptr);
    ERROR_CHECK(eventInstance->release());
    events.erase(eventInstance);
    delete &eventInfo;
    Callbacks::mut->unlock();
}

void Fmod::startEvent(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->start());
}

void Fmod::stopEvent(const uint64_t instanceId, int stopMode) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void Fmod::triggerEventCue(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->triggerCue());
}

int Fmod::getEventPlaybackState(const uint64_t instanceId) {
    int s = -1;
    FIND_AND_CHECK(instanceId, events, s)
    ERROR_CHECK(instance->getPlaybackState((FMOD_STUDIO_PLAYBACK_STATE *) &s));
    return s;
}

bool Fmod::getEventPaused(const uint64_t instanceId) {
    bool paused = false;
    FIND_AND_CHECK(instanceId, events, paused)
    ERROR_CHECK(instance->getPaused(&paused));
    return paused;
}

void Fmod::setEventPaused(const uint64_t instanceId, bool paused) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setPaused(paused));
}

float Fmod::getEventPitch(const uint64_t instanceId) {
    float pitch = 0.0f;
    FIND_AND_CHECK(instanceId, events, pitch)
    ERROR_CHECK(instance->getPitch(&pitch));
    return pitch;
}

void Fmod::setEventPitch(const uint64_t instanceId, float pitch) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setPitch(pitch));
}

float Fmod::getEventVolume(const uint64_t instanceId) {
    float volume = 0.0f;
    FIND_AND_CHECK(instanceId, events, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::setEventVolume(const uint64_t instanceId, float volume) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setVolume(volume));
}

int Fmod::getEventTimelinePosition(const uint64_t instanceId) {
    int tp = 0;
    FIND_AND_CHECK(instanceId, events, tp)
    ERROR_CHECK(instance->getTimelinePosition(&tp));
    return tp;
}

void Fmod::setEventTimelinePosition(const uint64_t instanceId, int position) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setTimelinePosition(position));
}

float Fmod::getEventReverbLevel(const uint64_t instanceId, int index) {
    float rvl = 0.0f;
    FIND_AND_CHECK(instanceId, events, rvl)
    ERROR_CHECK(instance->getReverbLevel(index, &rvl));
    return rvl;
}

void Fmod::setEventReverbLevel(const uint64_t instanceId, int index, float level) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setReverbLevel(index, level));
}

bool Fmod::isEventVirtual(const uint64_t instanceId) {
    bool v = false;
    FIND_AND_CHECK(instanceId, events, v)
    ERROR_CHECK(instance->isVirtual(&v));
    return v;
}

bool Fmod::getBusMute(const String busPath) {
    bool mute = false;
    FIND_AND_CHECK(busPath, buses, mute)
    ERROR_CHECK(instance->getMute(&mute));
    return mute;
}

bool Fmod::getBusPaused(const String busPath) {
    bool paused = false;
    FIND_AND_CHECK(busPath, buses, paused)
    ERROR_CHECK(instance->getPaused(&paused));
    return paused;
}

float Fmod::getBusVolume(const String busPath) {
    float volume = 0.0f;
    FIND_AND_CHECK(busPath, buses, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::setBusMute(const String busPath, bool mute) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setMute(mute));
}

void Fmod::setBusPaused(const String busPath, bool paused) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setPaused(paused));
}

void Fmod::setBusVolume(const String busPath, float volume) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->setVolume(volume));
}

void Fmod::stopAllBusEvents(const String busPath, int stopMode) {
    FIND_AND_CHECK(busPath, buses)
    ERROR_CHECK(instance->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void Fmod::loadBankData(LoadingBank *loadingBank){
    char path[MAX_PATH_SIZE];
    auto bank = loadingBank->bank;
    FMOD_RESULT  result = bank->getPath(path, MAX_PATH_SIZE, nullptr);
    if( result == FMOD_OK){
        Godot::print("FMOD Sound System: BANK " + String(path) + " LOADED");
        loadAllBuses(bank);
        loadAllVCAs(bank);
        loadAllEventDescriptions(bank);
        banks[loadingBank->godotPath] << bank;
    }
    else{
        if(result == FMOD_ERR_EVENT_NOTFOUND){
            Godot::print("FMOD Sound System: BANK " + String(path) + " COULDN'T BE LOADED. Path incorrect or MasterBank not loaded yet.");
        }
        else{
            ERROR_CHECK(result);
        }
        bank->unload();
    }
}

void Fmod::loadAllVCAs(FMOD::Studio::Bank *bank) {
    FMOD::Studio::VCA *array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " added to VCAs");
            VCAs[path] << vca;
        }
    }
}

void Fmod::loadAllBuses(FMOD::Studio::Bank *bank) {
    FMOD::Studio::Bus *array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " added to buses");
            buses[path] << bus;
        }
    }
}

void Fmod::loadAllEventDescriptions(FMOD::Studio::Bank *bank) {
    FMOD::Studio::EventDescription *array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " added to eventDescriptions");
            eventDescriptions[path] << eventDescription;
        }
    }
}

void Fmod::unloadAllVCAs(FMOD::Studio::Bank *bank) {
    FMOD::Studio::VCA *array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " removed from VCAs");
            VCAs.erase(path);
        }
    }
}

void Fmod::unloadAllBuses(FMOD::Studio::Bank *bank) {
    FMOD::Studio::Bus *array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " removed from buses");
            buses.erase(path);
        }
    }
}

void Fmod::unloadAllEventDescriptions(FMOD::Studio::Bank *bank) {
    FMOD::Studio::EventDescription *array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            Godot::print("FMOD Sound System: " + String(path) + " removed from eventDescriptions");
            eventDescriptions.erase(path);
        }
    }
}

bool Fmod::checkVCAPath(const String vcaPath) {
    return VCAs.has(vcaPath);
}

bool Fmod::checkBusPath(const String busPath) {
    return buses.has(busPath);
}

bool Fmod::checkEventPath(const String eventPath) {
    return eventDescriptions.has(eventPath);
}

FMOD::Studio::EventInstance *Fmod::createInstance(const String eventName, const bool isOneShot, Object *gameObject) {
    FIND_AND_CHECK(eventName, eventDescriptions, nullptr)
    FMOD::Studio::EventInstance *eventInstance = nullptr;
    ERROR_CHECK(instance->createInstance(&eventInstance));
    if (eventInstance && (!isOneShot || gameObject)) {
        auto *eventInfo = new EventInfo();
        eventInfo->gameObj = gameObject;
        eventInfo->isOneShot = isOneShot;
        eventInstance->setUserData(eventInfo);
        events.append(eventInstance);
    }
    return eventInstance;
}

EventInfo *Fmod::getEventInfo(FMOD::Studio::EventInstance * eventInstance) {
    EventInfo *eventInfo;
    ERROR_CHECK(eventInstance->getUserData((void **)&eventInfo));
    return eventInfo;
}

void Fmod::playOneShot(const String eventName, Object *gameObj) {
    FMOD::Studio::EventInstance *instance = createInstance(eventName, true, nullptr);
    if (instance) {
        // set 3D attributes once
        if (!isNull(gameObj)) {
            updateInstance3DAttributes(instance, gameObj);
        }
        ERROR_CHECK(instance->start());
        ERROR_CHECK(instance->release());
    }
}

void Fmod::playOneShotWithParams(const String eventName, Object *gameObj, const Dictionary parameters) {
    FMOD::Studio::EventInstance *instance = createInstance(eventName, true, nullptr);
    if (instance) {
        // set 3D attributes once
        if (!isNull(gameObj)) {
            updateInstance3DAttributes(instance, gameObj);
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

void Fmod::playOneShotAttached(const String eventName, Object *gameObj) {
    if (!isNull(gameObj)) {
        FMOD::Studio::EventInstance *instance = createInstance(eventName, true, gameObj);
        if (instance) {
            ERROR_CHECK(instance->start());
        }
    }
}

void Fmod::playOneShotAttachedWithParams(const String eventName, Object *gameObj, const Dictionary parameters) {
    if (!isNull(gameObj)) {
        FMOD::Studio::EventInstance *instance = createInstance(eventName, true, gameObj);
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

void Fmod::attachInstanceToNode(const uint64_t instanceId, Object *gameObj) {
    if (isNull(gameObj)) return;
    FIND_AND_CHECK(instanceId, events)
    getEventInfo(instance)->gameObj = gameObj;
}

void Fmod::detachInstanceFromNode(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, events)
    getEventInfo(instance)->gameObj = nullptr;
}

void Fmod::pauseAllEvents(const bool pause) {
    for (int i = 0; i < events.size(); i++) {
        auto eventInstance = events.get(i);
        if (eventInstance) {
            ERROR_CHECK(eventInstance->setPaused(pause));
        }
    }
}

void Fmod::muteAllEvents() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus *masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(true);
        }
    }
}

void Fmod::unmuteAllEvents() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus *masterBus = nullptr;
        if (ERROR_CHECK(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(false);
        }
    }
}

bool Fmod::banksStillLoading() {
    for (int i = 0; i < banks.size(); i++) {
        FMOD::Studio::Bank *bank = banks.get(i);
        FMOD_STUDIO_LOADING_STATE loadingState;
        ERROR_CHECK(bank->getLoadingState(&loadingState));
        if (loadingState == FMOD_STUDIO_LOADING_STATE_LOADING) {
            return true;
        }
    }
    return false;
}

float Fmod::getVCAVolume(const String VCAPath) {
    float volume = 0.0f;
    FIND_AND_CHECK(VCAPath, VCAs, volume)
    ERROR_CHECK(instance->getVolume(&volume));
    return volume;
}

void Fmod::setVCAVolume(const String VCAPath, float volume) {
    FIND_AND_CHECK(VCAPath, VCAs)
    ERROR_CHECK(instance->setVolume(volume));
}

void Fmod::playSound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, sounds)
    FMOD::Channel *channel = instance->channel;
    FMOD::Sound *sound = instance->sound;
    ERROR_CHECK(instance->channel->setPaused(false));
    ERROR_CHECK(coreSystem->playSound(sound, nullptr, true, &channel));
    if (channel) {
        instance->channel = channel;
    }
}

void Fmod::setSoundPaused(const uint64_t instanceId, bool paused) {
    FIND_AND_CHECK(instanceId, sounds)
    ERROR_CHECK(instance->channel->setPaused(paused));
}

void Fmod::stopSound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, sounds)
    ERROR_CHECK(instance->channel->stop());
}

bool Fmod::isSoundPlaying(const uint64_t instanceId) {
    bool isPlaying = false;
    FIND_AND_CHECK(instanceId, sounds, isPlaying)
    ERROR_CHECK(instance->channel->isPlaying(&isPlaying));
    return isPlaying;
}

void Fmod::setSoundVolume(const uint64_t instanceId, float volume) {
    FIND_AND_CHECK(instanceId, sounds)
    ERROR_CHECK(instance->channel->setVolume(volume));
}

float Fmod::getSoundVolume(const uint64_t instanceId) {
    float volume = 0.f;
    FIND_AND_CHECK(instanceId, sounds, volume)
    ERROR_CHECK(instance->channel->getVolume(&volume));
    return volume;
}

float Fmod::getSoundPitch(const uint64_t instanceId) {
    float pitch = 0.f;
    FIND_AND_CHECK(instanceId, sounds, pitch)
    ERROR_CHECK(instance->channel->getPitch(&pitch));
    return pitch;
}

void Fmod::setSoundPitch(const uint64_t instanceId, float pitch) {
    FIND_AND_CHECK(instanceId, sounds)
    ERROR_CHECK(instance->channel->setPitch(pitch));
}

const uint64_t Fmod::loadSound(String path, int mode) {
    DRIVE_PATH(path)
    FMOD::Sound *sound = nullptr;
    ERROR_CHECK(coreSystem->createSound(path.alloc_c_string(), mode, nullptr, &sound));
    if (sound) {
        FMOD::Channel *channel = nullptr;
        ERROR_CHECK(coreSystem->playSound(sound, nullptr, true, &channel));
        auto soundChannel = new SoundChannel();
        soundChannel->sound = sound;
        soundChannel->channel = channel;
        return sounds.append(soundChannel);
    }
    return 0;
}

void Fmod::releaseSound(const uint64_t instanceId) {
    FIND_AND_CHECK(instanceId, sounds)
    ERROR_CHECK(instance->sound->release());
    sounds.erase(instance);
    delete instance;
}

void Fmod::setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale) {
    if (distanceFactor > 0 && ERROR_CHECK(coreSystem->set3DSettings(dopplerScale, distanceFactor, rollOffScale))) {
        distanceScale = distanceFactor;
        Godot::print("FMOD Sound System: Successfully set global 3D settings");
    } else {
        GODOT_ERROR("FMOD Sound System: Failed to set 3D settings :|")
    }
}

void Fmod::waitForAllLoads() {
    ERROR_CHECK(system->flushSampleLoading());
    checkLoadingBanks();
}

Array Fmod::getAvailableDrivers() {
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
        driverInfo["speaker_mode"] = (int)speakerMode;
        driverInfo["number_of_channels"] = speakerModeChannels;
        driverList.push_back(driverInfo);
    }

    return driverList;
}

int Fmod::getDriver() {
    int driverId = -1;
    ERROR_CHECK(coreSystem->getDriver(&driverId));
    return driverId;
}

void Fmod::setDriver(const int id) {
    ERROR_CHECK(coreSystem->setDriver(id));
}

Dictionary Fmod::getPerformanceData() {

    // get the CPU usage
    FMOD_STUDIO_CPU_USAGE cpuUsage;
    ERROR_CHECK(system->getCPUUsage(&cpuUsage));
    Dictionary cpuPerfData = performanceData["CPU"];
    cpuPerfData["dsp"] = cpuUsage.dspusage;
    cpuPerfData["geometry"] = cpuUsage.geometryusage;
    cpuPerfData["stream"] = cpuUsage.streamusage;
    cpuPerfData["studio"] = cpuUsage.studiousage;
    cpuPerfData["update"] = cpuUsage.updateusage;

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
    filePerfData["sample_bytes_read"] = static_cast<int64_t >(sampleBytesRead);
    filePerfData["stream_bytes_read"] = static_cast<int64_t >(streamBytesRead);
    filePerfData["other_bytes_read"] = static_cast<int64_t >(otherBytesRead);

    return performanceData;
}

void Fmod::setGlobalParameterByName(const String parameterName, float value) {
    ERROR_CHECK(system->setParameterByName(parameterName.utf8().get_data(), value));
}

float Fmod::getGlobalParameterByName(String parameterName) {
    float value = 0.f;
    ERROR_CHECK(system->getParameterByName(parameterName.utf8().get_data(), &value));
    return value;
}

void Fmod::setGlobalParameterByID(const Array idPair, const float value) {
    if (idPair.size() != 2) {
        Godot::print_error("Invalid parameter ID", BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
        return;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    ERROR_CHECK(system->setParameterByID(id, value));
}

float Fmod::getGlobalParameterByID(const Array idPair) {
    if (idPair.size() != 2) {
        Godot::print_error("Invalid parameter ID", BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
        return -1.f;
    }
    FMOD_STUDIO_PARAMETER_ID id;
    id.data1 = idPair[0];
    id.data2 = idPair[1];
    float value = -1.f;
    ERROR_CHECK(system->getParameterByID(id, &value));
    return value;
}

void Fmod::setCallback(const uint64_t instanceId, int callbackMask) {
    FIND_AND_CHECK(instanceId, events)
    ERROR_CHECK(instance->setCallback(Callbacks::eventCallback, callbackMask));
    Godot::print(String("CallBack set on event ") + String::num(instanceId, 0));
}

FMOD_RESULT F_CALLBACK Callbacks::eventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters) {
    auto *instance = (FMOD::Studio::EventInstance *)event;
    auto instanceId = (uint64_t) instance;
    EventInfo *eventInfo;
    mut->lock();
    instance->getUserData((void **)&eventInfo);
    if (eventInfo) {
        Callbacks::CallbackInfo *callbackInfo = &eventInfo->callbackInfo;

        if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
            auto *props = (FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *)parameters;
            callbackInfo->markerCallbackInfo["event_id"] = instanceId;
            callbackInfo->markerCallbackInfo["name"] = props->name;
            callbackInfo->markerCallbackInfo["position"] = props->position;
            callbackInfo->markerSignalEmitted = false;
        } else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
            auto *props = (FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES *)parameters;
            callbackInfo->beatCallbackInfo["event_id"] = instanceId;
            callbackInfo->beatCallbackInfo["beat"] = props->beat;
            callbackInfo->beatCallbackInfo["bar"] = props->bar;
            callbackInfo->beatCallbackInfo["tempo"] = props->tempo;
            callbackInfo->beatCallbackInfo["time_signature_upper"] = props->timesignatureupper;
            callbackInfo->beatCallbackInfo["time_signature_lower"] = props->timesignaturelower;
            callbackInfo->beatCallbackInfo["position"] = props->position;
            callbackInfo->beatSignalEmitted = false;
        }
        else if (type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED || type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED) {
            auto *sound = (FMOD::Sound *)parameters;
            char n[256];
            sound->getName(n, 256);
            String name(n);
            String mType = type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED ? "played" : "stopped";
            callbackInfo->soundCallbackInfo["name"] = name;
            callbackInfo->soundCallbackInfo["type"] = mType;
            callbackInfo->soundSignalEmitted = false;
        }
    }
    mut->unlock();
    return FMOD_OK;
}

// runs on the game thread
void Fmod::runCallbacks() {
    Callbacks::mut->lock();
    for (int i = 0; i < events.size(); i++) {
        FMOD::Studio::EventInstance *eventInstance = events.get(i);
        auto eventInfo = getEventInfo(eventInstance);
        if (eventInstance && eventInfo) {
            Callbacks::CallbackInfo *cbInfo = &eventInfo->callbackInfo;
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
                if (cbInfo->soundCallbackInfo["type"] == String("played"))
                    emit_signal("sound_played", cbInfo->soundCallbackInfo);
                else
                    emit_signal("sound_stopped", cbInfo->soundCallbackInfo);
                cbInfo->soundSignalEmitted = true;
            }
        }
        else{
            GODOT_ERROR("A managed event doesn't have an EventInfoStructure")
        }
    }
    Callbacks::mut->unlock();
}

void Fmod::_init() {
    system = nullptr;
    coreSystem = nullptr;
    listener = nullptr;
    isInitialized = false;
    isNotinitPrinted = false;
    Callbacks::mut = Mutex::_new();
    performanceData["CPU"] = Dictionary();
    performanceData["memory"] = Dictionary();
    performanceData["file"] = Dictionary();
    distanceScale = 1.0;
}
