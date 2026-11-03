#ifndef GODOTFMOD_BIND_CONSTANTS_H
#define GODOTFMOD_BIND_CONSTANTS_H

#include <fmod_common.h>
#include <fmod_studio_common.h>

#include <core/binder_common.hpp>
#include <core/class_db.hpp>
#include <core/type_info.hpp>

// FMOD's C enums belong to no class, so VARIANT_ENUM_CAST would name their type without one, while their constants are
// bound in FmodServer: the API would then reference enums it never declares. FMOD_ENUM_CAST names them FmodServer.<enum>.
#define FMOD_ENUM_TYPE_INFO(m_enum, m_impl)                                                                           \
    template<>                                                                                                        \
    struct GetTypeInfo<m_impl> {                                                                                      \
        static constexpr Variant::Type VARIANT_TYPE = Variant::INT;                                                   \
        static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE; \
        static inline PropertyInfo get_class_info() {                                                                 \
            return make_property_info(                                                                                \
              Variant::Type::INT, "", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,  \
              "FmodServer." #m_enum                                                                                   \
            );                                                                                                        \
        }                                                                                                             \
    };

#define FMOD_ENUM_CAST(m_enum)                                                        \
    namespace godot {                                                                 \
        FMOD_ENUM_TYPE_INFO(m_enum, m_enum)                                           \
        FMOD_ENUM_TYPE_INFO(m_enum, m_enum const)                                     \
        FMOD_ENUM_TYPE_INFO(m_enum, m_enum&)                                          \
        FMOD_ENUM_TYPE_INFO(m_enum, const m_enum&)                                    \
        template<>                                                                    \
        struct VariantCaster<m_enum> {                                                \
            static _FORCE_INLINE_ m_enum cast(const Variant& p_variant) {             \
                return static_cast<m_enum>(p_variant.operator int64_t());             \
            }                                                                         \
        };                                                                            \
        template<>                                                                    \
        struct PtrToArg<m_enum> {                                                     \
            _FORCE_INLINE_ static m_enum convert(const void* p_ptr) {                 \
                return static_cast<m_enum>(*reinterpret_cast<const int64_t*>(p_ptr)); \
            }                                                                         \
            typedef int64_t EncodeT;                                                  \
            _FORCE_INLINE_ static void encode(m_enum p_val, void* p_ptr) {            \
                *reinterpret_cast<int64_t*>(p_ptr) = static_cast<int64_t>(p_val);     \
            }                                                                         \
        };                                                                            \
    }

FMOD_ENUM_CAST(FMOD_SPEAKERMODE)
FMOD_ENUM_CAST(FMOD_STUDIO_STOP_MODE)
FMOD_ENUM_CAST(FMOD_STUDIO_LOADING_STATE)
FMOD_ENUM_CAST(FMOD_STUDIO_PLAYBACK_STATE)
FMOD_ENUM_CAST(FMOD_DEBUG_MODE)

