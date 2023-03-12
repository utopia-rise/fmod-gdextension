#ifndef GODOTFMOD_FMODVCA_H
#define GODOTFMOD_FMODVCA_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodVCA : public RefCounted {
        GDCLASS(FmodVCA, RefCounted);

        FMOD::Studio::VCA* vca;

        float get_vca_volume(const String& VCAPath);
        void set_vca_volume(const String& VCAPath, float volume);

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMODVCA_H
