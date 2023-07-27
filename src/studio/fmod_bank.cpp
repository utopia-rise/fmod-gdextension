#include "fmod_bank.h"

#include "helpers/common.h"

using namespace godot;

void FmodBank::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_loading_state"), &FmodBank::get_loading_state);
    ClassDB::bind_method(D_METHOD("get_bus_count"), &FmodBank::get_bus_count);
    ClassDB::bind_method(D_METHOD("get_event_description_count"), &FmodBank::get_event_description_count);
    ClassDB::bind_method(D_METHOD("get_string_count"), &FmodBank::get_string_count);
    ClassDB::bind_method(D_METHOD("get_VCA_count"), &FmodBank::get_vca_count);
    ClassDB::bind_method(D_METHOD("get_description_list"), &FmodBank::get_description_list);
    ClassDB::bind_method(D_METHOD("get_bus_list"), &FmodBank::get_bus_list);
    ClassDB::bind_method(D_METHOD("get_vca_list"), &FmodBank::get_vca_list);
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodBank::is_valid);
}

int FmodBank::get_loading_state() {
    FMOD_STUDIO_LOADING_STATE state;
    ERROR_CHECK(_wrapped->getLoadingState(&state));
    return state;
}

int FmodBank::get_bus_count() {
    return buses.size();
}

int FmodBank::get_event_description_count() {
    return eventDescriptions.size();
}

int FmodBank::get_vca_count() const {
    return VCAs.size();
}

int FmodBank::get_string_count() const {
    int count = -1;
    ERROR_CHECK(_wrapped->getStringCount(&count));
    return count;
}

Array FmodBank::get_description_list() const {
    Array array;
    for (const Ref<FmodEventDescription>& ref : eventDescriptions) {
        array.append(ref);
    }
    return array;
}

Array FmodBank::get_bus_list() const {
    Array array;
    for (const Ref<FmodBus>& ref : buses) {
        array.append(ref);
    }
    return array;
}

Array FmodBank::get_vca_list() const {
    Array array;
    for (const Ref<FmodVCA>& ref : VCAs) {
        array.append(ref);
    }
    return array;
}

void FmodBank::update_bank_data() {
    load_all_buses();
    load_all_vca();
    load_all_event_descriptions();
}

void FmodBank::load_all_vca() {
    FMOD::Studio::VCA* array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(_wrapped->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        VCAs.clear();
        for (int i = 0; i < size; ++i) {
            Ref<FmodVCA> ref = FmodVCA::create_ref(array[i]);
            VCAs.push_back(ref);
        }
    }
}

void FmodBank::load_all_buses() {
    FMOD::Studio::Bus* array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(_wrapped->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        buses.clear();
        for (int i = 0; i < size; ++i) {
            Ref<FmodBus> ref = FmodBus::create_ref(array[i]);
            buses.push_back(ref);
        }
    }
}

void FmodBank::load_all_event_descriptions() {
    FMOD::Studio::EventDescription* array[MAX_EVENT_DESCRIPTION_COUNT];
    int size = 0;
    if (ERROR_CHECK(_wrapped->getEventList(array, MAX_EVENT_DESCRIPTION_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_DESCRIPTION_COUNT, size, Events)
        eventDescriptions.clear();
        for (int i = 0; i < size; ++i) {
            Ref<FmodEventDescription> ref = FmodEventDescription::create_ref(array[i]);
            eventDescriptions.push_back(ref);
        }
    }
}

const List<Ref<FmodEventDescription>>& FmodBank::getEventDescriptions() const {
    return eventDescriptions;
}

const List<Ref<FmodBus>>& FmodBank::getBuses() const {
    return buses;
}

const List<Ref<FmodVCA>>& FmodBank::getVcAs() const {
    return VCAs;
}

const String& FmodBank::get_godot_res_path() const {
    return _godot_res_path;
}