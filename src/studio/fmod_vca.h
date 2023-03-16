#ifndef GODOTFMOD_FMOD_VCA_H
#define GODOTFMOD_FMOD_VCA_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "helpers/fmod_object.h"

namespace godot {
    class FmodVCA : public FmodObject<FmodVCA, FMOD::Studio::VCA> {
        float get_vca_volume(const String& VCAPath);
        void set_vca_volume(const String& VCAPath, float volume);

    public:
        FmodVCA() = default;
        ~FmodVCA() = default;
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_VCA_H
