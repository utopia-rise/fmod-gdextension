#ifndef GODOTFMOD_FMOD_BANK_H
#define GODOTFMOD_FMOD_BANK_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodBank : public RefCounted
    {
        GDCLASS(FmodBank, RefCounted);

        FMOD::Studio::Bank* bank;

    public:
        FmodBank() = default;
        ~FmodBank() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_BANK_H
