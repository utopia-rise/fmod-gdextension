#ifndef GODOTFMOD_FMOD_CACHE_H
#define GODOTFMOD_FMOD_CACHE_H

#include "core/fmod_file.h"
#include "fmod_studio.hpp"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"

namespace godot {
    class FmodServer;

    class FmodCache {
        friend class FmodServer;

        FMOD::Studio::System* system;

        List<Ref<FmodBank>> loadingBanks;

        HashMap<String, Ref<FmodFile>> files;
        HashMap<String, Ref<FmodBank>> banks;

        HashMap<String, Ref<FmodEventDescription>> eventDescriptions;
        HashMap<String, Ref<FmodBus>> buses;
        HashMap<String, Ref<FmodVCA>> vcas;

        void _get_bank_data(Ref<FmodBank> bank);
        void _remove_bank_data(Ref<FmodBank> bank);

    public:
        FmodCache() = delete;
        FmodCache(const FmodCache& other) = delete;
        explicit FmodCache(FMOD::Studio::System* p_system);

        ~FmodCache();

        Ref<FmodBank> add_bank(const String& bankPath, unsigned int flag);
        bool has_bank(const String& bankPath);
        Ref<FmodBank> get_bank(const String& bankPath);
        void remove_bank(const String& bankPath);

        Ref<FmodFile> add_file(const String& filePath, unsigned int flag);
        bool has_file(const String& filePath);
        Ref<FmodFile> get_file(const String& filePath);
        void remove_file(const String& filePath);

        bool is_master_loaded();
        void clear();

        void update_pending();
        void force_loading();
        bool is_loading();

        bool has_vca_path(const String& vcaPath);
        bool has_bus_path(const String& busPath);
        bool has_event_path(const String& eventPath);

        Ref<FmodVCA> get_vca(const String& vcaPath);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event(const String& eventPath);
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_CACHE_H
