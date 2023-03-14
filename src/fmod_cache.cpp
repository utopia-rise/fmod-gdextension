
#include "fmod_cache.h"
#include "helpers/common.h"

using namespace godot;

bool FmodCache::banks_still_loading() {
    for (KeyValue<FmodBank, BankData> entry : banks) {
        FMOD_STUDIO_LOADING_STATE loadingState;
        ERROR_CHECK(entry.key->getLoadingState(&loadingState));
        if (loadingState == FMOD_STUDIO_LOADING_STATE_LOADING) {
            return true;
        }
    }
    return false;
}

void FmodCache::_check_loading_banks() {
    for (auto loadingBank : loadingBanks) {
        auto bank = loadingBank->bank;
        FMOD_STUDIO_LOADING_STATE* loading_state = nullptr;
        ERROR_CHECK(bank->getLoadingState(loading_state));
        if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _load_bank_data(loadingBank);
            delete loadingBank;
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_LOADING) {
            loadingBanks.push_back(loadingBank);
        } else if (*loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            bank->unload();
            delete loadingBank;
            GODOT_LOG(2, "Fmod Sound System: Error loading bank.")
        }
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

void FmodCache::_load_all_vca(Ref<FmodBank> bank) {
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

void FmodCache::_load_all_buses(Ref<FmodBank> bank) {
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

void FmodCache::_load_all_event_descriptions(Ref<FmodBank> bank) {
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

void FmodCache::_unload_all_vca(Ref<FmodBank> bank) {
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

void FmodCache::_unload_all_buses(Ref<FmodBank> bank) {
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

void FmodCache::_unload_all_event_descriptions(Ref<FmodBank> bank) {
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
    return VCAs.has(vcaPath)
}

bool FmodCache::check_bus_path(const String& busPath) {
    return buses.has(busPath)
}

bool FmodCache::check_event_path(const String& eventPath) {
    return eventDescriptions.has(eventPath)
}