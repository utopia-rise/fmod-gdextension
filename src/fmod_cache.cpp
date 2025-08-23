
#include "fmod_cache.h"

#include "helpers/common.h"
#include "classes/project_settings.hpp"

using namespace godot;

FmodCache::FmodCache(FMOD::Studio::System* p_system, FMOD::System* p_core_system) :
    system(p_system), core_system(p_core_system) {

}

FmodCache::~FmodCache() {
    system = nullptr;
    core_system = nullptr;
}

void FmodCache::update_pending() {
    if (loading_banks.size() == 0) { return; }
    List<Ref<FmodBank>> to_delete;
    for (const Ref<FmodBank>& loading_bank : loading_banks) {
        int loading_state = loading_bank->get_loading_state();
        if (loading_state == FMOD_STUDIO_LOADING_STATE_LOADED) {
            _get_bank_data(loading_bank);
            banks[loading_bank->get_godot_res_path()] = loading_bank.ptr();
            to_delete.push_back(loading_bank);
        } else if (loading_state == FMOD_STUDIO_LOADING_STATE_ERROR) {
            to_delete.push_back(loading_bank);
            GODOT_LOG_ERROR("Fmod Sound System: Error loading bank.")
        }
    }
    for (const Ref<FmodBank>& element : to_delete) {
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

Ref<FmodBank> FmodCache::add_bank(const String& bank_path, unsigned int flag) {
    FMOD::Studio::Bank* bank = nullptr;
    ERROR_CHECK_WITH_REASON(system->loadBankFile(bank_path.utf8().get_data(), flag, &bank), vformat("Cannot load bank %s", bank_path));
    if (!bank) { return {}; }
    Ref<FmodBank> ref = FmodBank::create_ref(bank, bank_path);
    GODOT_LOG_VERBOSE("FMOD Sound System: LOADING BANK " + String(bank_path))
    loading_banks.push_back(ref);
    if (flag != FMOD_STUDIO_LOAD_BANK_NONBLOCKING) { force_loading(); }
    return ref;
}

void FmodCache::remove_bank(const String& bank_path) {
    if (!banks.has(bank_path)) {
        GODOT_LOG_ERROR(vformat("Cannot unload bank with path %s, not in cache.", bank_path));
        return;
    }
    FmodBank* bank = banks[bank_path];
    _remove_bank_data(bank);
    ERROR_CHECK_WITH_REASON(bank->get_wrapped()->unload(), vformat("Cannot unload bank %s", bank_path));
    banks.erase(bank_path);
}

bool FmodCache::has_bank(const String& bankPath) {
    return banks.has(bankPath);
}

Ref<FmodBank> FmodCache::get_bank(const String& bankPath) {
    return banks.get(bankPath);
}

#ifndef IOS_ENABLED

uint32_t FmodCache::add_plugin(const String& p_plugin_path, uint32_t p_priority) {
    uint32_t handle;
#if defined(ANDROID_ENABLED) && !defined(TOOLS_ENABLED)
    const char* plugin_path = p_plugin_path.utf8().get_data();
#else
    const char* plugin_path = ProjectSettings::get_singleton()->globalize_path(p_plugin_path).utf8().get_data();
#endif
    ERROR_CHECK(core_system->loadPlugin(plugin_path, &handle, p_priority));
    plugin_handles.append(handle);
    return handle;
}

#else

void FmodCache::add_plugin(uint32_t p_plugin_handle) {
    plugin_handles.append(p_plugin_handle);
}

#endif

bool FmodCache::has_plugin(uint32_t p_plugin_handle) const {
    return plugin_handles.has(p_plugin_handle);
}

void FmodCache::remove_plugin(uint32_t p_plugin_handle) {
    if (!has_plugin(p_plugin_handle)) {
        GODOT_LOG_ERROR(vformat("Cannot unload plugin with handle %s, not in cache.", p_plugin_handle));
    }

    ERROR_CHECK(core_system->unloadPlugin(p_plugin_handle));
    plugin_handles.erase(p_plugin_handle);
}


Ref<FmodFile> FmodCache::add_file(const String& file_path, unsigned int flag) {
    FMOD::System* core = nullptr;
    ERROR_CHECK(system->getCoreSystem(&core));

    FMOD::Sound* sound = nullptr;
    ERROR_CHECK_WITH_REASON(core->createSound(file_path.utf8().get_data(), flag, nullptr, &sound), vformat("Cannot create sound %s", file_path));
    if (sound) {
        Ref<FmodFile> ref = FmodFile::create_ref(sound);
        files[file_path] = ref;
        GODOT_LOG_VERBOSE("FMOD Sound System: LOADING AS SOUND FILE" + String(file_path))
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
    if (
            HashMap<FMOD_GUID, Ref<FmodVCA>, FmodGuidHashMapHasher, FmodGuidHashMapComparer>::Iterator iterator{
                    vcas.find(guid)
            }
    ) {
        return iterator->value;
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find vca with guid: %s", fmod_guid_to_string(guid)));
#endif

    return {};
}

Ref<FmodVCA> FmodCache::get_vca(const String& vca_path) {
    if (HashMap<String, FMOD_GUID>::Iterator iterator {strings_to_guid.find(vca_path)}) {
        return get_vca(iterator->value);
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find vca with path: %s", vca_path));
#endif

    return {};
}

Ref<FmodBus> FmodCache::get_bus(const FMOD_GUID& guid) {
    if (
            HashMap<FMOD_GUID, Ref<FmodBus>, FmodGuidHashMapHasher, FmodGuidHashMapComparer>::Iterator iterator{
                    buses.find(guid)
            }
    ) {
        return iterator->value;
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find bus with guid: %s", fmod_guid_to_string(guid)));
#endif

    return {};
}

Ref<FmodBus> FmodCache::get_bus(const String& bus_path) {
    if (HashMap<String, FMOD_GUID>::Iterator iterator {strings_to_guid.find(bus_path)}) {
        return get_bus(iterator->value);
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find bus with path: %s", bus_path));
#endif

    return {};
}

Ref<FmodEventDescription> FmodCache::get_event(const FMOD_GUID& guid) {
    if (
      HashMap<FMOD_GUID, Ref<FmodEventDescription>, FmodGuidHashMapHasher, FmodGuidHashMapComparer>::Iterator iterator {
          event_descriptions.find(guid)
      }
    ) {
        return iterator->value;
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find event with guid: %s", fmod_guid_to_string(guid)));
#endif

    return {};
}

Ref<FmodEventDescription> FmodCache::get_event(const String& eventPath) {
    if (HashMap<String, FMOD_GUID>::Iterator iterator {strings_to_guid.find(eventPath)}) {
        return get_event(iterator->value);
    }

#ifdef DEBUG_ENABLED
    GODOT_LOG_WARNING(vformat("Cannot find event with path: %s", eventPath));
#endif

    return {};
}

FMOD_GUID FmodCache::get_event_guid(const String& event_path) {
    if (HashMap<String, FMOD_GUID>::Iterator iterator {strings_to_guid.find(event_path)}) {
        return iterator->value;
    }

    return {};
}

String FmodCache::get_event_path(const FMOD_GUID& guid) {
    if (
            HashMap<FMOD_GUID, Ref<FmodEventDescription>, FmodGuidHashMapHasher, FmodGuidHashMapComparer>::Iterator iterator{
                    event_descriptions.find(guid)
            }
            ) {
        return iterator->value->get_path();
    }

    return {};
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

void FmodCache::_remove_bank_data(FmodBank* bank) {
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