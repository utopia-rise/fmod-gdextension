//
// Created by Pierre-Thomas Meisels on 2019-01-01.
//

#include "godot_fmod.h"

using namespace godot;

GodotFmod::GodotFmod() = default;

GodotFmod::~GodotFmod() {
    GodotFmod::shutdown();
}

void GodotFmod::_register_methods() {
    register_method("init", &GodotFmod::init);
    register_method("update", &GodotFmod::update);
    register_method("shutdown", &GodotFmod::shutdown);
    register_method("addListener", &GodotFmod::addListener);
    register_method("setSoftwareFormat", &GodotFmod::setSoftwareFormat);
    register_method("loadbank", &GodotFmod::loadbank);
    register_method("unloadBank", &GodotFmod::unloadBank);
    register_method("getBankLoadingState", &GodotFmod::getBankLoadingState);
    register_method("getBankBusCount", &GodotFmod::getBankBusCount);
    register_method("getBankEventCount", &GodotFmod::getBankEventCount);
    register_method("getBankStringCount", &GodotFmod::getBankStringCount);
    register_method("getBankVCACount", &GodotFmod::getBankVCACount);
    register_method("createEventInstance", &GodotFmod::createEventInstance);
    register_method("getEventParameter", &GodotFmod::getEventParameter);
    register_method("setEventParameter", &GodotFmod::setEventParameter);
    register_method("releaseEvent", &GodotFmod::releaseEvent);
    register_method("startEvent", &GodotFmod::startEvent);
    register_method("stopEvent", &GodotFmod::stopEvent);
    register_method("triggerEventCue", &GodotFmod::triggerEventCue);
    register_method("getEventPlaybackState", &GodotFmod::getEventPlaybackState);
    register_method("getEventPaused", &GodotFmod::getEventPaused);
    register_method("setEventPaused", &GodotFmod::setEventPaused);
    register_method("getEventPitch", &GodotFmod::getEventPitch);
    register_method("setEventPitch", &GodotFmod::setEventPitch);
    register_method("getEventVolume", &GodotFmod::getEventVolume);
    register_method("setEventVolume", &GodotFmod::setEventVolume);
    register_method("getEventTimelinePosition", &GodotFmod::getEventTimelinePosition);
    register_method("setEventTimelinePosition", &GodotFmod::setEventTimelinePosition);
    register_method("getEventReverbLevel", &GodotFmod::getEventReverbLevel);
    register_method("setEventReverbLevel", &GodotFmod::setEventReverbLevel);
    register_method("isEventVirtual", &GodotFmod::isEventVirtual);
    register_method("getBusMute", &GodotFmod::getBusMute);
    register_method("getBusPaused", &GodotFmod::getBusPaused);
    register_method("getBusVolume", &GodotFmod::getBusVolume);
    register_method("setBusMute", &GodotFmod::setBusMute);
    register_method("setBusPaused", &GodotFmod::setBusPaused);
    register_method("setBusVolume", &GodotFmod::setBusVolume);
    register_method("stopAllBusEvents", &GodotFmod::stopAllBusEvents);
    register_method("getVCAVolume", &GodotFmod::getVCAVolume);
    register_method("setVCAVolume", &GodotFmod::setVCAVolume);
    register_method("playOneShot", &GodotFmod::playOneShot);
    register_method("playOneShotWithParams", &GodotFmod::playOneShotWithParams);
    register_method("playOneShotAttached", &GodotFmod::playOneShotAttached);
    register_method("playOneShotAttachedWithParams", &GodotFmod::playOneShotAttachedWithParams);
    register_method("attachInstanceToNode", &GodotFmod::attachInstanceToNode);
    register_method("detachInstanceFromNode", &GodotFmod::detachInstanceFromNode);
    register_method("loadSound", &GodotFmod::loadSound);
    register_method("playSound", &GodotFmod::playSound);
    register_method("stopSound", &GodotFmod::stopSound);
    register_method("releaseSound", &GodotFmod::releaseSound);
    register_method("setSoundPaused", &GodotFmod::setSoundPaused);
    register_method("isSoundPlaying", &GodotFmod::isSoundPlaying);
    register_method("setSoundVolume", &GodotFmod::setSoundVolume);
    register_method("getSoundVolume", &GodotFmod::getSoundVolume);
    register_method("setSoundPitch", &GodotFmod::setSoundPitch);
    register_method("getSoundPitch", &GodotFmod::getSoundPitch);
    register_method("setSound3DSettings", &GodotFmod::setSound3DSettings);
}

