#include "fmod_bank.h"
#include "helpers/common.h"

using namespace godot;

void FmodBank::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_loading_state"), &FmodBank::get_loading_state);
    ClassDB::bind_method(D_METHOD("get_bus_count"), &FmodBank::get_bus_count);
    ClassDB::bind_method(D_METHOD("get_event_count"), &FmodBank::get_event_count);
    ClassDB::bind_method(D_METHOD("get_string_count"), &FmodBank::get_string_count);
    ClassDB::bind_method(D_METHOD("get_VCA_count"), &FmodBank::get_vca_count);
}

int FmodBank::get_loading_state() {
    FMOD_STUDIO_LOADING_STATE state;
    ERROR_CHECK(bank->getLoadingState(&state));
    return state;
}

int FmodBank::get_bus_count() {
    int count = -1;
    ERROR_CHECK(bank->getBusCount(&count));
    return count;
}

int FmodBank::get_event_count() {
    int count = -1;
    ERROR_CHECK(bank->getEventCount(&count));
    return count;
}

int FmodBank::get_string_count() {
    int count = -1;
    ERROR_CHECK(bank->getStringCount(&count));
    return count;
}

int FmodBank::get_vca_count() {
    int count = -1;
    ERROR_CHECK(bank->getVCACount(&count));
    return count;
}