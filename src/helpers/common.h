#ifndef GODOTFMOD_COMMON_H
#define GODOTFMOD_COMMON_H

#include "classes/canvas_item.hpp"
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
#define MAX_DRIVER_NAME_SIZE 256

#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_DESCRIPTION_COUNT 256
#define MAX_EVENT_INSTANCE_COUNT 128

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

#define CHECK_SIZE(maxSize, actualSize, type)                                                                                                                  \
    if ((actualSize) > (maxSize)) {                                                                                                                            \
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " + String::num(actualSize) + " entries"; \
        GODOT_LOG(2, message)                                                                                                                                  \
        (actualSize) = maxSize;                                                                                                                                \
    }

#define ERROR_CHECK(_result) checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)


namespace godot {
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
