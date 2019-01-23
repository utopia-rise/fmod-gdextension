/*************************************************************************/
/*  godot_fmod.h                                                         */
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

#ifndef GODOT_FMOD_H
#define GODOT_FMOD_H

#include "core/object.h"
#include "core/reference.h"
#include "scene/2d/canvas_item.h"
#include "core/vector.h"
#include "core/map.h"

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"

#include <iostream>

class Fmod : public Object {
	GDCLASS(Fmod, Object);

	FMOD::Studio::System *system;

	Object *listener; // TODO: Multiple listener support
	
	// currently loaded banks
	Map<String, FMOD::Studio::Bank *> banks;

	// list of one shot instances 
	Vector<FMOD::Studio::EventInstance *> oneShotInstances;

	//Map<String, FMOD::Studio::EventInstance *> unmanagedEvents;

	FMOD_3D_ATTRIBUTES get3DAttributes(FMOD_VECTOR pos, FMOD_VECTOR up, FMOD_VECTOR forward, FMOD_VECTOR vel);
	FMOD_VECTOR toFmodVector(Vector3 vec);
	void setListenerAttributes();
	int checkErrors(FMOD_RESULT result);

protected:
	static void _bind_methods();

public:
	void init(int numOfChannels, int studioFlags, int flags);
	void update();
	void shutdown();
	void addListener(Object *gameObj);

	/* helper functions for common FMOD Studio actions */
	void playOneShot(String eventName, Object *gameObj);

	String loadbank(const String &pathToBank, int flags);
	void unloadBank(const String &pathToBank);
	int getBankLoadingState(const String &pathToBank);

	/*void getEvent();
	void getEventParameter();
	void setEventParameter();
	void releaseEvent();*/

	

	Fmod();
	~Fmod();
};

#endif
