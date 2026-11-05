#ifndef GODOTFMOD_FMOD_FILE_H
#define GODOTFMOD_FMOD_FILE_H

#include "classes/ref.hpp"
#include "classes/ref_counted.hpp"
#include "core/binder_common.hpp"
#include "fmod.hpp"

namespace godot {
    class FmodSound;

    // Owns its FMOD sound, released with the last reference.
    class FmodFile : public RefCounted {
        GDCLASS(FmodFile, RefCounted);

        FMOD::Sound* _wrapped = nullptr;

    public:
        // Not registered with Godot: no method or property takes it yet.
        enum Mode : uint32_t {
            MODE_2D = FMOD_2D,
            MODE_3D = FMOD_3D,
            MODE_3D_CUSTOMROLLOFF = FMOD_3D_CUSTOMROLLOFF,
            MODE_3D_HEADRELATIVE = FMOD_3D_HEADRELATIVE,
            MODE_3D_IGNOREGEOMETRY = FMOD_3D_IGNOREGEOMETRY,
            MODE_3D_INVERSEROLLOFF = FMOD_3D_INVERSEROLLOFF,
            MODE_3D_INVERSETAPEREDROLLOFF = FMOD_3D_INVERSETAPEREDROLLOFF,
            MODE_3D_LINEARROLLOFF = FMOD_3D_LINEARROLLOFF,
            MODE_3D_LINEARSQUAREROLLOFF = FMOD_3D_LINEARSQUAREROLLOFF,
            MODE_3D_WORLDRELATIVE = FMOD_3D_WORLDRELATIVE,
            ACCURATETIME = FMOD_ACCURATETIME,
            CREATECOMPRESSEDSAMPLE = FMOD_CREATECOMPRESSEDSAMPLE,
            CREATESAMPLE = FMOD_CREATESAMPLE,
            CREATESTREAM = FMOD_CREATESTREAM,
            DEFAULT = FMOD_DEFAULT,
            IGNORETAGS = FMOD_IGNORETAGS,
            LOOP_BIDI = FMOD_LOOP_BIDI,
            LOOP_NORMAL = FMOD_LOOP_NORMAL,
            LOOP_OFF = FMOD_LOOP_OFF,
            LOWMEM = FMOD_LOWMEM,
            MPEGSEARCH = FMOD_MPEGSEARCH,
            NONBLOCKING = FMOD_NONBLOCKING,
            OPENMEMORY = FMOD_OPENMEMORY,
            OPENMEMORY_POINT = FMOD_OPENMEMORY_POINT,
            OPENONLY = FMOD_OPENONLY,
            OPENRAW = FMOD_OPENRAW,
            OPENUSER = FMOD_OPENUSER,
            UNIQUE = FMOD_UNIQUE,
            VIRTUAL_PLAYFROMSTART = FMOD_VIRTUAL_PLAYFROMSTART,
        };

        static Ref<FmodFile> create_ref(FMOD::Sound* wrapped);
        ~FmodFile() override;

        Ref<FmodSound> create_instance();

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_FILE_H