void GodotFmod::init(int numOfChannels, const unsigned int studioFlag, const unsigned int flag) {
    // initialize FMOD Studio and FMOD Low Level System with provided flags
    if (checkErrors(system->initialize(numOfChannels, studioFlag, flag, nullptr))) {
        Godot::print("FMOD Sound System: Successfully initialized");
        if (studioFlag == FMOD_STUDIO_INIT_LIVEUPDATE) {
            Godot::print("FMOD Sound System: Live update enabled!");
        }
    } else {
        Godot::print_error("FMOD Sound System: Failed to initialize :|", "init", __FILE__, __LINE__);
    }
}

int GodotFmod::checkErrors(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        Godot::print_error(FMOD_ErrorString(result), "checkErrors", __FILE__, __LINE__);
        return 0;
    }
    return 1;
}

void GodotFmod::update() {
    // clean up one shots
    for (int i = 0; i < oneShotInstances.size(); i++) {
        auto instance = oneShotInstances[i];
        FMOD_STUDIO_PLAYBACK_STATE s;
        checkErrors(instance->getPlaybackState(&s));
        if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
            checkErrors(instance->release());
            oneShotInstances.erase(oneShotInstances.begin() + i);
            i--;
        }
    }

    // update and clean up attached one shots
    for (int i = 0; i < attachedOneShots.size(); i++) {
        auto aShot = attachedOneShots[i];
        if (isNull(aShot.gameObj)) {
            FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
            checkErrors(aShot.instance->stop(m));
            checkErrors(aShot.instance->release());
            attachedOneShots.erase(attachedOneShots.begin() + i);
            i--;
            continue;
        }
        FMOD_STUDIO_PLAYBACK_STATE s;
        checkErrors(aShot.instance->getPlaybackState(&s));
        if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
            checkErrors(aShot.instance->release());
            attachedOneShots.erase(attachedOneShots.begin() + i);
            i--;
            continue;
        }
        updateInstance3DAttributes(aShot.instance, aShot.gameObj);
    }

    // update listener position
    setListenerAttributes();

    // dispatch update to FMOD
    checkErrors(system->update());
}

void GodotFmod::setListenerAttributes() {
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
        Vector3 pos(posVector.x, posVector.y, 0.0f),
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

FMOD_VECTOR GodotFmod::toFmodVector(Vector3 &vec) {
    FMOD_VECTOR fv;
    fv.x = vec.x;
    fv.y = vec.y;
    fv.z = vec.z;
    return fv;
}

FMOD_3D_ATTRIBUTES GodotFmod::get3DAttributes(const FMOD_VECTOR &pos, const FMOD_VECTOR &up, const FMOD_VECTOR &forward,
                                              const FMOD_VECTOR &vel) {
    FMOD_3D_ATTRIBUTES f3d;
    f3d.forward = forward;
    f3d.position = pos;
    f3d.up = up;
    f3d.velocity = vel;
    return f3d;
}

bool GodotFmod::isNull(Object *o) {
    auto *ci = Object::cast_to<CanvasItem>(o);
    auto *s = Object::cast_to<Spatial>(o);
    return ci == nullptr && s == nullptr;
}

void GodotFmod::updateInstance3DAttributes(FMOD::Studio::EventInstance *instance, Object *o) {
    // try to set 3D attributes
    if (instance && !isNull(o)) {
        auto *ci = Object::cast_to<CanvasItem>(o);
        if (ci != nullptr) {
            Transform2D t2d = ci->get_transform();
            Vector2 posVector = t2d.get_origin() / distanceScale;
            Vector3 pos(posVector.x, posVector.y, 0.0f),
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

void GodotFmod::shutdown() {
    checkErrors(system->unloadAll());
    checkErrors(system->release());
}

void GodotFmod::addListener(Object *gameObj) {
    listener = gameObj;
}

void GodotFmod::setSoftwareFormat(int sampleRate, const int speakerMode, int numRawSpeakers) {
    checkErrors(lowLevelSystem->setSoftwareFormat(sampleRate, static_cast<FMOD_SPEAKERMODE>(speakerMode), numRawSpeakers));
}

String GodotFmod::loadbank(const String pathToBank, const unsigned int flag) {
    if (banks.count(pathToBank)) return pathToBank; // bank is already loaded
    FMOD::Studio::Bank *bank = nullptr;
    checkErrors(system->loadBankFile(pathToBank.alloc_c_string(), flag, &bank));
    if (bank) {
        banks[pathToBank] = bank;
        return pathToBank;
    }
    return pathToBank;
}

void GodotFmod::unloadBank(const String pathToBank) {
    if (!banks.count(pathToBank)) return; // bank is not loaded
    auto bankIt = banks.find(pathToBank);
    if (bankIt != banks.end()) checkErrors(bankIt->second->unload());
}

int GodotFmod::getBankLoadingState(const String pathToBank) {
    if (!banks.count(pathToBank)) return -1; // bank is not loaded
    auto bankIt = banks.find(pathToBank);
    if (bankIt != banks.end()) {
        FMOD_STUDIO_LOADING_STATE state;
        checkErrors(bankIt->second->getLoadingState(&state));
        return state;
    }
    return -1;
}

int GodotFmod::getBankBusCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getBusCount(&count));
        return count;
    }
    return -1;
}

int GodotFmod::getBankEventCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getEventCount(&count));
        return count;
    }
    return -1;
}

