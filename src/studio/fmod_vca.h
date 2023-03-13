#ifndef GODOTFMOD_FMOD_VCA_H
#define GODOTFMOD_FMOD_VCA_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodVCA : public RefCounted {
        GDCLASS(FmodVCA, RefCounted);

        FMOD::Studio::VCA* vca = nullptr;

        float get_vca_volume(const String& VCAPath);
        void set_vca_volume(const String& VCAPath, float volume);

    public:
        FmodVCA() = default;
        ~FmodVCA() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_VCA_H
