#ifndef GODOTFMOD_FMOD_CACHE_H
#define GODOTFMOD_FMOD_CACHE_H

#include "classes/object.hpp"
#include "fmod_studio.hpp"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"

namespace godot {

    struct LoadingBank {
        Ref<FmodBank> bank;
        String godotPath;
    };

    struct BankData {
        BankData() = default;
        BankData(BankData const &other) = default;

        Ref<FmodBank> bank;
        String godotPath;

        List<String> eventDescriptionPaths;
        List<String> busesPaths;
        List<String> VCAsPaths;
    };

    class FmodCache {
        friend class FmodServer;

        List<LoadingBank*> loadingBanks;

        HashMap<String, BankData*> banks;
        HashMap<String, Ref<FmodEventDescription>> eventDescriptions;
        HashMap<String, Ref<FmodBus>> buses;
        HashMap<String, Ref<FmodVCA>> VCAs;

        void _load_bank_data(LoadingBank* loadingBank);
        void _unload_bank_data(BankData* bank);

        void _load_all_vca(BankData* bank);
        void _load_all_buses(BankData* bank);
        void _load_all_event_descriptions(BankData* bank);

        void _unload_all_vca(BankData* bank);
        void _unload_all_buses(BankData* bank);
        void _unload_all_event_descriptions(BankData* bank);

    public:
        void add_bank();
        void remove_bank();
        void clear();

        bool banks_still_loading();
        void _check_loading_banks();

        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);

        Array get_all_vca();
        Array get_all_buses();
        Array get_all_event_descriptions();
        Array get_all_banks();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_CACHE_H
