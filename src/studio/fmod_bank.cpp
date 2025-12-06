#include "fmod_bank.h"

#include "helpers/common.h"
#include "fmod_server.h"

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
    ClassDB::bind_method(D_METHOD("get_godot_res_path"), &FmodBank::get_godot_res_path);
    ClassDB::bind_method(D_METHOD("get_path"), &FmodBank::get_path);
    ClassDB::bind_method(D_METHOD("get_guid"), &FmodBank::get_guid_as_string);
}

int FmodBank::get_loading_state() {
    FMOD_STUDIO_LOADING_STATE state;

    ERROR_CHECK_WITH_REASON(_wrapped->getLoadingState(&state), vformat("Cannot get loading state for bank %s", get_path()));
    return state;
}

int64_t FmodBank::get_bus_count() {
    return _buses.size();
}

int64_t FmodBank::get_event_description_count() {
    return _event_descriptions.size();
}

int64_t FmodBank::get_vca_count() const {
    return _vcas.size();
}

int FmodBank::get_string_count() const {
    int count = -1;
    ERROR_CHECK_WITH_REASON(_wrapped->getStringCount(&count), vformat("Cannot get string count for bank %s", get_path()));
    return count;
}

Array FmodBank::get_description_list() const {
    Array array;
    for (const Ref<FmodEventDescription>& ref : _event_descriptions) {
        array.append(ref);
    }
    return array;
}

Array FmodBank::get_bus_list() const {
    Array array;
    for (const Ref<FmodBus>& ref : _buses) {
        array.append(ref);
    }
    return array;
}

Array FmodBank::get_vca_list() const {
    Array array;
    for (const Ref<FmodVCA>& ref : _vcas) {
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
    int size = 0;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getVCACount(&size), vformat("Cannot get VCA count for bank %s", get_path()))) {
        if (size == 0) {
            return;
        }

        Vector<FMOD::Studio::VCA*> raw_vcas;
        raw_vcas.resize(size);

        if (ERROR_CHECK_WITH_REASON(_wrapped->getVCAList(raw_vcas.ptrw(), size, &size), vformat("Cannot get VCA list for bank %s", get_path()))) {
            _vcas.clear();

            for (int i = 0; i < size; ++i) {
                Ref<FmodVCA> ref = FmodVCA::create_ref(raw_vcas[i]);
                _vcas.push_back(ref);
            }
        }
    }
}

void FmodBank::load_all_buses() {
    int size = 0;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getBusCount(&size), vformat("Cannot get bus count for bank %s", get_path()))) {
        if (size == 0) {
            return;
        }

        Vector<FMOD::Studio::Bus*> raw_buses;
        raw_buses.resize(size);

        if (ERROR_CHECK_WITH_REASON(_wrapped->getBusList(raw_buses.ptrw(), size, &size), vformat("Cannot get bus list for bank %s", get_path()))) {
            _buses.clear();

            for (int i = 0; i < size; ++i) {
                Ref<FmodBus> ref = FmodBus::create_ref(raw_buses[i]);
                _buses.push_back(ref);
            }
        }
    }
}

void FmodBank::load_all_event_descriptions() {
    int size = 0;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getEventCount(&size), vformat("Cannot get event count for bank %s", get_path()))) {
        if (size == 0) {
            return;
        }

        Vector<FMOD::Studio::EventDescription*> raw_events;
        raw_events.resize(size);

        if (ERROR_CHECK_WITH_REASON(_wrapped->getEventList(raw_events.ptrw(), size, &size), vformat("Cannot get event list for bank %s", get_path()))) {
            _event_descriptions.clear();

            for (int i = 0; i < size; ++i) {
                Ref<FmodEventDescription> ref = FmodEventDescription::create_ref(raw_events[i]);
                _event_descriptions.push_back(ref);
            }
        }
    }
}

const List<Ref<FmodEventDescription>>& FmodBank::get_event_descriptions() const {
    return _event_descriptions;
}

const List<Ref<FmodBus>>& FmodBank::get_buses() const {
    return _buses;
}

const List<Ref<FmodVCA>>& FmodBank::get_vcas() const {
    return _vcas;
}

const String& FmodBank::get_godot_res_path() const {
    return _godot_res_path;
}

FmodBank::~FmodBank() {
    FmodServer::get_singleton()->unload_bank(_godot_res_path);
}
