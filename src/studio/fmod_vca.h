#ifndef GODOTFMOD_FMOD_VCA_H
#define GODOTFMOD_FMOD_VCA_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"

namespace godot {
    class FmodVCA  : public RefCounted {
        FMODCLASSWITHPATH(FmodVCA, RefCounted, FMOD::Studio::VCA);

    public:
        FmodVCA() = default;
        ~FmodVCA() override = default;

        float get_volume();
        void set_volume(float volume);

        protected:
            static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_VCA_H
