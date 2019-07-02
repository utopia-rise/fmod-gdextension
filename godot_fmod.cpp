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
    register_method("addListener", &Fmod::addListener);
    register_method("setSoftwareFormat", &Fmod::setSoftwareFormat);
    register_method("loadbank", &Fmod::loadbank);
    register_method("unloadBank", &Fmod::unloadBank);
    register_method("getBankLoadingState", &Fmod::getBankLoadingState);
    register_method("getBankBusCount", &Fmod::getBankBusCount);
    register_method("getBankEventCount", &Fmod::getBankEventCount);
    register_method("getBankStringCount", &Fmod::getBankStringCount);
    register_method("getBankVCACount", &Fmod::getBankVCACount);
    register_method("createEventInstance", &Fmod::createEventInstance);
    register_method("getEventParameter", &Fmod::getEventParameter);
    register_method("setEventParameter", &Fmod::setEventParameter);
    register_method("releaseEvent", &Fmod::releaseEvent);
    register_method("startEvent", &Fmod::startEvent);
    register_method("stopEvent", &Fmod::stopEvent);
    register_method("triggerEventCue", &Fmod::triggerEventCue);
    register_method("getEventPlaybackState", &Fmod::getEventPlaybackState);
    register_method("getEventPaused", &Fmod::getEventPaused);
    register_method("setEventPaused", &Fmod::setEventPaused);
    register_method("getEventPitch", &Fmod::getEventPitch);
    register_method("setEventPitch", &Fmod::setEventPitch);
    register_method("getEventVolume", &Fmod::getEventVolume);
    register_method("setEventVolume", &Fmod::setEventVolume);
    register_method("getEventTimelinePosition", &Fmod::getEventTimelinePosition);
    register_method("setEventTimelinePosition", &Fmod::setEventTimelinePosition);
    register_method("getEventReverbLevel", &Fmod::getEventReverbLevel);
    register_method("setEventReverbLevel", &Fmod::setEventReverbLevel);
    register_method("isEventVirtual", &Fmod::isEventVirtual);
    register_method("getBusMute", &Fmod::getBusMute);
    register_method("getBusPaused", &Fmod::getBusPaused);
    register_method("getBusVolume", &Fmod::getBusVolume);
    register_method("setBusMute", &Fmod::setBusMute);
    register_method("setBusPaused", &Fmod::setBusPaused);
    register_method("setBusVolume", &Fmod::setBusVolume);
    register_method("stopAllBusEvents", &Fmod::stopAllBusEvents);
    register_method("getVCAVolume", &Fmod::getVCAVolume);
    register_method("setVCAVolume", &Fmod::setVCAVolume);
    register_method("playOneShot", &Fmod::playOneShot);
    register_method("playOneShotWithParams", &Fmod::playOneShotWithParams);
    register_method("playOneShotAttached", &Fmod::playOneShotAttached);
    register_method("playOneShotAttachedWithParams", &Fmod::playOneShotAttachedWithParams);
    register_method("attachInstanceToNode", &Fmod::attachInstanceToNode);
    register_method("detachInstanceFromNode", &Fmod::detachInstanceFromNode);
    register_method("pauseAllEvents", &Fmod::pauseAllEvents);
    register_method("muteAllEvents", &Fmod::muteAllEvents);
    register_method("unmuteAllEvents", &Fmod::unmuteAllEvents);
    register_method("banksStillLoading", &Fmod::banksStillLoading);
    register_method("loadSound", &Fmod::loadSound);
    register_method("playSound", &Fmod::playSound);
    register_method("stopSound", &Fmod::stopSound);
    register_method("releaseSound", &Fmod::releaseSound);
    register_method("setSoundPaused", &Fmod::setSoundPaused);
    register_method("isSoundPlaying", &Fmod::isSoundPlaying);
    register_method("setSoundVolume", &Fmod::setSoundVolume);
    register_method("getSoundVolume", &Fmod::getSoundVolume);
    register_method("setSoundPitch", &Fmod::setSoundPitch);
    register_method("getSoundPitch", &Fmod::getSoundPitch);
    register_method("setCallback", &Fmod::setCallback);
    register_method("setSound3DSettings", &Fmod::setSound3DSettings);
    register_method("waitForAllLoads", &Fmod::waitForAllLoads);
    register_method("getAvailableDrivers", &Fmod::getAvailableDrivers);
    register_method("getDriver", &Fmod::getDriver);
    register_method("setDriver", &Fmod::setDriver);
    register_method("getPerformanceData", &Fmod::getPerformanceData);
    register_method("setGlobalParameter", &Fmod::setGlobalParameter);
    register_method("getGlobalParameter", &Fmod::getGlobalParameter);
    register_method("_process", &Fmod::_process);

    register_signal<Fmod>("timeline_beat", "params", GODOT_VARIANT_TYPE_DICTIONARY);
    register_signal<Fmod>("timeline_marker", "params", GODOT_VARIANT_TYPE_DICTIONARY);
}

