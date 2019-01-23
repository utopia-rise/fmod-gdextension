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
	checkErrors(FMOD::Studio::System::create(&system));
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
		if (!aShot.gameObj) {
			// null object
			checkErrors(aShot.instance->release());
			attachedOneShots.remove(i);
			i--;
			continue;
		}
		FMOD_STUDIO_PLAYBACK_STATE s;
		checkErrors(aShot.instance->getPlaybackState(&s));
		if (s == FMOD_STUDIO_PLAYBACK_STOPPED) {
			// one shot has finished playing
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
	if (instance) {
		// try to set 3D attributes
		if (o) {
			CanvasItem *ci = Object::cast_to<CanvasItem>(o);
			if (ci) {
				Transform2D t2d = ci->get_transform();
				Vector3 pos(t2d.get_origin().x, t2d.get_origin().y, 0.0f),
						up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0);
				FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
				checkErrors(instance->set3DAttributes(&attr));
			}
		}
		// TODO: Add 3D node support
	}
}

void Fmod::shutdown() {
	checkErrors(system->unloadAll());
	checkErrors(system->release());
}

void Fmod::setListenerAttributes() {
	if (!listener) {
		fprintf(stderr, "FMOD Sound System: Listener not set!\n");
		return;
	}
	CanvasItem *ci = Object::cast_to<CanvasItem>(listener);
	if (ci) {
		Transform2D t2d = ci->get_transform();
		Vector3 pos(t2d.get_origin().x, t2d.get_origin().y, 0.0f),
		up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0); // TODO: add doppler 
		FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
		checkErrors(system->setListenerAttributes(0, &attr));
		return;
	}
	// TODO: add support for 3D Nodes 
}

void Fmod::addListener(Object *gameObj) {
	listener = gameObj;
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
	if (bank) checkErrors(bank->value()->unload());
}

int Fmod::getBankLoadingState(const String &pathToBank) {
	if (!banks.has(pathToBank)) return -1; // bank is not loaded
	auto bank = banks.find(pathToBank);
	if (bank) {
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
		checkErrors(bank->value()->getBusCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankEventCount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		checkErrors(bank->value()->getEventCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankStringCount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		checkErrors(bank->value()->getStringCount(&count));
		return count;
	}
	return -1;
}

int Fmod::getBankVCACount(const String &pathToBank) {
	if (banks.has(pathToBank)) {
		int count;
		auto bank = banks.find(pathToBank);
		checkErrors(bank->value()->getVCACount(&count));
		return count;
	}
	return -1;
}

void Fmod::createEventInstance(const String &uuid, const String &eventPath) {
	
}

int Fmod::checkErrors(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		fprintf(stderr, "FMOD Sound System: %s\n", FMOD_ErrorString(result));
		return 0;
	}
	return 1;
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
		// try to set 3D attributes
		if (gameObj) {
			CanvasItem *ci = Object::cast_to<CanvasItem>(gameObj);
			if (ci) {
				Transform2D t2d = ci->get_transform();
				Vector3 pos(t2d.get_origin().x, t2d.get_origin().y, 0.0f),
						up(0, 1, 0), forward(0, 0, 1), vel(0, 0, 0);
				FMOD_3D_ATTRIBUTES attr = get3DAttributes(toFmodVector(pos), toFmodVector(up), toFmodVector(forward), toFmodVector(vel));
				checkErrors(instance->set3DAttributes(&attr));
			}
		}
		checkErrors(instance->start());
		oneShotInstances.push_back(instance);
		// TODO: Add 3D node support
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
	if (instance && gameObj) {
		AttachedOneShot aShot = { instance, gameObj };
		attachedOneShots.push_back(aShot);
		checkErrors(instance->start());
	}
}

void Fmod::_bind_methods() {
	/* system functions */
	ClassDB::bind_method(D_METHOD("system_init", "num_of_channels", "studio_flags", "flags"), &Fmod::init);
	ClassDB::bind_method(D_METHOD("system_update"), &Fmod::update);
	ClassDB::bind_method(D_METHOD("system_shutdown"), &Fmod::shutdown);	
	ClassDB::bind_method(D_METHOD("system_add_listener", "node"), &Fmod::addListener);

	/* integration helper functions */
	ClassDB::bind_method(D_METHOD("play_one_shot", "event_name", "node"), &Fmod::playOneShot);
	ClassDB::bind_method(D_METHOD("play_one_shot_attached", "event_name", "node"), &Fmod::playOneShotAttached);

	/* bank functions */
	ClassDB::bind_method(D_METHOD("bank_load", "path_to_bank", "flags"), &Fmod::loadbank);
	ClassDB::bind_method(D_METHOD("bank_unload", "path_to_bank"), &Fmod::unloadBank);
	ClassDB::bind_method(D_METHOD("bank_get_loading_state", "path_to_bank"), &Fmod::getBankLoadingState);
	ClassDB::bind_method(D_METHOD("bank_get_bus_count", "path_to_bank"), &Fmod::getBankBusCount);
	ClassDB::bind_method(D_METHOD("bank_get_event_count", "path_to_bank"), &Fmod::getBankEventCount);
	ClassDB::bind_method(D_METHOD("bank_get_string_count", "path_to_bank"), &Fmod::getBankStringCount);
	ClassDB::bind_method(D_METHOD("bank_get_vca_count", "path_to_bank"), &Fmod::getBankVCACount);




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

}

Fmod::Fmod() {
	system, listener = nullptr;
}

Fmod::~Fmod() {
	Fmod::shutdown();
}
