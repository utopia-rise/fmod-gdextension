#include "fmod_string_names.h"

using namespace godot;

FmodStringNames* FmodStringNames::instance = nullptr;

void FmodStringNames::create() {
    instance = memnew(FmodStringNames);
}

void FmodStringNames::free() {
    memdelete(instance);
    instance = nullptr;
}

FmodStringNames* FmodStringNames::get_instance() {
    return instance;
}

FmodStringNames::FmodStringNames() : bank_path_property_name("bank_paths") {

}