int GodotFmod::getBankStringCount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getStringCount(&count));
        return count;
    }
    return -1;
}

int GodotFmod::getBankVCACount(const String pathToBank) {
    if (banks.count(pathToBank)) {
        int count;
        auto bankIt = banks.find(pathToBank);
        if (bankIt != banks.end()) checkErrors(bankIt->second->getVCACount(&count));
        return count;
    }
    return -1;
}

String GodotFmod::createEventInstance(const String uuid, const String eventPath) {
    if (unmanagedEvents.count(uuid)) return uuid; // provided uuid is not valid
    if (!eventDescriptions.count(eventPath)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventPath.alloc_c_string(), &desc));
        eventDescriptions[eventPath] = desc;
    }
    auto descIt = eventDescriptions.find(eventPath);
    FMOD::Studio::EventInstance *instance;
    checkErrors(descIt->second->createInstance(&instance));
    if (instance) {
        unmanagedEvents[uuid] = instance;
    }
    return uuid;
}

float GodotFmod::getEventParameter(const String uuid, const String parameterName) {
    float p = -1;
    if (!unmanagedEvents.count(uuid)) return p;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end())
        checkErrors(i->second->getParameterValue(parameterName.ascii().get_data(), &p));
    return p;
}

void GodotFmod::setEventParameter(const String uuid, const String parameterName, float value) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setParameterValue(parameterName.ascii().get_data(), value));
}

void GodotFmod::releaseEvent(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->release());
}

void GodotFmod::startEvent(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->start());
}

void GodotFmod::stopEvent(const String uuid, const int stopMode) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) {
        checkErrors(i->second->stop(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
    }
}

void GodotFmod::triggerEventCue(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->triggerCue());
}

int GodotFmod::getEventPlaybackState(const String uuid) {
    if (!unmanagedEvents.count(uuid))
        return -1;
    else {
        auto i = unmanagedEvents.find(uuid);
        if (i != unmanagedEvents.end()) {
            FMOD_STUDIO_PLAYBACK_STATE s;
            checkErrors(i->second->getPlaybackState(&s));
            return s;
        }
        return -1;
    }
}

bool GodotFmod::getEventPaused(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return false;
    auto i = unmanagedEvents.find(uuid);
    bool paused = false;
    if (i != unmanagedEvents.end()) checkErrors(i->second->getPaused(&paused));
    return paused;
}

void GodotFmod::setEventPaused(const String uuid, bool paused) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setPaused(paused));
}

float GodotFmod::getEventPitch(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return 0.0f;
    auto i = unmanagedEvents.find(uuid);
    float pitch = 0.0f;
    if (i != unmanagedEvents.end()) checkErrors(i->second->getPitch(&pitch));
    return pitch;
}

void GodotFmod::setEventPitch(const String uuid, float pitch) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setPitch(pitch));
}

float GodotFmod::getEventVolume(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return 0.0f;
    auto i = unmanagedEvents.find(uuid);
    float volume = 0.0f;
    if (i != unmanagedEvents.end()) checkErrors(i->second->getVolume(&volume));
    return volume;
}

void GodotFmod::setEventVolume(const String uuid, float volume) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setVolume(volume));
}

int GodotFmod::getEventTimelinePosition(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return 0;
    auto i = unmanagedEvents.find(uuid);
    int tp = 0;
    if (i != unmanagedEvents.end()) checkErrors(i->second->getTimelinePosition(&tp));
    return tp;
}

void GodotFmod::setEventTimelinePosition(const String uuid, int position) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setTimelinePosition(position));
}

float GodotFmod::getEventReverbLevel(const String uuid, int index) {
    if (!unmanagedEvents.count(uuid)) return 0.0f;
    auto i = unmanagedEvents.find(uuid);
    float rvl = 0.0f;
    if (i != unmanagedEvents.end()) checkErrors(i->second->getReverbLevel(index, &rvl));
    return rvl;
}

