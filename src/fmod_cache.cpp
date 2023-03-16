
#include "fmod_cache.h"
#include "helpers/common.h"

using namespace godot;

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
    loadingBanks.push_back(bank);
}
void FmodCache::remove_bank(const String& bankPath) {
    if (banks.has(bankPath)) {
        Ref<FmodBank> bank = banks[bankPath];
        _remove_bank_objects(bank);
        banks.erase(bankPath);
    }
}

void FmodCache::update_pending() {
    if (loadingBanks.size() == 0) {
        return;
    }
    List<Ref<FmodBank>> toDelete;
    for (auto loadingBank : loadingBanks) {
        Ref<FmodBank> bank = loadingBank;
        int loading_state = bank->get_loading_state();
        if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            loadingBank->update_bank_data();
            _cache_bank_objects(loadingBank);
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

bool FmodCache::has_bank(const String& bankPath) {
    return banks.has(bankPath);
}

Ref<FmodBank> FmodCache::get_bank(const String& bankPath) {
    return banks[bankPath];
}

void FmodCache::clear() {
    force_loading();
    eventDescriptions.clear();
    buses.clear();
    VCAs.clear();
    banks.clear();
}
