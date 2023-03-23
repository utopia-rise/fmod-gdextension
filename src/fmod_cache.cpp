
#include "fmod_cache.h"
#include "helpers/common.h"

using namespace godot;

FmodCache::FmodCache(FMOD::Studio::System* p_system) {
    system = p_system;
}

FmodCache::~FmodCache() {
    system = nullptr;
}

void FmodCache::update_pending() {
    if (loadingBanks.size() == 0) {
        return;
    }
    List<Ref<FmodBank>> toDelete;
    for (Ref<FmodBank> loadingBank : loadingBanks) {
        int loading_state = loadingBank->get_loading_state();
        if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _get_bank_data(loadingBank);
            banks[loadingBank->get_path()] = loadingBank;
            toDelete.push_back(loadingBank);
        } else if (loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            toDelete.push_back(loadingBank);
            GODOT_LOG(2, "Fmod Sound System: Error loading bank.")
        }
    }
    for (const Ref<FmodBank>& element : toDelete) {
        loadingBanks.erase(element);
    }
}

void FmodCache::force_loading() {
    while (is_loading()) {
        update_pending();
    }
}

bool FmodCache::is_loading() {
    return loadingBanks.size() > 0;
}

Ref<FmodBank> FmodCache::add_bank(const String& bankPath, unsigned int flag) {
    FMOD::Studio::Bank* bank = nullptr;
    ERROR_CHECK(system->loadBankFile(bankPath.utf8().get_data(), flag, &bank));
    if (!bank) {
        return {};
    }
    Ref<FmodBank> ref = FmodBank::create_ref(bank);
    GODOT_LOG(0, "FMOD Sound System: LOADING BANK " + String(bankPath))
    loadingBanks.push_back(ref);
    if (flag != FMOD_STUDIO_LOAD_BANK_NONBLOCKING) {
        force_loading();
    }
    return ref;
}

void FmodCache::remove_bank(const String& bankPath) {
    if (banks.has(bankPath)) {
        Ref<FmodBank> bank = banks[bankPath];
        _remove_bank_data(bank);
        ERROR_CHECK(bank->get_wrapped()->unload());
        banks.erase(bankPath);
    }
}

bool FmodCache::has_bank(const String& bankPath) {
    return banks.has(bankPath);
}

Ref<FmodBank> FmodCache::get_bank(const String& bankPath) {
    return banks.get(bankPath);
}

Ref<FmodFile> FmodCache::add_file(const String& filePath, unsigned int flag) {
    FMOD::System* core = nullptr;
    ERROR_CHECK(system->getCoreSystem(&core));

    FMOD::Sound* sound = nullptr;
    ERROR_CHECK(core->createSound(filePath.utf8().get_data(), flag, nullptr, &sound));
    if (sound) {
        Ref<FmodFile> ref = FmodFile::create_ref(sound);
        files[filePath] = ref;
        GODOT_LOG(0, "FMOD Sound System: LOADING AS SOUND FILE" + String(filePath))
        return ref;
    }
    return {};
}

bool FmodCache::has_file(const String& filePath) {
    return files.find(filePath).operator bool();
}

Ref<FmodFile> FmodCache::get_file(const String& filePath) {
    return files.get(filePath);
}

void FmodCache::remove_file(const String& filePath) {
    if (files.has(filePath)) {
        Ref<FmodFile> ref = files[filePath];
        ERROR_CHECK(ref->get_wrapped()->release());
        files.erase(filePath);
    }
}

bool FmodCache::has_vca_path(const String& vcaPath) {
    return vcas.has(vcaPath);
}

bool FmodCache::has_bus_path(const String& busPath) {
    return buses.has(busPath);
}

bool FmodCache::has_event_path(const String& eventPath) {
    return eventDescriptions.has(eventPath);
}

Ref<FmodVCA> FmodCache::get_vca(const String& vcaPath) {
    return vcas.get(vcaPath);
}

Ref<FmodBus> FmodCache::get_bus(const String& busPath) {
    return buses.get(busPath);
}

Ref<FmodEventDescription> FmodCache::get_event(const String& eventPath) {
    return eventDescriptions.get(eventPath);
}

bool FmodCache::is_master_loaded() {
    return banks.size() > 0;
}

void FmodCache::clear() {
    force_loading();
    eventDescriptions.clear();
    buses.clear();
    vcas.clear();
    banks.clear();
}

void FmodCache::_get_bank_data(Ref<FmodBank> bank) {
    bank->update_bank_data();
    for (Ref<FmodBus> bus : bank->getBuses()) {
        buses[bus->get_path()] = bus;
    }
    for (Ref<FmodVCA> vca : bank->getVcAs()) {
        vcas[vca->get_path()] = vca;
    }
    for (Ref<FmodEventDescription> desc : bank->getEventDescriptions()) {
        eventDescriptions[desc->get_path()] = desc;
    }
}

void FmodCache::_remove_bank_data(Ref<FmodBank> bank) {
    for (Ref<FmodBus> bus : bank->getBuses()) {
        buses.erase(bus->get_path());
    }
    for (Ref<FmodVCA> vca : bank->getVcAs()) {
        vcas.erase(vca->get_path());
    }
    for (Ref<FmodEventDescription> desc : bank->getEventDescriptions()) {
        eventDescriptions.erase(desc->get_path());
    }
}