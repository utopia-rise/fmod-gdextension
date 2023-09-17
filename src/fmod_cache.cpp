
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
    if (loading_banks.size() == 0) { return; }
    List<Ref<FmodBank>> toDelete;
    for (Ref<FmodBank> loadingBank : loading_banks) {
        int loading_state = loadingBank->get_loading_state();
        if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _get_bank_data(loadingBank);
            banks[loadingBank->get_godot_res_path()] = loadingBank;
            toDelete.push_back(loadingBank);
        } else if (loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            toDelete.push_back(loadingBank);
            GODOT_LOG_ERROR("Fmod Sound System: Error loading bank.")
        }
    }
    for (const Ref<FmodBank>& element : toDelete) {
        loading_banks.erase(element);
    }
}

void FmodCache::force_loading() {
    while (is_loading()) {
        update_pending();
    }
}

bool FmodCache::is_loading() {
    return loading_banks.size() > 0;
}

Ref<FmodBank> FmodCache::add_bank(const String& bankPath, unsigned int flag) {
    FMOD::Studio::Bank* bank = nullptr;
    ERROR_CHECK(system->loadBankFile(bankPath.utf8().get_data(), flag, &bank));
    if (!bank) { return {}; }
    Ref<FmodBank> ref = FmodBank::create_ref(bank, bankPath);
    GODOT_LOG_INFO("FMOD Sound System: LOADING BANK " + String(bankPath))
    loading_banks.push_back(ref);
    if (flag != FMOD_STUDIO_LOAD_BANK_NONBLOCKING) { force_loading(); }
    return ref;
}

void FmodCache::remove_bank(const String& bankPath) {
    if (!banks.has(bankPath)) {
        GODOT_LOG_ERROR(vformat("Cannot unload bank with path %s, not in cache.", bankPath));
        return;
    }
    Ref<FmodBank> bank = banks[bankPath];
    _remove_bank_data(bank);
    ERROR_CHECK(bank->get_wrapped()->unload());
    banks.erase(bankPath);
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
        GODOT_LOG_INFO("FMOD Sound System: LOADING AS SOUND FILE" + String(filePath))
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

bool FmodCache::has_vca_guid(const FMOD_GUID& guid) {
    return vcas.has(guid);
}

bool FmodCache::has_vca_path(const String& vcaPath) {
    return strings_to_guid.has(vcaPath) && vcas.has(strings_to_guid.get(vcaPath));
}

bool FmodCache::has_bus_guid(const FMOD_GUID& guid) {
    return buses.has(guid);
}

bool FmodCache::has_bus_path(const String& busPath) {
    return strings_to_guid.has(busPath) && buses.has(strings_to_guid.get(busPath));
}

bool FmodCache::has_event_guid(const FMOD_GUID& guid) {
    return event_descriptions.has(guid);
}

bool FmodCache::has_event_path(const String& eventPath) {
    return strings_to_guid.has(eventPath) && event_descriptions.has(strings_to_guid.get(eventPath));
}

Ref<FmodVCA> FmodCache::get_vca(const FMOD_GUID& guid) {
    return vcas.get(guid);
}

Ref<FmodVCA> FmodCache::get_vca(const String& vcaPath) {
    return vcas.get(strings_to_guid.get(vcaPath));
}

Ref<FmodBus> FmodCache::get_bus(const FMOD_GUID& guid) {
    return buses.get(guid);
}

Ref<FmodBus> FmodCache::get_bus(const String& busPath) {
    return buses.get(strings_to_guid.get(busPath));
}

Ref<FmodEventDescription> FmodCache::get_event(const FMOD_GUID& guid) {
    return event_descriptions.get(guid);
}

Ref<FmodEventDescription> FmodCache::get_event(const String& eventPath) {
    return event_descriptions.get(strings_to_guid.get(eventPath));
}

bool FmodCache::is_master_loaded() {
    return banks.size() > 0;
}

void FmodCache::clear() {
    force_loading();
    event_descriptions.clear();
    buses.clear();
    vcas.clear();
    banks.clear();
}

void FmodCache::_get_bank_data(Ref<FmodBank> bank) {
    bank->update_bank_data();
    for (Ref<FmodBus> bus : bank->getBuses()) {
        FMOD_GUID guid {bus->get_guid()};
        buses[guid] = bus;
        strings_to_guid[bus->get_path()] = guid;
    }
    for (Ref<FmodVCA> vca : bank->getVcAs()) {
        FMOD_GUID guid {vca->get_guid()};
        vcas[guid] = vca;
        strings_to_guid[vca->get_path()] = guid;
    }
    for (Ref<FmodEventDescription> desc : bank->getEventDescriptions()) {
        FMOD_GUID guid {desc->get_guid()};
        event_descriptions[guid] = desc;
        strings_to_guid[desc->get_path()] = guid;
    }
}

void FmodCache::_remove_bank_data(Ref<FmodBank> bank) {
    for (Ref<FmodBus> bus : bank->getBuses()) {
        strings_to_guid.erase(bus->get_path());
        buses.erase(bus->get_guid());
    }
    for (Ref<FmodVCA> vca : bank->getVcAs()) {
        strings_to_guid.erase(vca->get_path());
        vcas.erase(vca->get_guid());
    }
    for (Ref<FmodEventDescription> desc : bank->getEventDescriptions()) {
        strings_to_guid.erase(desc->get_path());
        event_descriptions.erase(desc->get_guid());
    }
}