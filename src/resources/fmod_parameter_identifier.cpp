#include "fmod_parameter_identifier.h"

using namespace godot;

void FmodParameterIdentifier::set_parameter_id(const uint64_t p_id) {
    id = p_id;
}

uint64_t FmodParameterIdentifier::get_parameter_id() const {
    return id;
}

void FmodParameterIdentifier::set_parameter_name(const String& p_name) {
    name = p_name;
}

const String& FmodParameterIdentifier::get_parameter_name() const {
    return name;
}

void FmodParameterIdentifier::set_load_by_name(const bool p_load_by_name) {
    load_by_name = p_load_by_name;
}

bool FmodParameterIdentifier::get_load_by_name() const {
    return load_by_name;
}

void FmodParameterIdentifier::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_parameter_id", "p_id"), &FmodParameterIdentifier::set_parameter_id);
    ClassDB::bind_method(D_METHOD("get_parameter_id"), &FmodParameterIdentifier::get_parameter_id);
    ClassDB::bind_method(D_METHOD("set_parameter_name", "p_name"), &FmodParameterIdentifier::set_parameter_name);
    ClassDB::bind_method(D_METHOD("get_parameter_name"), &FmodParameterIdentifier::get_parameter_name);
    ClassDB::bind_method(D_METHOD("set_load_by_name", "p_load_by_name"), &FmodParameterIdentifier::set_load_by_name);
    ClassDB::bind_method(D_METHOD("get_load_by_name"), &FmodParameterIdentifier::get_load_by_name);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "parameter_id",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_parameter_id", "get_parameter_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "parameter_name",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_parameter_name", "get_parameter_name");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "load_by_name",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_load_by_name", "get_load_by_name");
}