#ifndef GODOTFMOD_FMODBANK_H
#define GODOTFMOD_FMODBANK_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodBank : public RefCounted {
        GDCLASS(FmodBank, RefCounted);

        FMOD::Studio::Bank* bank;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMODBANK_H
