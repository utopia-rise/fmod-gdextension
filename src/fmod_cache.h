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

    struct FmodGuidHashMapHasher {
        static _FORCE_INLINE_ uint32_t hash(const FMOD_GUID& guid) {
            return guid.Data1;
        }
    };

    struct FmodGuidHashMapComparer {
        static bool compare(const FMOD_GUID& p_lhs, const FMOD_GUID& p_rhs) {
            uint64_t* left_guid {reinterpret_cast<uint64_t*>(const_cast<FMOD_GUID*>(&p_lhs))};
            uint64_t* right_guid {reinterpret_cast<uint64_t*>(const_cast<FMOD_GUID*>(&p_rhs))};
            return left_guid[0] == right_guid[0] && left_guid[1] == right_guid[1];
        }
    };

    class FmodCache {
        friend class FmodServer;

        FMOD::Studio::System* system;

        List<Ref<FmodBank>> loading_banks;

        HashMap<String, Ref<FmodFile>> files;
        HashMap<String, FmodBank*> banks;

        HashMap<FMOD_GUID, Ref<FmodEventDescription>, FmodGuidHashMapHasher, FmodGuidHashMapComparer> event_descriptions;
        HashMap<FMOD_GUID, Ref<FmodBus>, FmodGuidHashMapHasher, FmodGuidHashMapComparer> buses;
        HashMap<FMOD_GUID, Ref<FmodVCA>, FmodGuidHashMapHasher, FmodGuidHashMapComparer> vcas;

        HashMap<String, FMOD_GUID> strings_to_guid;

        void _get_bank_data(Ref<FmodBank> bank);
        void _remove_bank_data(FmodBank* bank);

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

        bool has_vca_guid(const FMOD_GUID& guid);
        bool has_vca_path(const String& vcaPath);
        bool has_bus_guid(const FMOD_GUID& guid);
        bool has_bus_path(const String& busPath);
        bool has_event_guid(const FMOD_GUID& guid);
        bool has_event_path(const String& eventPath);

        Ref<FmodVCA> get_vca(const FMOD_GUID& guid);
        Ref<FmodVCA> get_vca(const String& vcaPath);
        Ref<FmodBus> get_bus(const FMOD_GUID& guid);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event(const FMOD_GUID& guid);
        Ref<FmodEventDescription> get_event(const String& eventPath);
        FMOD_GUID get_event_guid(const String& event_path);
        String get_event_path(const FMOD_GUID& guid);
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_CACHE_H
