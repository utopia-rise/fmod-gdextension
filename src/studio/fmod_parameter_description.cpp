#include "fmod_parameter_description.h"

#include <helpers/common.h>

using namespace godot;

const String& FmodParameterDescription::get_name() const {
    return _name;
}

uint64_t FmodParameterDescription::get_id() const {
    return fmod_parameter_id_to_ulong(_wrapped.id);
}

float FmodParameterDescription::get_minimum() const {
    return _wrapped.minimum;
}

float FmodParameterDescription::get_maximum() const {
    return _wrapped.maximum;
}

float FmodParameterDescription::get_default_value() const {
    return _wrapped.defaultvalue;
}

void FmodParameterDescription::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &FmodParameterDescription::get_name);
    ClassDB::bind_method(D_METHOD("get_id"), &FmodParameterDescription::get_id);
    ClassDB::bind_method(D_METHOD("get_minimum"), &FmodParameterDescription::get_minimum);
    ClassDB::bind_method(D_METHOD("get_maximum"), &FmodParameterDescription::get_maximum);
    ClassDB::bind_method(D_METHOD("get_default_value"), &FmodParameterDescription::get_default_value);
}