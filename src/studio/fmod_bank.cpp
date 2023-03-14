#include "fmod_bank.h"
#include "helpers/common.h"

using namespace godot;

void FmodBank::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_loading_state"), &FmodBank::get_loading_state);
    ClassDB::bind_method(D_METHOD("get_bus_count"), &FmodBank::get_bus_count);
    ClassDB::bind_method(D_METHOD("get_event_count"), &FmodBank::get_event_count);
    ClassDB::bind_method(D_METHOD("get_string_count"), &FmodBank::get_string_count);
    ClassDB::bind_method(D_METHOD("get_VCA_count"), &FmodBank::get_vca_count);
    ClassDB::bind_method(D_METHOD("get_description_list"), &FmodBank::get_description_list);
    ClassDB::bind_method(D_METHOD("get_bus_list"), &FmodBank::get_bus_list);
    ClassDB::bind_method(D_METHOD("get_vca_list"), &FmodBank::get_vca_list);
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

int FmodBank::get_event_description_count() {
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

Array FmodBank::get_description_list() {
    Array array;
    FMOD::Studio::EventDescription* instances[MAX_EVENT_DESCRIPTION_COUNT];
    int count = 0;
    ERROR_CHECK(bank->getEventList(instances, MAX_EVENT_DESCRIPTION_COUNT, &count));
    CHECK_SIZE(MAX_EVENT_DESCRIPTION_COUNT, count, events)
    for (int i = 0; i < count; i++) {
        array.append((uint64_t) instances[i]);
    }
    return array;
}

Array FmodBank::get_bus_list() {
    Array array;
    FMOD::Studio::EventDescription* instances[MAX_BUS_COUNT];
    int count = 0;
    ERROR_CHECK(bank->getEventList(instances, MAX_BUS_COUNT, &count));
    CHECK_SIZE(MAX_BUS_COUNT, count, events)
    for (int i = 0; i < count; i++) {
        array.append((uint64_t) instances[i]);
    }
    return array;
}

Array FmodBank::get_vca_list() {
    Array array;
    FMOD::Studio::EventDescription* instances[MAX_VCA_COUNT];
    int count = 0;
    ERROR_CHECK(bank->getEventList(instances, MAX_VCA_COUNT, &count));
    CHECK_SIZE(MAX_VCA_COUNT, count, events)
    for (int i = 0; i < count; i++) {
        array.append((uint64_t) instances[i]);
    }
    return array;
}

