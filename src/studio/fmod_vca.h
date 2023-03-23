#ifndef GODOTFMOD_FMOD_VCA_H
#define GODOTFMOD_FMOD_VCA_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "fmod_object.h"

namespace godot {
    class FmodVCA : public FmodObjectWithPath<FmodVCA, FMOD::Studio::VCA> {
    public:
        FmodVCA() = default;
        ~FmodVCA() override = default;

        float get_vca_volume(const String& VCAPath);
        void set_vca_volume(const String& VCAPath, float volume);

        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_VCA_H
