#ifndef GODOTFMOD_FMOD_CACHE_H
#define GODOTFMOD_FMOD_CACHE_H

#include "classes/object.hpp"
#include "core/fmod_file.h"
#include "fmod_studio.hpp"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"

namespace godot {

    class FmodCache {
        List<Ref<FmodBank>> loadingBanks;

        HashMap<String, Ref<FmodFile>> file;
        HashMap<String, Ref<FmodBank>> banks;
        HashMap<String, Ref<FmodEventDescription>> eventDescriptions;
        HashMap<String, Ref<FmodBus>> buses;
        HashMap<String, Ref<FmodVCA>> VCAs;

    public:
        void add_bank(const String& bankPath, Ref<FmodBank> bank);
        bool has_bank(const String& bankPath);
        Ref<FmodBank> get_bank(const String& bankPath);
        void remove_bank(const String& bankPath);

        void add_file(const String& filePath, Ref<FmodFile> bank);
        bool has_file(const String& filePath);
        Ref<FmodFile> get_file(const String& filePath);
        void remove_file(const String& filePath);

        bool is_master_loaded();
        void refresh();
        void clear();

        void update_pending();
        void force_loading();
        bool is_loading();

        void add_VCA(FMOD::Studio::VCA* vca);
        void add_bus(FMOD::Studio::Bus* bus);
        void add_event(FMOD::Studio::EventDescription* event);

        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);

        Ref<FmodVCA> get_VCA(const String& vcaPath);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event(const String& eventPath);
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_CACHE_H