void GodotFmod::setEventReverbLevel(const String uuid, int index, float level) {
    if (!unmanagedEvents.count(uuid)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) checkErrors(i->second->setReverbLevel(index, level));
}

bool GodotFmod::isEventVirtual(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return false;
    auto i = unmanagedEvents.find(uuid);
    bool v = false;
    if (i != unmanagedEvents.end()) checkErrors(i->second->isVirtual(&v));
    return v;
}

bool GodotFmod::getBusMute(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return false;
    bool mute = false;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getMute(&mute));
    return mute;
}

bool GodotFmod::getBusPaused(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return false;
    bool paused = false;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getPaused(&paused));
    return paused;
}

float GodotFmod::getBusVolume(const String busPath) {
    loadBus(busPath);
    if (!buses.count(busPath)) return 0.0f;
    float volume = 0.0f;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->getVolume(&volume));
    return volume;
}

void GodotFmod::setBusMute(const String busPath, bool mute) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setMute(mute));
}

void GodotFmod::setBusPaused(const String busPath, bool paused) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setPaused(paused));
}

void GodotFmod::setBusVolume(const String busPath, float volume) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    auto bus = buses.find(busPath);
    checkErrors(bus->second->setVolume(volume));
}

void GodotFmod::stopAllBusEvents(const String busPath, int stopMode) {
    loadBus(busPath);
    if (!buses.count(busPath)) return;
    checkErrors(buses.find(busPath)->second->stopAllEvents(static_cast<FMOD_STUDIO_STOP_MODE>(stopMode)));
}

void GodotFmod::loadBus(const String &busPath) {
    if (!buses.count(busPath)) {
        FMOD::Studio::Bus *b = nullptr;
        checkErrors(system->getBus(busPath.ascii().get_data(), &b));
        if (b) buses[busPath] = b;
    }
}

void GodotFmod::loadVCA(const String &VCAPath) {
    if (!VCAs.count(VCAPath)) {
        FMOD::Studio::VCA *vca = nullptr;
        checkErrors(system->getVCA(VCAPath.ascii().get_data(), &vca));
        if (vca) VCAs[VCAPath] = vca;
    }
}

void GodotFmod::playOneShot(const String eventName, Object *gameObj) {
    if (!eventDescriptions.count(eventName)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventName.alloc_c_string(), &desc));
        eventDescriptions[eventName] = desc;
    }
    auto desc = eventDescriptions.find(eventName);
    FMOD::Studio::EventInstance *instance;
    checkErrors(desc->second->createInstance(&instance));
    if (instance) {
        // set 3D attributes once
        if (!isNull(gameObj)) {
            updateInstance3DAttributes(instance, gameObj);
        }
        checkErrors(instance->start());
        oneShotInstances.push_back(instance);
    }
}

void GodotFmod::playOneShotWithParams(const String eventName, Object *gameObj, const Dictionary parameters) {
    if (!eventDescriptions.count(eventName)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
        eventDescriptions[eventName] = desc;
    }
    auto desc = eventDescriptions.find(eventName);
    FMOD::Studio::EventInstance *instance;
    checkErrors(desc->second->createInstance(&instance));
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
            checkErrors(instance->setParameterValue(k.ascii().get_data(), v));
        }
        checkErrors(instance->start());
        oneShotInstances.push_back(instance);
    }
}

void GodotFmod::playOneShotAttached(const String eventName, Object *gameObj) {
    if (!eventDescriptions.count(eventName)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
        eventDescriptions[eventName] = desc;
    }
    auto desc = eventDescriptions.find(eventName);
    FMOD::Studio::EventInstance *instance;
    checkErrors(desc->second->createInstance(&instance));
    if (instance && !isNull(gameObj)) {
        AttachedOneShot aShot = { instance, gameObj };
        attachedOneShots.push_back(aShot);
        checkErrors(instance->start());
    }
}

void GodotFmod::playOneShotAttachedWithParams(const String eventName, Object *gameObj, const Dictionary parameters) {
    if (!eventDescriptions.count(eventName)) {
        FMOD::Studio::EventDescription *desc = nullptr;
        checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
        eventDescriptions[eventName] = desc;
    }
    auto desc = eventDescriptions.find(eventName);
    FMOD::Studio::EventInstance *instance;
    checkErrors(desc->second->createInstance(&instance));
    if (instance && !isNull(gameObj)) {
        AttachedOneShot aShot = { instance, gameObj };
        attachedOneShots.push_back(aShot);
        // set the initial parameter values
        auto keys = parameters.keys();
        for (int i = 0; i < keys.size(); i++) {
            String k = keys[i];
            float v = parameters[keys[i]];
            checkErrors(instance->setParameterValue(k.ascii().get_data(), v));
        }
        checkErrors(instance->start());
    }
}