void Fmod::init(int numOfChannels, const unsigned int studioFlag, const unsigned int flag) {
    // initialize FMOD Studio and FMOD Low Level System with provided flags
    if (checkErrors(system->initialize(numOfChannels, studioFlag, flag, nullptr))) {
        isInitialized = true;
        Godot::print("FMOD Sound System: Successfully initialized");
        if (studioFlag == FMOD_STUDIO_INIT_LIVEUPDATE) {
            Godot::print("FMOD Sound System: Live update enabled!");
        }
    } else {
        Godot::print_error("FMOD Sound System: Failed to initialize :|", "init", __FILE__, __LINE__);
    }
}

int Fmod::checkErrors(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        Godot::print_error(FMOD_ErrorString(result), "checkErrors", __FILE__, __LINE__);
        return 0;
    }
    return 1;
}

void Fmod::setListenerAttributes() {
    if (isNull(listener)) {
        if (nullListenerWarning) {
            Godot::print_error("FMOD Sound System: Listener not set!", "setListenerAttributes", __FILE__, __LINE__);
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
        checkErrors(system->setListenerAttributes(0, &attr));

    } else {
        // needs testing
        auto *s = Object::cast_to<Spatial>(listener);
        Transform t = s->get_transform();
        Vector3 pos = t.get_origin() / distanceScale;
        Vector3 up = t.get_basis().elements[1];
        Vector3 forward = t.get_basis().elements[2];
        Vector3 vel(0, 0, 0);
        auto attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
        checkErrors(system->setListenerAttributes(0, &attr));
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
            checkErrors(instance->set3DAttributes(&attr));
        } else {
            // needs testing
            auto *s = Object::cast_to<Spatial>(o);
            Transform t = s->get_transform();
            Vector3 pos = t.get_origin() / distanceScale;
            Vector3 up = t.get_basis().elements[1];
            Vector3 forward = t.get_basis().elements[2];
            Vector3 vel(0, 0, 0);
            FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
            checkErrors(instance->set3DAttributes(&attr));
        }
    }
}

void Fmod::shutdown() {
    checkErrors(system->unloadAll());
    checkErrors(system->release());
}

void Fmod::addListener(Object *gameObj) {
    listener = gameObj;
}

void Fmod::setSoftwareFormat(int sampleRate, const int speakerMode, int numRawSpeakers) {
    checkErrors(coreSystem->setSoftwareFormat(sampleRate, static_cast<FMOD_SPEAKERMODE>(speakerMode), numRawSpeakers));
}

String Fmod::loadbank(const String pathToBank, const unsigned int flag) {
    if (banks.count(pathToBank)) return pathToBank; // bank is already loaded
    FMOD::Studio::Bank *bank = nullptr;
    checkErrors(system->loadBankFile(pathToBank.alloc_c_string(), flag, &bank));
    if (bank) {
        banks[pathToBank] = bank;
        return pathToBank;
    }
    return pathToBank;
}

void Fmod::unloadBank(const String pathToBank) {
    if (!banks.count(pathToBank)) return; // bank is not loaded
    auto bankIt = banks.find(pathToBank);
    if (bankIt != banks.end()) {
        checkErrors(bankIt->second->unload());
        banks.erase(pathToBank);
    }
}

