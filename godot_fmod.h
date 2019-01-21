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

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"

class Fmod : public Object {
	GDCLASS(Fmod, Object);

	FMOD::Studio::System *system;	

	Map<String, FMOD::Studio::Bank *> banks;
	Map<String, FMOD::Studio::EventInstance *> events;

	int checkErrors(FMOD_RESULT result);

protected:
	static void _bind_methods();

public:
	void initialize(int numOfChannels, int studioFlags, int flags);
	void update();
	void shutdown();

	Fmod();
	~Fmod();
};

#endif