void GodotFmod::attachInstanceToNode(const String uuid, Object *gameObj) {
    if (!unmanagedEvents.count(uuid) || isNull(gameObj)) return;
    auto i = unmanagedEvents.find(uuid);
    if (i != unmanagedEvents.end()) {
        AttachedOneShot aShot = { i->second, gameObj };
        attachedOneShots.push_back(aShot);
    }
}

void GodotFmod::detachInstanceFromNode(const String uuid) {
    if (!unmanagedEvents.count(uuid)) return;
    auto instance = unmanagedEvents.find(uuid);
    if (instance != unmanagedEvents.end()) {
        for (int i = 0; attachedOneShots.size(); i++) {
            auto attachedInstance = attachedOneShots[i].instance;
            if (attachedInstance == instance->second) {
                attachedOneShots.erase(attachedOneShots.begin() + i);
                break;
            }
        }
    }
}

float GodotFmod::getVCAVolume(const String VCAPath) {
    loadVCA(VCAPath);
    if (!VCAs.count(VCAPath)) return 0.0f;
    auto vca = VCAs.find(VCAPath);
    float volume = 0.0f;
    checkErrors(vca->second->getVolume(&volume));
    return volume;
}

void GodotFmod::setVCAVolume(const String VCAPath, float volume) {
    loadVCA(VCAPath);
    if (!VCAs.count(VCAPath)) return;
    auto vca = VCAs.find(VCAPath);
    checkErrors(vca->second->setVolume(volume));
}

void GodotFmod::playSound(const String uuid) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPaused(false));
    }
}

void GodotFmod::setSoundPaused(const String uuid, bool paused) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPaused(paused));
    }
}

void GodotFmod::stopSound(const String uuid) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->stop());
    }
}

bool GodotFmod::isSoundPlaying(const String uuid) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        bool isPlaying = false;
        checkErrors(c->isPlaying(&isPlaying));
        return isPlaying;
    }
    return false;
}

void GodotFmod::setSoundVolume(const String uuid, float volume) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setVolume(volume));
    }
}

float GodotFmod::getSoundVolume(const String uuid) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        float volume = 0.f;
        checkErrors(c->getVolume(&volume));
        return volume;
    }
    return 0.f;
}

float GodotFmod::getSoundPitch(const String uuid) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        float pitch = 0.f;
        checkErrors(c->getPitch(&pitch));
        return pitch;
    }
    return 0.f;
}

void GodotFmod::setSoundPitch(const String uuid, float pitch) {
    if (sounds.count(uuid)) {
        auto s = sounds.find(uuid)->second;
        auto c = channels.find(s)->second;
        checkErrors(c->setPitch(pitch));
    }
}

String GodotFmod::loadSound(const String uuid, const String path, const int mode) {
    if (!sounds.count(path)) {
        FMOD::Sound *sound = nullptr;
        checkErrors(lowLevelSystem->createSound(path.alloc_c_string(), static_cast<FMOD_MODE>(mode), nullptr, &sound));
        if (sound) {
            sounds[uuid] = sound;
            FMOD::Channel *channel = nullptr;
            checkErrors(lowLevelSystem->playSound(sound, nullptr, true, &channel));
            if (channel) channels[sound] = channel;
        }
    }
    return uuid;
}

void GodotFmod::releaseSound(const String path) {
    if (!sounds.count(path)) return; // sound is not loaded
    auto sound = sounds.find(path);
    if (sound->second) checkErrors(sound->second->release());
}

void GodotFmod::setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale) {
    if (distanceFactor > 0 && checkErrors(lowLevelSystem->set3DSettings(dopplerScale, distanceFactor, rollOffScale))) {
        distanceScale = distanceFactor;
        Godot::print("Successfully set global 3D settings");
    } else {
        Godot::print_error("FMOD Sound System: Failed to set 3D settings :|", "GodotFmod::setSound3DSettings", __FILE__, __LINE__);
    }
}

void GodotFmod::_init() {
    system = nullptr;
    lowLevelSystem = nullptr;
    listener = nullptr;
    checkErrors(FMOD::Studio::System::create(&system));
    checkErrors(system->getLowLevelSystem(&lowLevelSystem));
    distanceScale = 1.0;
}