int Fmod::getBankLoadingState(const String pathToBank) {
    if (!banks.count(pathToBank)) return -1; // bank is not loaded
    auto bankIt = banks.find(pathToBank);
    if (bankIt != banks.end()) {
        FMOD_STUDIO_LOADING_STATE state;
        checkErrors(bankIt->second->getLoadingState(&state));
        return state;
    }
    return -1;
}

int Fmod::getBankBusCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count = -1;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getBusCount(&count));
        return count;
    }
    return -1;
}

int Fmod::getBankEventCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count = -1;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getEventCount(&count));
        return count;
    }
    return -1;
}

int Fmod::getBankStringCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count = -1;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getStringCount(&count));
        return count;
    }
    return -1;
}

int Fmod::getBankVCACount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count = -1;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getVCACount(&count));
        return count;
    }
    return -1;
}

const uint64_t Fmod::createEventInstance(String eventPath) {
    FMOD::Studio::EventInstance *instance = createInstance(eventPath, false, nullptr);
    if (instance) {
        const auto instanceId = (uint64_t)instance;
        events[instanceId] = instance;
        return instanceId;
    }
    return 0;
}

float Fmod::getEventParameter(const uint64_t instanceId, String parameterName) {
    float p = -1;
    if (!events.count(instanceId)) return p;
    auto i = events.find(instanceId);
    if (i != events.end())
        checkErrors(i->second->getParameterByName(parameterName.ascii().get_data(), &p));
    return p;
}

void Fmod::setEventParameter(const uint64_t instanceId, String parameterName, float value) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->setParameterByName(parameterName.ascii().get_data(), value));
}

void Fmod::releaseEvent(const uint64_t instanceId) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) {
        releaseOneEvent(i->second);
    }
}

void Fmod::releaseOneEvent(FMOD::Studio::EventInstance *eventInstance) {
    Callbacks::mut->lock();
    EventInfo *eventInfo = getEventInfo(eventInstance);
    eventInstance->setUserData(nullptr);
    checkErrors(eventInstance->release());
    events.erase((uint64_t) eventInstance);
    delete &eventInfo;
    Callbacks::mut->unlock();
}

void Fmod::startEvent(const uint64_t instanceId) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->start());
}

void Fmod::stopEvent(const uint64_t instanceId, int stopMode) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) {
        checkErrors(i->second->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
    }
}

void Fmod::triggerEventCue(const uint64_t instanceId) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->triggerCue());
}

int Fmod::getEventPlaybackState(const uint64_t instanceId) {
    if (!events.count(instanceId))
        return -1;
    else {
        auto i = events.find(instanceId);
        if (i != events.end()) {
            FMOD_STUDIO_PLAYBACK_STATE s;
            checkErrors(i->second->getPlaybackState(&s));
            return s;
        }
        return -1;
    }
}

bool Fmod::getEventPaused(const uint64_t instanceId) {
    if (!events.count(instanceId)) return false;
    auto i = events.find(instanceId);
    bool paused = false;
    if (i != events.end()) checkErrors(i->second->getPaused(&paused));
    return paused;
}

void Fmod::setEventPaused(const uint64_t instanceId, bool paused) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->setPaused(paused));
}

float Fmod::getEventPitch(const uint64_t instanceId) {
    if (!events.count(instanceId)) return 0.0f;
    auto i = events.find(instanceId);
    float pitch = 0.0f;
    if (i != events.end()) checkErrors(i->second->getPitch(&pitch));
    return pitch;
}

void Fmod::setEventPitch(const uint64_t instanceId, float pitch) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->setPitch(pitch));
}

float Fmod::getEventVolume(const uint64_t instanceId) {
    if (!events.count(instanceId)) return 0.0f;
    auto i = events.find(instanceId);
    float volume = 0.0f;
    if (i != events.end()) {
        FMOD::Studio::EventInstance *event = i->second;
        checkErrors(event->getVolume(&volume));
    }
    return volume;
}

void Fmod::setEventVolume(const uint64_t instanceId, float volume) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) {
        FMOD::Studio::EventInstance *event = i->second;
        checkErrors(event->setVolume(volume));
    }
}

