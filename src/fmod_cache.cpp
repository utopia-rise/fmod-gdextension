
#include "fmod_cache.h"
#include "helpers/common.h"

using namespace godot;

void FmodCache::load_pending() {
    if (loadingBanks.size() == 0) {
        return;
    }
    List<LoadingBank*> toDelete;
    for (auto loadingBank : loadingBanks) {
        Ref<FmodBank> bank = loadingBank->bank;
        int loading_state = bank->get_loading_state();
        if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _load_bank_data(loadingBank);
            toDelete.push_back(loadingBank);
        } else if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADING) {
            loadingBanks.push_back(loadingBank);
        } else if (loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            toDelete.push_back(loadingBank);
            GODOT_LOG(2, "Fmod Sound System: Error loading bank.")
        }
    }
    for(auto element : toDelete){
        loadingBanks.erase(element);
        delete element;
    }
}

void FmodCache::_load_bank_data(LoadingBank* loadingBank) {
    char path[MAX_PATH_SIZE];
    auto bank = loadingBank->bank;
    FMOD_RESULT result = bank->getPath(path, MAX_PATH_SIZE, nullptr);
    if (result == FMOD_OK) {
        GODOT_LOG(0, "FMOD Sound System: BANK " + String(path) + " LOADED")
        _load_all_buses(bank);
        _load_all_vca(bank);
        _load_all_event_descriptions(bank);
        banks[loadingBank->godotPath] << bank;
    } else {
        if (result == FMOD_ERR_EVENT_NOTFOUND) {
            GODOT_LOG(0, "FMOD Sound System: BANK " + String(path) + " COULDN'T BE LOADED. Path incorrect or MasterBank not loaded yet.")
        } else {
            ERROR_CHECK(result);
        }
        bank->unload();
    }
}

void FmodCache::_load_all_vca(BankData* bank) {
    FMOD::Studio::VCA* array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to VCAs")
            VCAs[path] << vca;
        }
    }
}

void FmodCache::_load_all_buses(BankData* bank) {
    FMOD::Studio::Bus* array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to buses")
            buses[path] << bus;
        }
    }
}

void FmodCache::_load_all_event_descriptions(BankData* bank) {
    FMOD::Studio::EventDescription* array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " added to eventDescriptions")
            eventDescriptions[path] << eventDescription;
        }
    }
}

void FmodCache::_unload_all_vca(BankData* bank) {
    FMOD::Studio::VCA* array[MAX_VCA_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getVCAList(array, MAX_VCA_COUNT, &size))) {
        CHECK_SIZE(MAX_VCA_COUNT, size, VCAs)
        for (int i = 0; i < size; i++) {
            auto vca = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(vca->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from VCAs")
            VCAs.erase(path);
        }
    }
}

void FmodCache::_unload_all_buses(BankData* bank) {
    FMOD::Studio::Bus* array[MAX_BUS_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getBusList(array, MAX_BUS_COUNT, &size))) {
        CHECK_SIZE(MAX_BUS_COUNT, size, buses)
        for (int i = 0; i < size; i++) {
            auto bus = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(bus->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from buses")
            buses.erase(path);
        }
    }
}

void FmodCache::_unload_all_event_descriptions(BankData* bank) {
    FMOD::Studio::EventDescription* array[MAX_EVENT_COUNT];
    int size = 0;
    if (ERROR_CHECK(bank->getEventList(array, MAX_EVENT_COUNT, &size))) {
        CHECK_SIZE(MAX_EVENT_COUNT, size, Events)
        for (int i = 0; i < size; i++) {
            auto eventDescription = array[i];
            char path[MAX_PATH_SIZE];
            ERROR_CHECK(eventDescription->getPath(path, MAX_PATH_SIZE, nullptr));
            GODOT_LOG(0, "FMOD Sound System: " + String(path) + " removed from eventDescriptions")
            eventDescriptions.erase(path);
        }
    }
}
void FmodCache::_unload_bank_data(BankData* bank) {
    _unload_all_buses(bank);
    _unload_all_vca(bank);
    _unload_all_event_descriptions(bank);
    ERROR_CHECK(bank->bank.unload());
    banks.erase(bank->godotPath);
    GODOT_LOG(0, "FMOD Sound System: BANK " + String(bank->godotPath) + " UNLOADED")
}

bool FmodCache::check_vca_path(const String& vcaPath) {
    return VCAs.has(vcaPath);
}

bool FmodCache::check_bus_path(const String& busPath) {
    return buses.has(busPath);
}

bool FmodCache::check_event_path(const String& eventPath) {
    return eventDescriptions.has(eventPath);
}
void FmodCache::add_bank(const String& bankPath, Ref<FmodBank> bank) {
    auto* loadingBank = new LoadingBank();
    loadingBank->bank = bank;
    loadingBank->godotPath = bankPath;
    loadingBanks.push_back(loadingBank);
}
void FmodCache::remove_bank(const String& bankPath) {
    if (banks.has(bankPath)) {
        _unload_bank_data(banks[bankPath]);
    }
}

void FmodCache::force_loading() {
    while(is_loading()){
        load_pending();
    }
}

bool FmodCache::is_loading() {
    return loadingBanks.size() > 0;
}

bool FmodCache::has_bank(const String& bankPath) {
    return banks.has(bankPath);
}

bool FmodCache::get_bank(const String& bankPath) {
    return banks[bankPath];
}

void FmodCache::clear() {
    force_loading();
    for(auto entry: banks){
        entry.value->bank.un
    }
    banks.clear()
}


Array get_all_vca() {}
Array get_all_buses() {}
Array get_all_event_descriptions() {}

Array get_all_banks() {}
