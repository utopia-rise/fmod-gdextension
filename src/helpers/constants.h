#ifndef GODOTFMOD_CONSTANTS_H
#define GODOTFMOD_CONSTANTS_H

#include <core/class_db.hpp>
#include <fmod_common.h>

#define REGISTER_ALL_CONSTANTS                                         \
    BIND_CONSTANT(FMOD_INIT_3D_RIGHTHANDED)                            \
    BIND_CONSTANT(FMOD_INIT_CHANNEL_DISTANCEFILTER)                    \
    BIND_CONSTANT(FMOD_INIT_CHANNEL_LOWPASS)                           \
    BIND_CONSTANT(FMOD_INIT_GEOMETRY_USECLOSEST)                       \
    BIND_CONSTANT(FMOD_INIT_MIX_FROM_UPDATE)                           \
    BIND_CONSTANT(FMOD_INIT_NORMAL)                                    \
    BIND_CONSTANT(FMOD_INIT_PREFER_DOLBY_DOWNMIX)                      \
    BIND_CONSTANT(FMOD_INIT_PROFILE_ENABLE)                            \
    BIND_CONSTANT(FMOD_INIT_PROFILE_METER_ALL)                         \
    BIND_CONSTANT(FMOD_INIT_STREAM_FROM_UPDATE)                        \
    BIND_CONSTANT(FMOD_INIT_THREAD_UNSAFE)                             \
    BIND_CONSTANT(FMOD_INIT_VOL0_BECOMES_VIRTUAL)                      \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_INIT_NORMAL)                             \
    BIND_CONSTANT(FMOD_STUDIO_INIT_LIVEUPDATE)                         \
    BIND_CONSTANT(FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS)              \
    BIND_CONSTANT(FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE)                 \
    BIND_CONSTANT(FMOD_STUDIO_INIT_DEFERRED_CALLBACKS)                 \
    BIND_CONSTANT(FMOD_STUDIO_INIT_LOAD_FROM_UPDATE)                   \
                                                                       \
    BIND_CONSTANT(FMOD_SPEAKERMODE_5POINT1)                            \
    BIND_CONSTANT(FMOD_SPEAKERMODE_7POINT1)                            \
    BIND_CONSTANT(FMOD_SPEAKERMODE_7POINT1POINT4)                      \
    BIND_CONSTANT(FMOD_SPEAKERMODE_DEFAULT)                            \
    BIND_CONSTANT(FMOD_SPEAKERMODE_MAX)                                \
    BIND_CONSTANT(FMOD_SPEAKERMODE_MONO)                               \
    BIND_CONSTANT(FMOD_SPEAKERMODE_QUAD)                               \
    BIND_CONSTANT(FMOD_SPEAKERMODE_RAW)                                \
    BIND_CONSTANT(FMOD_SPEAKERMODE_STEREO)                             \
    BIND_CONSTANT(FMOD_SPEAKERMODE_SURROUND)                           \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NORMAL)                        \
    BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_NONBLOCKING)                   \
    BIND_CONSTANT(FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES)            \
                                                                       \
    BIND_CONSTANT(FMOD_2D)                                             \
    BIND_CONSTANT(FMOD_3D)                                             \
    BIND_CONSTANT(FMOD_3D_CUSTOMROLLOFF)                               \
    BIND_CONSTANT(FMOD_3D_HEADRELATIVE)                                \
    BIND_CONSTANT(FMOD_3D_IGNOREGEOMETRY)                              \
    BIND_CONSTANT(FMOD_3D_INVERSEROLLOFF)                              \
    BIND_CONSTANT(FMOD_3D_INVERSETAPEREDROLLOFF)                       \
    BIND_CONSTANT(FMOD_3D_LINEARROLLOFF)                               \
    BIND_CONSTANT(FMOD_3D_LINEARSQUAREROLLOFF)                         \
    BIND_CONSTANT(FMOD_3D_WORLDRELATIVE)                               \
    BIND_CONSTANT(FMOD_ACCURATETIME)                                   \
    BIND_CONSTANT(FMOD_CREATECOMPRESSEDSAMPLE)                         \
    BIND_CONSTANT(FMOD_CREATESAMPLE)                                   \
    BIND_CONSTANT(FMOD_CREATESTREAM)                                   \
    BIND_CONSTANT(FMOD_DEFAULT)                                        \
    BIND_CONSTANT(FMOD_IGNORETAGS)                                     \
    BIND_CONSTANT(FMOD_LOOP_BIDI)                                      \
    BIND_CONSTANT(FMOD_LOOP_NORMAL)                                    \
    BIND_CONSTANT(FMOD_LOOP_OFF)                                       \
    BIND_CONSTANT(FMOD_LOWMEM)                                         \
    BIND_CONSTANT(FMOD_MPEGSEARCH)                                     \
    BIND_CONSTANT(FMOD_NONBLOCKING)                                    \
    BIND_CONSTANT(FMOD_OPENMEMORY)                                     \
    BIND_CONSTANT(FMOD_OPENMEMORY_POINT)                               \
    BIND_CONSTANT(FMOD_OPENONLY)                                       \
    BIND_CONSTANT(FMOD_OPENRAW)                                        \
    BIND_CONSTANT(FMOD_OPENUSER)                                       \
    BIND_CONSTANT(FMOD_UNIQUE)                                         \
    BIND_CONSTANT(FMOD_VIRTUAL_PLAYFROMSTART)                          \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT)                       \
    BIND_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE)                          \
    BIND_CONSTANT(FMOD_STUDIO_STOP_FORCEINT)                           \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_PREUPDATE)               \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_POSTUPDATE)              \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_BANK_UNLOAD)             \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_CONNECTED)    \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_DISCONNECTED) \
    BIND_CONSTANT(FMOD_STUDIO_SYSTEM_CALLBACK_ALL)                     \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_CREATED)                  \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_DESTROYED)                \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STARTING)                 \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STARTED)                  \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_RESTARTED)                \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_STOPPED)                  \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_START_FAILED)             \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND)  \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND) \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_CREATED)           \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED)         \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER)          \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT)            \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED)             \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED)            \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL)          \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL)          \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_START_EVENT_COMMAND)      \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_NESTED_TIMELINE_BEAT)     \
    BIND_CONSTANT(FMOD_STUDIO_EVENT_CALLBACK_ALL)                      \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADING)                 \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADED)                  \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADING)                   \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADED)                    \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_ERROR)                     \
    BIND_CONSTANT(FMOD_STUDIO_LOADING_STATE_FORCEINT)                  \
                                                                       \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_PLAYING)                        \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_SUSTAINING)                     \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPED)                        \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STARTING)                       \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPING)                       \
    BIND_CONSTANT(FMOD_STUDIO_PLAYBACK_FORCEINT)

#endif// GODOTFMOD_CONSTANTS_H
