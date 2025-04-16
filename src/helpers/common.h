#ifndef GODOTFMOD_COMMON_H
#define GODOTFMOD_COMMON_H

#include "classes/canvas_item.hpp"
#include "classes/node3d.hpp"
#include "fmod_common.h"
#include "variant/utility_functions.hpp"

#include <fmod_errors.h>
#include <fmod_studio_common.h>
#include <helpers/current_function.h>

#include <godot.hpp>
#include <variant/utility_functions.hpp>

#define MAX_PATH_SIZE 512
#define MAX_DRIVER_NAME_SIZE 256

#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_DESCRIPTION_COUNT 256
#define MAX_EVENT_INSTANCE_COUNT 128

#define GODOT_LOG_INFO(message) UtilityFunctions::print(message);
#define GODOT_LOG_VERBOSE(message) UtilityFunctions::print_verbose(message);
#define GODOT_LOG_WARNING(message) UtilityFunctions::push_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
#define GODOT_LOG_ERROR(message) UtilityFunctions::push_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);

#define CHECK_SIZE(maxSize, actualSize, type)                                                                          \
    if ((actualSize) > (maxSize)) {                                                                                    \
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " \
                       + String::num(actualSize) + " entries";                                                         \
        GODOT_LOG_ERROR(message)                                                                                          \
        (actualSize) = maxSize;                                                                                        \
    }

#define ERROR_CHECK_WITH_REASON(_result, _reason) \
(((_result) != FMOD_OK) ? (godot::UtilityFunctions::push_error(FMOD_ErrorString(_result), BOOST_CURRENT_FUNCTION, __FILE__, __LINE__), \
                           godot::UtilityFunctions::push_error(_reason, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__), false) : true)

#define ERROR_CHECK(_result) \
(((_result) != FMOD_OK) ? (godot::UtilityFunctions::push_error(FMOD_ErrorString(_result), BOOST_CURRENT_FUNCTION, __FILE__, __LINE__), false) : true)

#define FMODCLASS(m_class, m_inherits, m_owned)               \
    GDCLASS(m_class, m_inherits)                              \
                                                              \
private:                                                      \
    m_owned* _wrapped = nullptr;                              \
                                                              \
public:                                                       \
    inline static Ref<m_class> create_ref(m_owned* wrapped) { \
        Ref<m_class> ref;                                     \
        if (wrapped) {                                        \
            ref.instantiate();                                \
            ref->_wrapped = wrapped;                          \
        }                                                     \
        return ref;                                           \
    }                                                         \
                                                              \
    bool is_valid() const {                                   \
        return _wrapped != nullptr && _wrapped->isValid();    \
    }                                                         \
                                                              \
    m_owned* get_wrapped() const {                            \
        return _wrapped;                                      \
    }                                                         \
                                                              \
private:

#define FMODCLASSWITHPATH(m_class, m_inherits, m_owned)                  \
    GDCLASS(m_class, m_inherits)                                         \
                                                                         \
private:                                                                 \
    m_owned* _wrapped = nullptr;                                         \
    FMOD_GUID _guid;                                                     \
    String _path;                                                        \
                                                                         \
public:                                                                  \
    inline static Ref<m_class> create_ref(m_owned* wrapped) {            \
        Ref<m_class> ref;                                                \
        if (wrapped) {                                                   \
            ref.instantiate();                                           \
            ref->_wrapped = wrapped;                                     \
            char path[MAX_PATH_SIZE];                                    \
            ERROR_CHECK(wrapped->getPath(path, MAX_PATH_SIZE, nullptr)); \
            ERROR_CHECK(wrapped->getID(&ref->_guid));                    \
            ref->_path = String(path);                                   \
        }                                                                \
        return ref;                                                      \
    }                                                                    \
                                                                         \
    bool is_valid() const {                                              \
        return _wrapped != nullptr && _wrapped->isValid();               \
    }                                                                    \
                                                                         \
    m_owned* get_wrapped() const {                                       \
        return _wrapped;                                                 \
    }                                                                    \
                                                                         \
    String get_path() const {                                            \
        return _path;                                                    \
    }                                                                    \
                                                                         \
    FMOD_GUID get_guid() const {                                         \
        return _guid;                                                    \
    }                                                                    \
                                                                         \
    String get_guid_as_string() const {                                  \
        return fmod_guid_to_string(_guid);                               \
    }                                                                    \
                                                                         \
private:

namespace godot {

    class NodeWrapper {
        Node* node {nullptr};
        ObjectID id;

    public:
        _FORCE_INLINE_ static bool is_spatial_node(Object* p_object) {
            return Node::cast_to<Node3D>(p_object) || Node::cast_to<CanvasItem>(p_object);
        }

        bool is_valid() {
            if (!node || !id.is_valid() || !UtilityFunctions::is_instance_id_valid(id)) { return false; }
            return node->is_inside_tree();
        }

        Node* get_node() { return node; }

        void set_node(Node* p_node) {
            if (p_node) {
                if (!is_spatial_node(p_node)) {
                    GODOT_LOG_ERROR("Invalid Object. A Godot object bound to FMOD has to be either a Node3D or CanvasItem.")
                    return;
                }
                node = p_node;
                id = p_node->get_instance_id();
            }
            node = nullptr;
        }

        NodeWrapper() = default;
        explicit NodeWrapper(Node* p_node) { set_node(p_node); };
    };

    template<class T>
    static inline Ref<T> create_ref() {
        Ref<T> ref;
        ref.instantiate();
        return ref;
    }

    static inline FMOD_GUID string_to_fmod_guid(const char* guid) {
        FMOD_GUID result;
        sscanf(guid,
               "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
               &result.Data1, &result.Data2, &result.Data3,
               &result.Data4[0], &result.Data4[1], &result.Data4[2], &result.Data4[3],
               &result.Data4[4], &result.Data4[5], &result.Data4[6], &result.Data4[7]);
        return result;
    }

    static inline String fmod_guid_to_string(const FMOD_GUID& guid) {
        char result[39];
        snprintf(result, sizeof(result), "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                 guid.Data1, guid.Data2, guid.Data3,
                 guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                 guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        return result;
    }

    static inline uint64_t fmod_parameter_id_to_ulong(const FMOD_STUDIO_PARAMETER_ID& parameter_id) {
        const unsigned int first_id_part {parameter_id.data1};
        return (static_cast<uint64_t>(first_id_part) << 32) | static_cast<uint64_t>(parameter_id.data2);
    }

    static inline FMOD_STUDIO_PARAMETER_ID ulong_to_fmod_parameter_id(uint64_t converted) {
        FMOD_STUDIO_PARAMETER_ID paramId;
        paramId.data2 = static_cast<unsigned int>(converted & 0xFFFFFFFF);
        paramId.data1 = static_cast<unsigned int>((converted >> 32) & 0xFFFFFFFF);
        return paramId;
    }

    static inline bool equals(const FMOD_GUID& first, const FMOD_GUID& second) {
        return first.Data1 == second.Data1
        && first.Data2 == second.Data2
        && first.Data3 == second.Data3
        && first.Data4[0] == second.Data4[0]
        && first.Data4[1] == second.Data4[1]
        && first.Data4[2] == second.Data4[2]
        && first.Data4[3] == second.Data4[3]
        && first.Data4[4] == second.Data4[4]
        && first.Data4[5] == second.Data4[5]
        && first.Data4[6] == second.Data4[6]
        && first.Data4[7] == second.Data4[7];
    }
}// namespace godot

#endif// GODOTFMOD_COMMON_H
