//
// Created by CedNaru on 18/07/2019.
//

#ifndef GODOTFMOD_CONSTANTS_H
#define GODOTFMOD_CONSTANTS_H

#include <fmod_common.h>

#define DECLARE_CONSTANT(name) \
    int _##name = name;        \
    int get_##name() {         \
        return _##name;        \
    }                          \
    void set_##name(int i) {   \
        _##name = name;        \
        return;                \
    }

#define REGISTER_CONSTANT(name) \
    register_property<Fmod, int>(#name, &Fmod::set_##name, &Fmod::get_##name, name);

#define DECLARE_ALL_CONSTANTS                                  \
    DECLARE_CONSTANT(FMOD_INIT_3D_RIGHTHANDED)                 \
    DECLARE_CONSTANT(FMOD_INIT_CHANNEL_DISTANCEFILTER)         \
    DECLARE_CONSTANT(FMOD_INIT_CHANNEL_LOWPASS)                \
    DECLARE_CONSTANT(FMOD_INIT_GEOMETRY_USECLOSEST)            \
    DECLARE_CONSTANT(FMOD_INIT_MIX_FROM_UPDATE)                \
    DECLARE_CONSTANT(FMOD_INIT_NORMAL)                         \
    DECLARE_CONSTANT(FMOD_INIT_PREFER_DOLBY_DOWNMIX)           \
    DECLARE_CONSTANT(FMOD_INIT_PROFILE_ENABLE)                 \
    DECLARE_CONSTANT(FMOD_INIT_PROFILE_METER_ALL)              \
    DECLARE_CONSTANT(FMOD_INIT_STREAM_FROM_UPDATE)             \
    DECLARE_CONSTANT(FMOD_INIT_THREAD_UNSAFE)                  \
    DECLARE_CONSTANT(FMOD_INIT_VOL0_BECOMES_VIRTUAL)           \
                                                               \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_NORMAL)                  \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_LIVEUPDATE)              \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS)   \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE)      \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_DEFERRED_CALLBACKS)      \
    DECLARE_CONSTANT(FMOD_STUDIO_INIT_LOAD_FROM_UPDATE)        \
                                                               \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_5POINT1)                 \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_7POINT1)                 \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_7POINT1POINT4)           \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_DEFAULT)                 \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_MAX)                     \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_MONO)                    \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_QUAD)                    \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_RAW)                     \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_STEREO)                  \
    DECLARE_CONSTANT(FMOD_SPEAKERMODE_SURROUND)                \
                                                               \
    DECLARE_CONSTANT(FMOD_STUDIO_LOAD_BANK_NORMAL)             \
    DECLARE_CONSTANT(FMOD_STUDIO_LOAD_BANK_NONBLOCKING)        \
    DECLARE_CONSTANT(FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES) \
                                                               \
    DECLARE_CONSTANT(FMOD_2D)                                  \
    DECLARE_CONSTANT(FMOD_3D)                                  \
    DECLARE_CONSTANT(FMOD_3D_CUSTOMROLLOFF)                    \
    DECLARE_CONSTANT(FMOD_3D_HEADRELATIVE)                     \
    DECLARE_CONSTANT(FMOD_3D_IGNOREGEOMETRY)                   \
    DECLARE_CONSTANT(FMOD_3D_INVERSEROLLOFF)                   \
    DECLARE_CONSTANT(FMOD_3D_INVERSETAPEREDROLLOFF)            \
    DECLARE_CONSTANT(FMOD_3D_LINEARROLLOFF)                    \
    DECLARE_CONSTANT(FMOD_3D_LINEARSQUAREROLLOFF)              \
    DECLARE_CONSTANT(FMOD_3D_WORLDRELATIVE)                    \
    DECLARE_CONSTANT(FMOD_ACCURATETIME)                        \
    DECLARE_CONSTANT(FMOD_CREATECOMPRESSEDSAMPLE)              \
    DECLARE_CONSTANT(FMOD_CREATESAMPLE)                        \
    DECLARE_CONSTANT(FMOD_CREATESTREAM)                        \
    DECLARE_CONSTANT(FMOD_DEFAULT)                             \
    DECLARE_CONSTANT(FMOD_IGNORETAGS)                          \
    DECLARE_CONSTANT(FMOD_LOOP_BIDI)                           \
    DECLARE_CONSTANT(FMOD_LOOP_NORMAL)                         \
    DECLARE_CONSTANT(FMOD_LOOP_OFF)                            \
    DECLARE_CONSTANT(FMOD_LOWMEM)                              \
    DECLARE_CONSTANT(FMOD_MPEGSEARCH)                          \
    DECLARE_CONSTANT(FMOD_NONBLOCKING)                         \
    DECLARE_CONSTANT(FMOD_OPENMEMORY)                          \
    DECLARE_CONSTANT(FMOD_OPENMEMORY_POINT)                    \
    DECLARE_CONSTANT(FMOD_OPENONLY)                            \
    DECLARE_CONSTANT(FMOD_OPENRAW)                             \
    DECLARE_CONSTANT(FMOD_OPENUSER)                            \
    DECLARE_CONSTANT(FMOD_UNIQUE)                              \
    DECLARE_CONSTANT(FMOD_VIRTUAL_PLAYFROMSTART)               \
                                                               \
    DECLARE_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT)            \
    DECLARE_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE)               \
    DECLARE_CONSTANT(FMOD_STUDIO_STOP_FORCEINT)

