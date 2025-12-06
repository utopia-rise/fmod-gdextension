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

bool FmodParameterDescription::is_read_only() const {
    return (_wrapped.flags & FMOD_STUDIO_PARAMETER_READONLY) == FMOD_STUDIO_PARAMETER_READONLY;
}

bool FmodParameterDescription::is_automatic() const {
    return (_wrapped.flags & FMOD_STUDIO_PARAMETER_AUTOMATIC) == FMOD_STUDIO_PARAMETER_AUTOMATIC;
}

bool FmodParameterDescription::is_global() const {
    return (_wrapped.flags & FMOD_STUDIO_PARAMETER_GLOBAL) == FMOD_STUDIO_PARAMETER_GLOBAL;
}

bool FmodParameterDescription::is_discrete() const {
    return (_wrapped.flags & FMOD_STUDIO_PARAMETER_DISCRETE) == FMOD_STUDIO_PARAMETER_DISCRETE;
}

bool FmodParameterDescription::is_labeled() const {
    return (_wrapped.flags & FMOD_STUDIO_PARAMETER_LABELED) == FMOD_STUDIO_PARAMETER_LABELED;
}

void FmodParameterDescription::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &FmodParameterDescription::get_name);
    ClassDB::bind_method(D_METHOD("get_id"), &FmodParameterDescription::get_id);
    ClassDB::bind_method(D_METHOD("get_minimum"), &FmodParameterDescription::get_minimum);
    ClassDB::bind_method(D_METHOD("get_maximum"), &FmodParameterDescription::get_maximum);
    ClassDB::bind_method(D_METHOD("get_default_value"), &FmodParameterDescription::get_default_value);
    ClassDB::bind_method(D_METHOD("is_read_only"), &FmodParameterDescription::is_read_only);
    ClassDB::bind_method(D_METHOD("is_automatic"), &FmodParameterDescription::is_automatic);
    ClassDB::bind_method(D_METHOD("is_global"), &FmodParameterDescription::is_global);
    ClassDB::bind_method(D_METHOD("is_discrete"), &FmodParameterDescription::is_discrete);
    ClassDB::bind_method(D_METHOD("is_labeled"), &FmodParameterDescription::is_labeled);
}