int Fmod::getEventTimelinePosition(const uint64_t instanceId) {
    if (!events.count(instanceId)) return 0;
    auto i = events.find(instanceId);
    int tp = 0;
    if (i != events.end()) checkErrors(i->second->getTimelinePosition(&tp));
    return tp;
}

void Fmod::setEventTimelinePosition(const uint64_t instanceId, int position) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->setTimelinePosition(position));
}

float Fmod::getEventReverbLevel(const uint64_t instanceId, int index) {
    if (!events.count(instanceId)) return 0.0f;
    auto i = events.find(instanceId);
    float rvl = 0.0f;
    if (i != events.end()) checkErrors(i->second->getReverbLevel(index, &rvl));
    return rvl;
}

void Fmod::setEventReverbLevel(const uint64_t instanceId, int index, float level) {
    if (!events.count(instanceId)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) checkErrors(i->second->setReverbLevel(index, level));
}

bool Fmod::isEventVirtual(const uint64_t instanceId) {
    if (!events.count(instanceId)) return false;
    auto i = events.find(instanceId);
    bool v = false;
    if (i != events.end()) checkErrors(i->second->isVirtual(&v));
    return v;
}

bool Fmod::getBusMute(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return false;
    bool mute = false;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getMute(&mute));
    return mute;
}

bool Fmod::getBusPaused(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return false;
    bool paused = false;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getPaused(&paused));
    return paused;
}

float Fmod::getBusVolume(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return 0.0f;
    float volume = 0.0f;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getVolume(&volume));
    return volume;
}

void Fmod::setBusMute(const String busPath, bool mute) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setMute(mute));
}

void Fmod::setBusPaused(const String busPath, bool paused) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setPaused(paused));
}

void Fmod::setBusVolume(const String busPath, float volume) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setVolume(volume));
}

void Fmod::stopAllBusEvents(const String busPath, int stopMode) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    checkErrors(buses.find(busPath)->second->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void Fmod::loadBus(const String &busPath) {
    if (!buses.count(busPath)) {
        FMOD::Studio::Bus *b = nullptr;
        checkErrors(system->getBus(busPath.ascii().get_data(), &b));
        if (b) buses[busPath] = b;
    }
}

void Fmod::loadVCA(const String &VCAPath) {
    if (!VCAs.count(VCAPath)) {
        FMOD::Studio::VCA *vca = nullptr;
        checkErrors(system->getVCA(VCAPath.ascii().get_data(), &vca));
        if (vca) VCAs[VCAPath] = vca;
    }
}

FMOD::Studio::EventInstance *Fmod::createInstance(const String eventName, const bool isOneShot, Object *gameObject) {
    if (!eventDescriptions.count(eventName)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventName.alloc_c_string(), &desc));
        eventDescriptions[eventName] = desc;
    }
    auto descIt = eventDescriptions.find(eventName);
    FMOD::Studio::EventInstance *instance;
    checkErrors(descIt->second->createInstance(&instance));
    if (instance && (!isOneShot || gameObject)) {
        auto *eventInfo = new EventInfo();
        eventInfo->gameObj = gameObject;
        eventInfo->isOneShot = isOneShot;
        instance->setUserData(eventInfo);
        auto instanceId = (uint64_t)instance;
        events[instanceId] = instance;
    }
    return instance;
}

Fmod::EventInfo *Fmod::getEventInfo(FMOD::Studio::EventInstance * eventInstance) {
    EventInfo *eventInfo;
    eventInstance->getUserData((void **)&eventInfo);
    return eventInfo;
}

void Fmod::playOneShot(const String eventName, Object *gameObj) {
    FMOD::Studio::EventInstance *instance = createInstance(eventName, true, nullptr);
    if (instance) {
        // set 3D attributes once
        if (!isNull(gameObj)) {
            updateInstance3DAttributes(instance, gameObj);
        }
        checkErrors(instance->start());
        checkErrors(instance->release());
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
            checkErrors(instance->setParameterByName(k.ascii().get_data(), v));
        }
        checkErrors(instance->start());
        checkErrors(instance->release());
    }
}