// FMOD's flags are #defines of an unsigned int typedef, which has no type of its own to cast: they are bound as a
// bitfield named after the typedef.
#define BIND_FMOD_FLAG(m_flags, m_constant)                                                               \
    ::godot::ClassDB::bind_integer_constant(get_class_static(), #m_flags, #m_constant, m_constant, true);

#define REGISTER_ALL_CONSTANTS                                                                            \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_3D_RIGHTHANDED)                                              \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_CHANNEL_DISTANCEFILTER)                                      \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_CHANNEL_LOWPASS)                                             \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_GEOMETRY_USECLOSEST)                                         \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_NORMAL)                                                      \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_PREFER_DOLBY_DOWNMIX)                                        \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_PROFILE_ENABLE)                                              \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_PROFILE_METER_ALL)                                           \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_STREAM_FROM_UPDATE)                                          \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_THREAD_UNSAFE)                                               \
    BIND_FMOD_FLAG(FMOD_INITFLAGS, FMOD_INIT_VOL0_BECOMES_VIRTUAL)                                        \
                                                                                                          \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_NORMAL)                                        \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_LIVEUPDATE)                                    \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS)                         \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE)                            \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_DEFERRED_CALLBACKS)                            \
    BIND_FMOD_FLAG(FMOD_STUDIO_INITFLAGS, FMOD_STUDIO_INIT_LOAD_FROM_UPDATE)                              \
                                                                                                          \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_5POINT1)                                                          \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_7POINT1)                                                          \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_7POINT1POINT4)                                                    \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_DEFAULT)                                                          \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_MAX)                                                              \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_MONO)                                                             \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_QUAD)                                                             \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_RAW)                                                              \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_STEREO)                                                           \
    BIND_ENUM_CONSTANT(FMOD_SPEAKERMODE_SURROUND)                                                         \
                                                                                                          \
    BIND_FMOD_FLAG(FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_LOAD_BANK_NORMAL)                             \
    BIND_FMOD_FLAG(FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_LOAD_BANK_NONBLOCKING)                        \
    BIND_FMOD_FLAG(FMOD_STUDIO_LOAD_BANK_FLAGS, FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES)                 \
                                                                                                          \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_2D)                                                                    \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D)                                                                    \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_CUSTOMROLLOFF)                                                      \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_HEADRELATIVE)                                                       \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_IGNOREGEOMETRY)                                                     \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_INVERSEROLLOFF)                                                     \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_INVERSETAPEREDROLLOFF)                                              \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_LINEARROLLOFF)                                                      \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_LINEARSQUAREROLLOFF)                                                \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_3D_WORLDRELATIVE)                                                      \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_ACCURATETIME)                                                          \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_CREATECOMPRESSEDSAMPLE)                                                \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_CREATESAMPLE)                                                          \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_CREATESTREAM)                                                          \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_DEFAULT)                                                               \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_IGNORETAGS)                                                            \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_LOOP_BIDI)                                                             \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_LOOP_NORMAL)                                                           \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_LOOP_OFF)                                                              \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_LOWMEM)                                                                \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_MPEGSEARCH)                                                            \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_NONBLOCKING)                                                           \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_OPENMEMORY)                                                            \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_OPENMEMORY_POINT)                                                      \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_OPENONLY)                                                              \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_OPENRAW)                                                               \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_OPENUSER)                                                              \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_UNIQUE)                                                                \
    BIND_FMOD_FLAG(FMOD_MODE, FMOD_VIRTUAL_PLAYFROMSTART)                                                 \
                                                                                                          \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_STOP_ALLOWFADEOUT)                                                     \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_STOP_IMMEDIATE)                                                        \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_STOP_FORCEINT)                                                         \
                                                                                                          \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_PREUPDATE)               \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_POSTUPDATE)              \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_BANK_UNLOAD)             \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_CONNECTED)    \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_DISCONNECTED) \
    BIND_FMOD_FLAG(FMOD_STUDIO_SYSTEM_CALLBACK_TYPE, FMOD_STUDIO_SYSTEM_CALLBACK_ALL)                     \
                                                                                                          \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_CREATED)                   \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_DESTROYED)                 \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_STARTING)                  \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_STARTED)                   \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_RESTARTED)                 \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_STOPPED)                   \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_START_FAILED)              \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND)   \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND)  \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_CREATED)            \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED)          \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER)           \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT)             \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED)              \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED)             \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL)           \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL)           \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_START_EVENT_COMMAND)       \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_NESTED_TIMELINE_BEAT)      \
    BIND_FMOD_FLAG(FMOD_STUDIO_EVENT_CALLBACK_TYPE, FMOD_STUDIO_EVENT_CALLBACK_ALL)                       \
                                                                                                          \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADING)                                               \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_UNLOADED)                                                \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADING)                                                 \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_LOADED)                                                  \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_ERROR)                                                   \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_LOADING_STATE_FORCEINT)                                                \
                                                                                                          \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_PLAYING)                                                      \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_SUSTAINING)                                                   \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPED)                                                      \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STARTING)                                                     \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_STOPPING)                                                     \
    BIND_ENUM_CONSTANT(FMOD_STUDIO_PLAYBACK_FORCEINT)                                                     \
                                                                                                          \
    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_TTY)                                                               \
    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_CALLBACK)                                                          \
    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_FILE)

#endif// GODOTFMOD_BIND_CONSTANTS_H
