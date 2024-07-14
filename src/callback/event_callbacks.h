#ifndef GODOTFMOD_GODOT_FMOD_CALLBACK_H
#define GODOTFMOD_GODOT_FMOD_CALLBACK_H

#include <fmod_common.h>
#include <fmod_studio_common.h>

namespace Callbacks {
    FMOD_RESULT F_CALLBACK event_callback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters);
}// namespace Callbacks

#endif// GODOTFMOD_GODOT_FMOD_CALLBACK_H
