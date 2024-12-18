#ifndef GODOTFMOD_FMOD_STRING_NAMES_H
#define GODOTFMOD_FMOD_STRING_NAMES_H


#include "godot.hpp"
#include "variant/string_name.hpp"

using namespace godot;

class FmodStringNames {
    friend void initialize_fmod_module(ModuleInitializationLevel p_level);
    friend void uninitialize_fmod_module(ModuleInitializationLevel p_level);

    static void create();
    static void free();

    static FmodStringNames* instance;

    FmodStringNames();

public:
    static FmodStringNames* get_instance();

    static constexpr const char* EVENT_PARAMETER_PREFIX_FOR_PROPERTIES = "fmod_parameters";

    StringName bank_path_property_name;
    StringName event_parameter_prefix_for_properties;
};

#endif //GODOTFMOD_FMOD_STRING_NAMES_H