#define REGISTER_ALL_CONSTANTS                                  \
    REGISTER_CONSTANT(FMOD_INIT_3D_RIGHTHANDED)                 \
    REGISTER_CONSTANT(FMOD_INIT_CHANNEL_DISTANCEFILTER)         \
    REGISTER_CONSTANT(FMOD_INIT_CHANNEL_LOWPASS)                \
    REGISTER_CONSTANT(FMOD_INIT_GEOMETRY_USECLOSEST)            \
    REGISTER_CONSTANT(FMOD_INIT_MIX_FROM_UPDATE)                \
    REGISTER_CONSTANT(FMOD_INIT_NORMAL)                         \
    REGISTER_CONSTANT(FMOD_INIT_PREFER_DOLBY_DOWNMIX)           \
    REGISTER_CONSTANT(FMOD_INIT_PROFILE_ENABLE)                 \
    REGISTER_CONSTANT(FMOD_INIT_PROFILE_METER_ALL)              \
    REGISTER_CONSTANT(FMOD_INIT_STREAM_FROM_UPDATE)             \
    REGISTER_CONSTANT(FMOD_INIT_THREAD_UNSAFE)                  \
    REGISTER_CONSTANT(FMOD_INIT_VOL0_BECOMES_VIRTUAL)           \
                                                                \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_NORMAL)                  \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_LIVEUPDATE)              \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS)   \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE)      \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_DEFERRED_CALLBACKS)      \
    REGISTER_CONSTANT(FMOD_STUDIO_INIT_LOAD_FROM_UPDATE)        \
                                                                \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_5POINT1)                 \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_7POINT1)                 \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_7POINT1POINT4)           \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_DEFAULT)                 \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_MAX)                     \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_MONO)                    \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_QUAD)                    \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_RAW)                     \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_STEREO)                  \
    REGISTER_CONSTANT(FMOD_SPEAKERMODE_SURROUND)                \
                                                                \
    REGISTER_CONSTANT(FMOD_STUDIO_LOAD_BANK_NORMAL)             \
    REGISTER_CONSTANT(FMOD_STUDIO_LOAD_BANK_NONBLOCKING)        \
    REGISTER_CONSTANT(FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES) \
                                                                \
    REGISTER_CONSTANT(FMOD_2D)                                  \
    REGISTER_CONSTANT(FMOD_3D)                                  \
    REGISTER_CONSTANT(FMOD_3D_CUSTOMROLLOFF)                    \
    REGISTER_CONSTANT(FMOD_3D_HEADRELATIVE)                     \
    REGISTER_CONSTANT(FMOD_3D_IGNOREGEOMETRY)                   \
    REGISTER_CONSTANT(FMOD_3D_INVERSEROLLOFF)                   \
    REGISTER_CONSTANT(FMOD_3D_INVERSETAPEREDROLLOFF)            \
    REGISTER_CONSTANT(FMOD_3D_LINEARROLLOFF)                    \
    REGISTER_CONSTANT(FMOD_3D_LINEARSQUAREROLLOFF)              \
    REGISTER_CONSTANT(FMOD_3D_WORLDRELATIVE)                    \
    REGISTER_CONSTANT(FMOD_ACCURATETIME)                        \
    REGISTER_CONSTANT(FMOD_CREATECOMPRESSEDSAMPLE)              \
    REGISTER_CONSTANT(FMOD_CREATESAMPLE)                        \
    REGISTER_CONSTANT(FMOD_CREATESTREAM)                        \
    REGISTER_CONSTANT(FMOD_DEFAULT)                             \
    REGISTER_CONSTANT(FMOD_IGNORETAGS)                          \
    REGISTER_CONSTANT(FMOD_LOOP_BIDI)                           \
    REGISTER_CONSTANT(FMOD_LOOP_NORMAL)                         \
    REGISTER_CONSTANT(FMOD_LOOP_OFF)                            \
    REGISTER_CONSTANT(FMOD_LOWMEM)                              \
    REGISTER_CONSTANT(FMOD_MPEGSEARCH)                          \
    REGISTER_CONSTANT(FMOD_NONBLOCKING)                         \
    REGISTER_CONSTANT(FMOD_OPENMEMORY)                          \
    REGISTER_CONSTANT(FMOD_OPENMEMORY_POINT)                    \
    REGISTER_CONSTANT(FMOD_OPENONLY)                            \
    REGISTER_CONSTANT(FMOD_OPENRAW)                             \
    REGISTER_CONSTANT(FMOD_OPENUSER)                            \
    REGISTER_CONSTANT(FMOD_UNIQUE)                              \
    REGISTER_CONSTANT(FMOD_VIRTUAL_PLAYFROMSTART)               \
                                                                \
    REGISTER_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT)            \
    REGISTER_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE)               \
    REGISTER_CONSTANT(FMOD_STUDIO_STOP_FORCEINT)

#endif// GODOTFMOD_CONSTANTS_H