void Fmod::playOneShotAttached(const String eventName, Object *gameObj) {
    if (!isNull(gameObj)) {
        FMOD::Studio::EventInstance *instance = createInstance(eventName, true, gameObj);
        if (instance) {
            checkErrors(instance->start());
            checkErrors(instance->release());
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
                checkErrors(instance->setParameterByName(k.ascii().get_data(), v));
            }
            checkErrors(instance->start());
            checkErrors(instance->release());
        }
    }
}

void Fmod::attachInstanceToNode(const uint64_t instanceId, Object *gameObj) {
    if (!events.count(instanceId) || isNull(gameObj)) return;
    auto i = events.find(instanceId);
    if (i != events.end()) {
        EventInfo *eventInfo = getEventInfo(i->second);
        eventInfo->gameObj = gameObj;
    }
}

void Fmod::detachInstanceFromNode(const uint64_t instanceId) {
    if (!events.count(instanceId)) return;
    auto iterator = events.find(instanceId);
    if (iterator != events.end()) {
        EventInfo *eventInfo = getEventInfo(iterator->second);
        eventInfo->gameObj = nullptr;
    }
}

void Fmod::pauseAllEvents(const bool pause) {
    for (auto &it : events) {
        checkErrors(it.second->setPaused(pause));
    }
}

void Fmod::muteAllEvents() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus *masterBus = nullptr;
        if (checkErrors(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(true);
        }
    }
}

void Fmod::unmuteAllEvents() {
    if (banks.size() > 1) {
        FMOD::Studio::Bus *masterBus = nullptr;
        if (checkErrors(system->getBus("bus:/", &masterBus))) {
            masterBus->setMute(false);
        }
    }
}

bool Fmod::banksStillLoading() {
    for (auto &it : banks) {
        FMOD::Studio::Bank *bank = it.second;
        FMOD_STUDIO_LOADING_STATE loadingState;
        checkErrors(bank->getLoadingState(&loadingState));
        if (loadingState == FMOD_STUDIO_LOADING_STATE_LOADING) {
            return true;
        }
    }
    return false;
}

float Fmod::getVCAVolume(const String VCAPath) {
    loadVCA(VCAPath);
    if (!VCAs.count(VCAPath)) return 0.0f;
    auto vca = VCAs.find(VCAPath);
    float volume = 0.0f;
    checkErrors(vca->second->getVolume(&volume));
    return volume;
}

void Fmod::setVCAVolume(const String VCAPath, float volume) {
    loadVCA(VCAPath);
    if (!VCAs.count(VCAPath)) return;
    auto vca = VCAs.find(VCAPath);
    checkErrors(vca->second->setVolume(volume));
}

void Fmod::playSound(const uint64_t instanceId) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPaused(false));
    }
}

void Fmod::setSoundPaused(const uint64_t instanceId, bool paused) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPaused(paused));
    }
}

void Fmod::stopSound(const uint64_t instanceId) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->stop());
    }
}

bool Fmod::isSoundPlaying(const uint64_t instanceId) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        bool isPlaying = false;
        checkErrors(c->isPlaying(&isPlaying));
        return isPlaying;
    }
    return false;
}

void Fmod::setSoundVolume(const uint64_t instanceId, float volume) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setVolume(volume));
    }
}

float Fmod::getSoundVolume(const uint64_t instanceId) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        float volume = 0.f;
        checkErrors(c->getVolume(&volume));
        return volume;
    }
    return 0.f;
}

float Fmod::getSoundPitch(const uint64_t instanceId) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        float pitch = 0.f;
        checkErrors(c->getPitch(&pitch));
        return pitch;
    }
    return 0.f;
}

void Fmod::setSoundPitch(const uint64_t instanceId, float pitch) {
    if (sounds.count(instanceId)) {
        auto s = sounds.find(instanceId)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPitch(pitch));
    }
}

const uint64_t Fmod::loadSound(String path, int mode) {
    FMOD::Sound *sound = nullptr;
    checkErrors(coreSystem->createSound(path.alloc_c_string(), static_cast<FMOD_MODE>(mode), nullptr, &sound));
    if (sound) {
        const auto instanceId = (uint64_t)sound;
        sounds[instanceId] = sound;
        FMOD::Channel *channel = nullptr;
        checkErrors(coreSystem->playSound(sound, nullptr, true, &channel));
        if (channel) {
            channels[sound] = channel;
            return instanceId;
        }
    }
    return 0;
}

