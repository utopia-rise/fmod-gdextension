#ifndef GODOTFMOD_ENUM_H
#define GODOTFMOD_ENUM_H

#include <core/binder_common.hpp>
#include <core/class_db.hpp>
#include <core/type_info.hpp>

// FMOD's C enums belong to no class, so VARIANT_ENUM_CAST would name their type without one, while their constants are
// bound in a class: the API would then reference enums it never declares. FMOD_ENUM_CAST names them <class>.<enum>.
#define FMOD_ENUM_TYPE_INFO(m_class, m_enum, m_impl)                                                                  \
    template<>                                                                                                        \
    struct GetTypeInfo<m_impl> {                                                                                      \
        static constexpr Variant::Type VARIANT_TYPE = Variant::INT;                                                   \
        static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE; \
        static inline PropertyInfo get_class_info() {                                                                 \
            return make_property_info(                                                                                \
              Variant::Type::INT, "", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,  \
              #m_class "." #m_enum                                                                                    \
            );                                                                                                        \
        }                                                                                                             \
    };

#define FMOD_ENUM_CAST(m_class, m_enum)                                               \
    namespace godot {                                                                 \
        FMOD_ENUM_TYPE_INFO(m_class, m_enum, m_enum)                                  \
        FMOD_ENUM_TYPE_INFO(m_class, m_enum, m_enum const)                            \
        FMOD_ENUM_TYPE_INFO(m_class, m_enum, m_enum&)                                 \
        FMOD_ENUM_TYPE_INFO(m_class, m_enum, const m_enum&)                           \
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

// FMOD's flags are #defines of an unsigned int typedef, which has no type of its own to cast: they are bound as a
// bitfield named after the typedef.
#define BIND_FMOD_FLAG(m_flags, m_constant) \
    ::godot::ClassDB::bind_integer_constant(get_class_static(), #m_flags, #m_constant, m_constant, true);

#endif// GODOTFMOD_ENUM_H
