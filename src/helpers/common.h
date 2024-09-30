#ifndef GODOTFMOD_COMMON_H
#define GODOTFMOD_COMMON_H

#include "classes/canvas_item.hpp"
#include "classes/node3d.hpp"
#include "fmod_common.h"
#include <fmod_studio_common.h>
#include "variant/utility_functions.hpp"

#include <fmod_errors.h>
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

#define ERROR_CHECK(_result) godot::checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

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
    static bool checkErrors(FMOD_RESULT result, const char* function, const char* file, int line) {
        if (result != FMOD_OK) {
            UtilityFunctions::push_error(FMOD_ErrorString(result), function, file, line);
            return false;
        }
        return true;
    }

    static bool is_dead(Object* node) {
        if (!node || !UtilityFunctions::is_instance_id_valid(node->get_instance_id())) {
            return true;
        }
        return !Object::cast_to<Node>(node)->is_inside_tree();
    }

    static bool is_fmod_valid(Object* node) {
        if (node) {
            bool ret = Node::cast_to<Node3D>(node) || Node::cast_to<CanvasItem>(node);
            if (!ret) { GODOT_LOG_ERROR("Invalid Object. A listener has to be either a Node3D or CanvasItem.") }
            return ret;
        }
        GODOT_LOG_ERROR("Object is null")
        return false;
    }

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
}// namespace godot

#endif// GODOTFMOD_COMMON_H