void Fmod::releaseSound(const uint64_t instanceId) {
    if (!sounds.count(instanceId)) return; // sound is not loaded
    auto sound = sounds.find(instanceId);
    if (sound->second) {
        checkErrors(sound->second->release());
        sounds.erase(instanceId);
    }
}

void Fmod::setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale) {
    if (distanceFactor > 0 && checkErrors(coreSystem->set3DSettings(dopplerScale, distanceFactor, rollOffScale))) {
        distanceScale = distanceFactor;
        Godot::print("FMOD Sound System: Successfully set global 3D settings");
    } else {
        Godot::print_error("FMOD Sound System: Failed to set 3D settings :|", "Fmod::setSound3DSettings", __FILE__, __LINE__);
    }
}

void Fmod::waitForAllLoads() {
    checkErrors(system->flushSampleLoading());
}

Array Fmod::getAvailableDrivers() {
    Array driverList;
    int numDrivers = 0;

    checkErrors(coreSystem->getNumDrivers(&numDrivers));

    for (int i = 0; i < numDrivers; i++) {
        char name[256];
        int sampleRate;
        FMOD_SPEAKERMODE speakerMode;
        int speakerModeChannels;
        checkErrors(coreSystem->getDriverInfo(i, name, 256, nullptr, &sampleRate, &speakerMode, &speakerModeChannels));
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
    checkErrors(coreSystem->getDriver(&driverId));
    return driverId;
}

void Fmod::setDriver(const int id) {
    checkErrors(coreSystem->setDriver(id));
}

Dictionary Fmod::getPerformanceData() {

    // get the CPU usage
    FMOD_STUDIO_CPU_USAGE cpuUsage;
    checkErrors(system->getCPUUsage(&cpuUsage));
    Dictionary cpuPerfData = performanceData["CPU"];
    cpuPerfData["dsp"] = cpuUsage.dspusage;
    cpuPerfData["geometry"] = cpuUsage.geometryusage;
    cpuPerfData["stream"] = cpuUsage.streamusage;
    cpuPerfData["studio"] = cpuUsage.studiousage;
    cpuPerfData["update"] = cpuUsage.updateusage;

    // get the memory usage
    int currentAlloc = 0;
    int maxAlloc = 0;
    checkErrors(FMOD::Memory_GetStats(&currentAlloc, &maxAlloc));
    Dictionary memPerfData = performanceData["memory"];
    memPerfData["currently_allocated"] = currentAlloc;
    memPerfData["max_allocated"] = maxAlloc;

    // get the file usage
    long long sampleBytesRead = 0;
    long long streamBytesRead = 0;
    long long otherBytesRead = 0;
    checkErrors(coreSystem->getFileUsage(&sampleBytesRead, &streamBytesRead, &otherBytesRead));
    Dictionary filePerfData = performanceData["file"];
    filePerfData["sample_bytes_read"] = static_cast<int64_t >(sampleBytesRead);
    filePerfData["stream_bytes_read"] = static_cast<int64_t >(streamBytesRead);
    filePerfData["other_bytes_read"] = static_cast<int64_t >(otherBytesRead);

    return performanceData;
}

void Fmod::setGlobalParameter(const String parameterName, float value) {
    checkErrors(system->setParameterByName(parameterName.ascii().get_data(), value));
}

float Fmod::getGlobalParameter(const String parameterName) {
    float value = 0.f;
    checkErrors(system->getParameterByName(parameterName.ascii().get_data(), &value));
    return value;
}

void Fmod::setCallback(const uint64_t instanceId, int callbackMask) {
    if (!events.count(instanceId)) return;
    FMOD::Studio::EventInstance *event = events.find(instanceId)->second;
    if (event) {
        checkErrors(event->setCallback(Callbacks::eventCallback, callbackMask));
    }
}

FMOD_RESULT F_CALLBACK Callbacks::eventCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters) {
    auto *instance = (FMOD::Studio::EventInstance *)event;
    auto instanceId = (uint64_t)instance;
    Fmod::EventInfo *eventInfo;
    mut->lock();
    instance->getUserData((void **)&eventInfo);
    if (eventInfo) {
        Callbacks::CallbackInfo callbackInfo = eventInfo->callbackInfo;

        if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
            auto *props = (FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES *)parameters;
            callbackInfo.markerCallbackInfo["event_id"] = instanceId;
            callbackInfo.markerCallbackInfo["name"] = props->name;
            callbackInfo.markerCallbackInfo["position"] = props->position;
            callbackInfo.markerSignalEmitted = false;
        } else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
            auto *props = (FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES *)parameters;
            callbackInfo.beatCallbackInfo["event_id"] = instanceId;
            callbackInfo.beatCallbackInfo["beat"] = props->beat;
            callbackInfo.beatCallbackInfo["bar"] = props->bar;
            callbackInfo.beatCallbackInfo["tempo"] = props->tempo;
            callbackInfo.beatCallbackInfo["time_signature_upper"] = props->timesignatureupper;
            callbackInfo.beatCallbackInfo["time_signature_lower"] = props->timesignaturelower;
            callbackInfo.beatCallbackInfo["position"] = props->position;
            callbackInfo.beatSignalEmitted = false;
        }
        else if (type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED || type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED) {
            auto *sound = (FMOD::Sound *)parameters;
            char n[256];
            sound->getName(n, 256);
            String name(n);
            String mType = type == FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED ? "played" : "stopped";
            callbackInfo.soundCallbackInfo["name"] = name;
            callbackInfo.soundCallbackInfo["type"] = mType;
            callbackInfo.soundSignalEmitted = false;
        }
    }
    mut->unlock();
    return FMOD_OK;
}

