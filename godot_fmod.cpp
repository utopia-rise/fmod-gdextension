/*************************************************************************/
/*  godot_fmod.cpp                                                       */
/*************************************************************************/
/*                                                                       */
/*       FMOD Studio module and bindings for the Godot game engine       */
/*                                                                       */
/*************************************************************************/
/* Copyright (c) 2019 Alex Fonseka                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "godot_fmod.h"

void Fmod::init(int numOfChannels, int studioFlags, int flags) {
	// initialize FMOD Studio and FMOD Low Level System with provided flags
	if (checkErrors(system->initialize(numOfChannels, studioFlags, flags, nullptr))) {
		printf("FMOD Sound System successfully initialized with %d channels\n", numOfChannels);
		if (studioFlags == FMOD_STUDIO_INIT_LIVEUPDATE)
			printf("Live update enabled!\n");
	} else
		fprintf(stderr, "FMOD Sound System failed to initialize :|\n");
}

void Fmod::update() {
	// clean up one shots
	for (int i = 0; i < oneShotInstances.size(); i++) {
		auto instance = oneShotInstances.get(i);
		FMOD_STUDIO_PLAYBACK_STATE s;
		checkErrors(instance->getPlaybackState(&s));
		if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
			checkErrors(instance->release());
			oneShotInstances.remove(i);
			i--;
		}
	}

	// update and clean up attached one shots
	for (int i = 0; i < attachedOneShots.size(); i++) {
		auto aShot = attachedOneShots.get(i);
		if (isNull(aShot.gameObj)) {
			FMOD_STUDIO_STOP_MODE m = FMOD_STUDIO_STOP_IMMEDIATE;
			checkErrors(aShot.instance->stop(m));
			checkErrors(aShot.instance->release());
			attachedOneShots.remove(i);
			i--;
			continue;
		}
		FMOD_STUDIO_PLAYBACK_STATE s;
		checkErrors(aShot.instance->getPlaybackState(&s));
		if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
			checkErrors(aShot.instance->release());
			attachedOneShots.remove(i);
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

void Fmod::updateInstance3DAttributes(FMOD::Studio::EventInstance *instance, Object *o) {
	// try to set 3D attributes
	if (instance && !isNull(o)) {
		CanvasItem *ci = Object::cast_to<CanvasItem>(o);
		if (ci != NULL) {
			Transform2D t2d = ci->get_transform();
			Vector3 pos(t2d.get_origin().x, t2d.get_origin().y, 0.0f),
					up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0);
			FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
			checkErrors(instance->set3DAttributes(&attr));
		} else {
			// needs testing
			Spatial *s = Object::cast_to<Spatial>(o);
			Transform t = s->get_transform();
			Vector3 pos = t.get_origin();
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

void Fmod::setListenerAttributes() {
	if (isNull(listener)) {
		fprintf(stderr, "FMOD Sound System: Listener not set!\n");
		return;
	}
	CanvasItem *ci = Object::cast_to<CanvasItem>(listener);
	if (ci != NULL) {
		Transform2D t2d = ci->get_transform();
		Vector3 pos(t2d.get_origin().x, t2d.get_origin().y, 0.0f),
				up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0); // TODO: add doppler
		FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
		checkErrors(system->setListenerAttributes(0, &attr));

	} else {
		// needs testing
		Spatial *s = Object::cast_to<Spatial>(listener);
		Transform t = s->get_transform();
		Vector3 pos = t.get_origin();
		Vector3 up = t.get_basis().elements[1];
		Vector3 forward = t.get_basis().elements[2];
		Vector3 vel(0, 0, 0);
		FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
		checkErrors(system->setListenerAttributes(0, &attr));
	}
}

void Fmod::addListener(Object *gameObj) {
	listener = gameObj;
}

void Fmod::setSoftwareFormat(int sampleRate, int speakerMode, int numRawSpeakers) {
	auto m = static_cast<FMOD_SPEAKERMODE>(speakerMode);
	checkErrors(lowLevelSystem->setSoftwareFormat(sampleRate, m, numRawSpeakers));
}

String Fmod::loadbank(const String &pathToBank, int flags) {
	if (banks.has(pathToBank)) return pathToBank; // bank is already loaded
	FMOD::Studio::Bank *bank = nullptr;
	checkErrors(system->loadBankFile(pathToBank.ascii().get_data(), flags, &bank));
	if (bank) {
		banks.insert(pathToBank, bank);
		return pathToBank;
	}
	return pathToBank;
}

void Fmod::unloadBank(const String &pathToBank) {
	if (!banks.has(pathToBank)) return; // bank is not loaded
	auto bank = banks.find(pathToBank);
	if (bank->value()) checkErrors(bank->value()->unload());
}

int Fmod::getBankLoadingState(const String &pathToBank) {
	if (!banks.has(pathToBank)) return -1; // bank is not loaded
	auto bank = banks.find(pathToBank);
	if (bank->value()) {
		FMOD_STUDIO_LOADING_STATE state;
		checkErrors(bank->value()->getLoadingState(&state));
		return state;
	}
	return -1;
}

int Fmod::getBankBusCount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		if (bank->value()) checkErrors(bank->value()->getBusCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankEventCount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		if (bank->value()) checkErrors(bank->value()->getEventCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankStringCount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		if (bank->value()) checkErrors(bank->value()->getStringCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankVCACount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		if (bank->value()) checkErrors(bank->value()->getVCACount(&count));
		return count;
	}
	return -1;
}

void Fmod::createEventInstance(const String &uuid, const String &eventPath) {
	if (unmanagedEvents.has(uuid)) return; // provided uuid is not valid
	if (!eventDescriptions.has(eventPath)) {
		FMOD::Studio::EventDescription *desc = nullptr;
		checkErrors(system->getEvent(eventPath.ascii().get_data(), &desc));
		eventDescriptions.insert(eventPath, desc);
	}
	auto desc = eventDescriptions.find(eventPath);
	FMOD::Studio::EventInstance *instance;
	checkErrors(desc->value()->createInstance(&instance));
	if (instance)
		unmanagedEvents.insert(uuid, instance);
}

float Fmod::getEventParameter(const String &uuid, const String &parameterName) {
	float p = -1;
	if (!unmanagedEvents.has(uuid)) return p;
	auto i = unmanagedEvents.find(uuid);
	if (i->value())
		checkErrors(i->value()->getParameterValue(parameterName.ascii().get_data(), &p));
	return p;
}

void Fmod::setEventParameter(const String &uuid, const String &parameterName, float value) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setParameterValue(parameterName.ascii().get_data(), value));
}

void Fmod::releaseEvent(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->release());
}

void Fmod::startEvent(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->start());
}

void Fmod::stopEvent(const String &uuid, int stopMode) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) {
		auto m = static_cast<FMOD_STUDIO_STOP_MODE>(stopMode);
		checkErrors(i->value()->stop(m));
	}
}

void Fmod::triggerEventCue(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->triggerCue());
}

int Fmod::getEventPlaybackState(const String &uuid) {
	if (!unmanagedEvents.has(uuid))
		return -1;
	else {
		auto i = unmanagedEvents.find(uuid);
		if (i->value()) {
			FMOD_STUDIO_PLAYBACK_STATE s;
			checkErrors(i->value()->getPlaybackState(&s));
			return s;
		}
		return -1;
	}
}

bool Fmod::getEventPaused(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return false;
	auto i = unmanagedEvents.find(uuid);
	bool paused = false;
	if (i->value()) checkErrors(i->value()->getPaused(&paused));
	return paused;
}

void Fmod::setEventPaused(const String &uuid, bool paused) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setPaused(paused));
}

float Fmod::getEventPitch(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return 0.0f;
	auto i = unmanagedEvents.find(uuid);
	float pitch = 0.0f;
	if (i->value()) checkErrors(i->value()->getPitch(&pitch));
	return pitch;
}

void Fmod::setEventPitch(const String &uuid, float pitch) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setPitch(pitch));
}

float Fmod::getEventVolume(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return 0.0f;
	auto i = unmanagedEvents.find(uuid);
	float volume = 0.0f;
	if (i->value()) checkErrors(i->value()->getVolume(&volume));
	return volume;
}

void Fmod::setEventVolume(const String &uuid, float volume) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setVolume(volume));
}

int Fmod::getEventTimelinePosition(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return 0;
	auto i = unmanagedEvents.find(uuid);
	int tp = 0;
	if (i->value()) checkErrors(i->value()->getTimelinePosition(&tp));
	return tp;
}

void Fmod::setEventTimelinePosition(const String &uuid, int position) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setTimelinePosition(position));
}

float Fmod::getEventReverbLevel(const String &uuid, int index) {
	if (!unmanagedEvents.has(uuid)) return 0.0f;
	auto i = unmanagedEvents.find(uuid);
	float rvl = 0.0f;
	if (i->value()) checkErrors(i->value()->getReverbLevel(index, &rvl));
	return rvl;
}

void Fmod::setEventReverbLevel(const String &uuid, int index, float level) {
	if (!unmanagedEvents.has(uuid)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) checkErrors(i->value()->setReverbLevel(index, level));
}

bool Fmod::isEventVirtual(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return false;
	auto i = unmanagedEvents.find(uuid);
	bool v = false;
	if (i->value()) checkErrors(i->value()->isVirtual(&v));
	return v;
}

bool Fmod::getBusMute(const String &busPath) {
	loadBus(busPath);
	if (!buses.has(busPath)) return false;
	bool mute = false;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->getMute(&mute));
	return mute;
}

bool Fmod::getBusPaused(const String &busPath) {
	loadBus(busPath);
	if (!buses.has(busPath)) return false;
	bool paused = false;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->getPaused(&paused));
	return paused;
}

float Fmod::getBusVolume(const String &busPath) {
	loadBus(busPath);
	if (!buses.has(busPath)) return 0.0f;
	float volume = 0.0f;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->getVolume(&volume));
	return volume;
}

void Fmod::setBusMute(const String &busPath, bool mute) {
	loadBus(busPath);
	if (!buses.has(busPath)) return;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->setMute(mute));
}

void Fmod::setBusPaused(const String &busPath, bool paused) {
	loadBus(busPath);
	if (!buses.has(busPath)) return;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->setPaused(paused));
}

void Fmod::setBusVolume(const String &busPath, float volume) {
	loadBus(busPath);
	if (!buses.has(busPath)) return;
	auto bus = buses.find(busPath);
	checkErrors(bus->value()->setVolume(volume));
}

void Fmod::stopAllBusEvents(const String &busPath, int stopMode) {
	loadBus(busPath);
	if (!buses.has(busPath)) return;
	auto bus = buses.find(busPath);
	auto m = static_cast<FMOD_STUDIO_STOP_MODE>(stopMode);
	checkErrors(bus->value()->stopAllEvents(m));
}

int Fmod::checkErrors(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		fprintf(stderr, "FMOD Sound System: %s\n", FMOD_ErrorString(result));
		return 0;
	}
	return 1;
}

bool Fmod::isNull(Object *o) {
	CanvasItem *ci = Object::cast_to<CanvasItem>(o);
	Spatial *s = Object::cast_to<Spatial>(o);
	if (ci == NULL && s == NULL)
		// an object cannot be 2D and 3D at the same time
		// which means if the first cast returned null then the second cast also returned null
		return true;
	return false; // all g.
}

void Fmod::loadBus(const String &busPath) {
	if (!buses.has(busPath)) {
		FMOD::Studio::Bus *b = nullptr;
		checkErrors(system->getBus(busPath.ascii().get_data(), &b));
		if (b) buses.insert(busPath, b);
	}
}

void Fmod::loadVCA(const String &VCAPath) {
	if (!VCAs.has(VCAPath)) {
		FMOD::Studio::VCA *vca = nullptr;
		checkErrors(system->getVCA(VCAPath.ascii().get_data(), &vca));
		if (vca) VCAs.insert(VCAPath, vca);
	}
}

FMOD_VECTOR Fmod::toFmodVector(Vector3 vec) {
	FMOD_VECTOR fv;
	fv.x = vec.x;
	fv.y = vec.y;
	fv.z = vec.z;
	return fv;
}

FMOD_3D_ATTRIBUTES Fmod::get3DAttributes(FMOD_VECTOR pos, FMOD_VECTOR up, FMOD_VECTOR forward, FMOD_VECTOR vel) {
	FMOD_3D_ATTRIBUTES f3d;
	f3d.forward = forward;
	f3d.position = pos;
	f3d.up = up;
	f3d.velocity = vel;
	return f3d;
}

void Fmod::playOneShot(const String &eventName, Object *gameObj) {
	if (!eventDescriptions.has(eventName)) {
		FMOD::Studio::EventDescription *desc = nullptr;
		checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
		eventDescriptions.insert(eventName, desc);
	}
	auto desc = eventDescriptions.find(eventName);
	FMOD::Studio::EventInstance *instance;
	checkErrors(desc->value()->createInstance(&instance));
	if (instance) {
		// set 3D attributes once
		if (!isNull(gameObj)) {
			updateInstance3DAttributes(instance, gameObj);
		}
		checkErrors(instance->start());
		oneShotInstances.push_back(instance);
	}
}

void Fmod::playOneShotWithParams(const String &eventName, Object *gameObj, const Dictionary &parameters) {
	if (!eventDescriptions.has(eventName)) {
		FMOD::Studio::EventDescription *desc = nullptr;
		checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
		eventDescriptions.insert(eventName, desc);
	}
	auto desc = eventDescriptions.find(eventName);
	FMOD::Studio::EventInstance *instance;
	checkErrors(desc->value()->createInstance(&instance));
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

void Fmod::playOneShotAttached(const String &eventName, Object *gameObj) {
	if (!eventDescriptions.has(eventName)) {
		FMOD::Studio::EventDescription *desc = nullptr;
		checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
		eventDescriptions.insert(eventName, desc);
	}
	auto desc = eventDescriptions.find(eventName);
	FMOD::Studio::EventInstance *instance;
	checkErrors(desc->value()->createInstance(&instance));
	if (instance && !isNull(gameObj)) {
		AttachedOneShot aShot = { instance, gameObj };
		attachedOneShots.push_back(aShot);
		checkErrors(instance->start());
	}
}

void Fmod::playOneShotAttachedWithParams(const String &eventName, Object *gameObj, const Dictionary &parameters) {
	if (!eventDescriptions.has(eventName)) {
		FMOD::Studio::EventDescription *desc = nullptr;
		checkErrors(system->getEvent(eventName.ascii().get_data(), &desc));
		eventDescriptions.insert(eventName, desc);
	}
	auto desc = eventDescriptions.find(eventName);
	FMOD::Studio::EventInstance *instance;
	checkErrors(desc->value()->createInstance(&instance));
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

void Fmod::attachInstanceToNode(const String &uuid, Object *gameObj) {
	if (!unmanagedEvents.has(uuid) || isNull(gameObj)) return;
	auto i = unmanagedEvents.find(uuid);
	if (i->value()) {
		AttachedOneShot aShot = { i->value(), gameObj };
		attachedOneShots.push_back(aShot);
	}
}

void Fmod::detachInstanceFromNode(const String &uuid) {
	if (!unmanagedEvents.has(uuid)) return;
	auto instance = unmanagedEvents.find(uuid);
	if (instance->value()) {
		for (int i = 0; attachedOneShots.size(); i++) {
			auto attachedInstance = attachedOneShots.get(i).instance;
			if (attachedInstance == instance->value()) {
				attachedOneShots.remove(i);
				break;
			}
		}
	}
}

void Fmod::_bind_methods() {
	/* system functions */
	ClassDB::bind_method(D_METHOD("system_init", "num_of_channels", "studio_flags", "flags"), &Fmod::init);
	ClassDB::bind_method(D_METHOD("system_update"), &Fmod::update);
	ClassDB::bind_method(D_METHOD("system_shutdown"), &Fmod::shutdown);
	ClassDB::bind_method(D_METHOD("system_add_listener", "node"), &Fmod::addListener);
	ClassDB::bind_method(D_METHOD("system_set_software_format", "sample_rate", "speaker_mode", "num_raw_speakers"), &Fmod::setSoftwareFormat);

	/* integration helper functions */
	ClassDB::bind_method(D_METHOD("play_one_shot", "event_name", "node"), &Fmod::playOneShot);
	ClassDB::bind_method(D_METHOD("play_one_shot_with_params", "event_name", "node", "initial_parameters"), &Fmod::playOneShotWithParams);
	ClassDB::bind_method(D_METHOD("play_one_shot_attached", "event_name", "node"), &Fmod::playOneShotAttached);
	ClassDB::bind_method(D_METHOD("play_one_shot_attached_with_params", "event_name", "node", "initial_parameters"), &Fmod::playOneShotAttachedWithParams);
	ClassDB::bind_method(D_METHOD("attach_instance_to_node", "uuid", "node"), &Fmod::attachInstanceToNode);
	ClassDB::bind_method(D_METHOD("detach_instance_from_node", "uuid"), &Fmod::detachInstanceFromNode);

	/* bank functions */
	ClassDB::bind_method(D_METHOD("bank_load", "path_to_bank", "flags"), &Fmod::loadbank);
	ClassDB::bind_method(D_METHOD("bank_unload", "path_to_bank"), &Fmod::unloadBank);
	ClassDB::bind_method(D_METHOD("bank_get_loading_state", "path_to_bank"), &Fmod::getBankLoadingState);
	ClassDB::bind_method(D_METHOD("bank_get_bus_count", "path_to_bank"), &Fmod::getBankBusCount);
	ClassDB::bind_method(D_METHOD("bank_get_event_count", "path_to_bank"), &Fmod::getBankEventCount);
	ClassDB::bind_method(D_METHOD("bank_get_string_count", "path_to_bank"), &Fmod::getBankStringCount);
	ClassDB::bind_method(D_METHOD("bank_get_vca_count", "path_to_bank"), &Fmod::getBankVCACount);

	/* event functions */
	ClassDB::bind_method(D_METHOD("event_create_instance", "uuid", "event_path"), &Fmod::createEventInstance);
	ClassDB::bind_method(D_METHOD("event_get_parameter", "uuid", "parameter_name"), &Fmod::getEventParameter);
	ClassDB::bind_method(D_METHOD("event_set_parameter", "uuid", "parameter_name", "value"), &Fmod::setEventParameter);
	ClassDB::bind_method(D_METHOD("event_release", "uuid"), &Fmod::releaseEvent);
	ClassDB::bind_method(D_METHOD("event_start", "uuid"), &Fmod::startEvent);
	ClassDB::bind_method(D_METHOD("event_stop", "uuid", "stop_mode"), &Fmod::stopEvent);
	ClassDB::bind_method(D_METHOD("event_trigger_cue", "uuid"), &Fmod::triggerEventCue);
	ClassDB::bind_method(D_METHOD("event_get_playback_state", "uuid"), &Fmod::getEventPlaybackState);
	ClassDB::bind_method(D_METHOD("event_get_paused", "uuid"), &Fmod::getEventPaused);
	ClassDB::bind_method(D_METHOD("event_set_paused", "uuid", "paused"), &Fmod::setEventPaused);
	ClassDB::bind_method(D_METHOD("event_get_pitch", "uuid"), &Fmod::getEventPitch);
	ClassDB::bind_method(D_METHOD("event_set_pitch", "uuid", "pitch"), &Fmod::setEventPitch);
	ClassDB::bind_method(D_METHOD("event_get_volume", "uuid"), &Fmod::getEventVolume);
	ClassDB::bind_method(D_METHOD("event_set_volume", "uuid", "volume"), &Fmod::setEventVolume);
	ClassDB::bind_method(D_METHOD("event_get_timeline_position", "uuid"), &Fmod::getEventTimelinePosition);
	ClassDB::bind_method(D_METHOD("event_set_timeline_position", "uuid", "position"), &Fmod::setEventTimelinePosition);
	ClassDB::bind_method(D_METHOD("event_get_reverb_level", "uuid", "index"), &Fmod::getEventReverbLevel);
	ClassDB::bind_method(D_METHOD("event_set_reverb_level", "uuid", "index", "level"), &Fmod::setEventReverbLevel);
	ClassDB::bind_method(D_METHOD("event_is_virtual", "uuid"), &Fmod::isEventVirtual);

	/* bus functions */
	ClassDB::bind_method(D_METHOD("bus_get_mute", "path_to_bus"), &Fmod::getBusMute);
	ClassDB::bind_method(D_METHOD("bus_get_paused", "path_to_bus"), &Fmod::getBusPaused);
	ClassDB::bind_method(D_METHOD("bus_get_volume", "path_to_bus"), &Fmod::getBusVolume);
	ClassDB::bind_method(D_METHOD("bus_set_mute", "path_to_bus", "mute"), &Fmod::setBusMute);
	ClassDB::bind_method(D_METHOD("bus_set_paused", "path_to_bus", "paused"), &Fmod::setBusPaused);
	ClassDB::bind_method(D_METHOD("bus_set_volume", "path_to_bus", "volume"), &Fmod::setBusVolume);
	ClassDB::bind_method(D_METHOD("bus_stop_all_events", "path_to_bus", "stop_mode"), &Fmod::stopAllBusEvents);

	/* VCA functions */
	ClassDB::bind_method(D_METHOD("vca_get_volume", "path_to_vca"), &Fmod::getVCAVolume);
	ClassDB::bind_method(D_METHOD("vca_set_volume", "path_to_vca", "volume"), &Fmod::setVCAVolume);

	/* FMOD_INITFLAGS */
	BIND_CONSTANT(FMOD_INIT_NORMAL);
	BIND_CONSTANT(FMOD_INIT_STREAM_FROM_UPDATE);
	BIND_CONSTANT(FMOD_INIT_MIX_FROM_UPDATE);
	BIND_CONSTANT(FMOD_INIT_3D_RIGHTHANDED);
	BIND_CONSTANT(FMOD_INIT_CHANNEL_LOWPASS);
	BIND_CONSTANT(FMOD_INIT_CHANNEL_DISTANCEFILTER);
	BIND_CONSTANT(FMOD_INIT_PROFILE_ENABLE);
	BIND_CONSTANT(FMOD_INIT_VOL0_BECOMES_VIRTUAL);
	BIND_CONSTANT(FMOD_INIT_GEOMETRY_USECLOSEST);
	BIND_CONSTANT(FMOD_INIT_PREFER_DOLBY_DOWNMIX);
	BIND_CONSTANT(FMOD_INIT_THREAD_UNSAFE);
	BIND_CONSTANT(FMOD_INIT_PROFILE_METER_ALL);
	BIND_CONSTANT(FMOD_INIT_DISABLE_SRS_HIGHPASSFILTER);

	/* FMOD_STUDIO_INITFLAGS */
	BIND_CONSTANT(FMOD_STUDIO_INIT_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_INIT_LIVEUPDATE);
	BIND_CONSTANT(FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS);
	BIND_CONSTANT(FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE);
	BIND_CONSTANT(FMOD_STUDIO_INIT_DEFERRED_CALLBACKS);
	BIND_CONSTANT(FMOD_STUDIO_INIT_LOAD_FROM_UPDATE);

	/* FMOD_STUDIO_LOAD_BANK_FLAGS */
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NORMAL);
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NONBLOCKING);
	BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES);

	/* FMOD_STUDIO_LOADING_STATE */
	BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADING);
	BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADING);
	BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADED);
	BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_ERROR);

	/* FMOD_STUDIO_PLAYBACK_STATE */
	BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_PLAYING);
	BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_SUSTAINING);
	BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPED);
	BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STARTING);
	BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPING);

	/* FMOD_STUDIO_STOP_MODE */
	BIND_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	BIND_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE);

	/* FMOD_SPEAKERMODE */
	BIND_CONSTANT(FMOD_SPEAKERMODE_DEFAULT);
	BIND_CONSTANT(FMOD_SPEAKERMODE_RAW);
	BIND_CONSTANT(FMOD_SPEAKERMODE_MONO);
	BIND_CONSTANT(FMOD_SPEAKERMODE_STEREO);
	BIND_CONSTANT(FMOD_SPEAKERMODE_QUAD);
	BIND_CONSTANT(FMOD_SPEAKERMODE_SURROUND);
	BIND_CONSTANT(FMOD_SPEAKERMODE_5POINT1);
	BIND_CONSTANT(FMOD_SPEAKERMODE_7POINT1);
	BIND_CONSTANT(FMOD_SPEAKERMODE_7POINT1POINT4);
	BIND_CONSTANT(FMOD_SPEAKERMODE_MAX);
}

float Fmod::getVCAVolume(const String &VCAPath) {
	loadVCA(VCAPath);
	if (!VCAs.has(VCAPath)) return 0.0f;
	auto vca = VCAs.find(VCAPath);
	float volume = 0.0f;
	checkErrors(vca->value()->getVolume(&volume));
	return volume;
}

void Fmod::setVCAVolume(const String &VCAPath, float volume) {
	loadVCA(VCAPath);
	if (!VCAs.has(VCAPath)) return;
	auto vca = VCAs.find(VCAPath);
	checkErrors(vca->value()->setVolume(volume));
}

Fmod::Fmod() {
	system, lowLevelSystem, listener = nullptr;
	checkErrors(FMOD::Studio::System::create(&system));
	checkErrors(system->getLowLevelSystem(&lowLevelSystem));
}

Fmod::~Fmod() {
	Fmod::shutdown();
}
