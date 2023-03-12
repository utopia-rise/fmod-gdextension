#ifndef GODOTFMOD_FMOD_OBJECT_H
#define GODOTFMOD_FMOD_OBJECT_H

#include "fmod_studio.hpp"
namespace godot {
    class FmodObject {
    protected:
        FMOD::Studio::System* system;
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_OBJECT_H
