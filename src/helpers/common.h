#ifndef GODOTFMOD_COMMON_H
#define GODOTFMOD_COMMON_H

#include "classes/node3d.hpp"
#include "fmod_common.h"
#include "variant/utility_functions.hpp"
#include <fmod_errors.h>
#include <helpers/current_function.h>

#define CUSTOM_FILESYSTEM

#ifndef CUSTOM_FILESYSTEM
    #ifdef __ANDROID__
        #define DRIVE_PATH(path) path = path.replace("res://", "file:///android_asset/");
    #else
        #define DRIVE_PATH(path) path = path.replace("res://", "./");
    #endif
#else
    #define DRIVE_PATH(path)
#endif

#define MAX_PATH_SIZE 512
#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_INSTANCE 128
#define MAX_EVENT_COUNT 256
#define MAX_DRIVER_NAME_SIZE 256

namespace godot {

#define GODOT_LOG(level, message)                                                                \
    switch (level) {                                                                             \
        case 0:                                                                                  \
            UtilityFunctions::print(message);                                                    \
            break;                                                                               \
        case 1:                                                                                  \
            UtilityFunctions::push_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__); \
            break;                                                                               \
        case 2:                                                                                  \
            UtilityFunctions::push_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);   \
            break;                                                                               \
    }

#define FIND_AND_CHECK_WITH_RETURN(instanceId, cont, defaultReturn)                                   \
    auto instance = (cont).get(instanceId);                                                           \
    if (!instance) {                                                                                  \
        String message = vformat("FMOD Sound System: cannot find instance in %s collection.", #cont); \
        GODOT_LOG(2, message)                                                                         \
        return defaultReturn;                                                                         \
    }
#define FIND_AND_CHECK_WITHOUT_RETURN(instanceId, set) FIND_AND_CHECK_WITH_RETURN(instanceId, set, void())
#define FUNC_CHOOSER(_f1, _f2, _f3, _f4, ...) _f4
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define MACRO_CHOOSER(...) FUNC_RECOMPOSER((__VA_ARGS__, FIND_AND_CHECK_WITH_RETURN, FIND_AND_CHECK_WITHOUT_RETURN, ))
#define FIND_AND_CHECK(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CHECK_SIZE(maxSize, actualSize, type)                                                                                                                  \
    if ((actualSize) > (maxSize)) {                                                                                                                            \
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " + String::num(actualSize) + " entries"; \
        GODOT_LOG(2, message)                                                                                                                                  \
        (actualSize) = maxSize;                                                                                                                                \
    }

#define ERROR_CHECK(_result) checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

    bool checkErrors(FMOD_RESULT result, const char* function, const char* file, int line) {
        if (result != FMOD_OK) {
            UtilityFunctions::push_error(FMOD_ErrorString(result), function, file, line);
            return false;
        }
        return true;
    }

    bool is_dead(Object* node) {
        if (!node) {
            return true;
        }
        return !UtilityFunctions::is_instance_valid(Object::cast_to<Node>(node)->get_owner());
    }

    bool is_fmod_valid(Object* node) {
        if (node) {
            bool ret = Node::cast_to<Node3D>(node) || Node::cast_to<CanvasItem>(node);
            if (!ret) {
                GODOT_LOG(2, "Invalid Object. A listener has to be either a Node3D or CanvasItem.")
            }
            return ret;
        }
        GODOT_LOG(2, "Object is null")
        return false;
    }

}// namespace godot

#endif// GODOTFMOD_COMMON_H