// runs on the game thread
void Fmod::runCallbacks() {
    Callbacks::mut->lock();
    for (auto e : events) {
        FMOD::Studio::EventInstance *eventInstance = e.second;
        Callbacks::CallbackInfo cbInfo = getEventInfo(eventInstance)->callbackInfo;
        // check for Marker callbacks
        if (!cbInfo.markerSignalEmitted) {
            emit_signal("timeline_marker", cbInfo.markerCallbackInfo);
            cbInfo.markerSignalEmitted = true;
        }

        // check for Beat callbacks
        if (!cbInfo.beatSignalEmitted) {
            emit_signal("timeline_beat", cbInfo.beatCallbackInfo);
            cbInfo.beatSignalEmitted = true;
        }

        // check for Sound callbacks
        if (!cbInfo.soundSignalEmitted) {
            if (cbInfo.soundCallbackInfo["type"] == String("played"))
                emit_signal("sound_played", cbInfo.soundCallbackInfo);
            else
                emit_signal("sound_stopped", cbInfo.soundCallbackInfo);
            cbInfo.soundSignalEmitted = true;
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
    checkErrors(FMOD::Studio::System::create(&system));
    checkErrors(system->getCoreSystem(&coreSystem));
    distanceScale = 1.0;
}

void Fmod::_process(const float delta) {
    if (!isInitialized) {
        if (!isNotinitPrinted) {
            Godot::print_error("FMOD Sound System: Fmod should be initialized before calling update", "update", __FILE__, __LINE__);
            isNotinitPrinted = true;
        }
        return;
    }
    for (auto i : events) {
        FMOD::Studio::EventInstance *eventInstance = i.second;
        EventInfo *eventInfo = getEventInfo(eventInstance);
        if (eventInfo->gameObj) {
            if (isNull(eventInfo->gameObj)) {
                FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
                checkErrors(eventInstance->stop(m));
                releaseOneEvent(eventInstance);
                continue;
            }
            if (eventInfo->isOneShot) {
                FMOD_STUDIO_PLAYBACK_STATE s;
                checkErrors(eventInstance->getPlaybackState(&s));
                if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
                    releaseOneEvent(eventInstance);
                    continue;
                }
            }
            updateInstance3DAttributes(eventInstance, eventInfo->gameObj);
        }
    }

    // update listener position
    setListenerAttributes();

    //run callback events
    runCallbacks();

    // dispatch update to FMOD
    checkErrors(system->update());
}
