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
	ERROR_CHECK_WITH_REASON(_wrapped->getStringCount(&count), vformat("Cannot get string count for bank %s", get_path()));
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
	int size = 0;
	FMOD::Studio::VCA** array = nullptr;

	if (ERROR_CHECK_WITH_REASON(_wrapped->getVCACount(&size), vformat("Cannot get num VCA for bank %s", get_path()))) {
		array = (FMOD::Studio::VCA**)alloca(sizeof(FMOD::Studio::VCA*) * size);
	}

	if (ERROR_CHECK_WITH_REASON(_wrapped->getVCAList(array, size, &size), vformat("Cannot get VCA list for bank %s", get_path()))) {
		VCAs.clear();
		for (int i = 0; i < size; ++i) {
			Ref<FmodVCA> ref = FmodVCA::create_ref(array[i]);
			VCAs.push_back(ref);
		}
	}
}

void FmodBank::load_all_buses() {
	int size = 0;
	FMOD::Studio::Bus** array = nullptr;

	if (ERROR_CHECK_WITH_REASON(_wrapped->getBusCount(&size), vformat("Cannot get num buses for bank %s", get_path()))) {
		array = (FMOD::Studio::Bus**)alloca(sizeof(FMOD::Studio::Bus*) * size);
	}

	if (ERROR_CHECK_WITH_REASON(_wrapped->getBusList(array, size, &size), vformat("Cannot get bus list for bank %s", get_path()))) {
		buses.clear();
		for (int i = 0; i < size; ++i) {

			if (!array[i] || !array[i]->isValid())
			{
				GODOT_LOG_WARNING("Invalid Object. array[i].")
			}

			Ref<FmodBus> ref = FmodBus::create_ref(array[i]);
			buses.push_back(ref);
		}
	}
}

void FmodBank::load_all_event_descriptions() {

	int size = 0;
	FMOD::Studio::EventDescription** array = nullptr;

	if (ERROR_CHECK_WITH_REASON(_wrapped->getEventCount(&size), vformat("Cannot get num events for bank %s", get_path()))) {
		array = (FMOD::Studio::EventDescription**)alloca(sizeof(FMOD::Studio::EventDescription*) * size);
	}

	if (ERROR_CHECK_WITH_REASON(_wrapped->getEventList(array, size, &size), vformat("Cannot get event list for bank %s", get_path()))) {
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

FmodBank::~FmodBank() {
	FmodServer::get_singleton()->unload_bank(_godot_res_path);
}
