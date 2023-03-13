#ifndef GODOTFMOD_FMOD_CACHE_H
#define GODOTFMOD_FMOD_CACHE_H

#include "classes/object.hpp"
#include "fmod_studio.hpp"
#include "studio/fmod_bank.h"
#include "templates/hash_map.hpp"

namespace godot{
    class FmodCache {

        struct LoadingBank {
            FMOD::Studio::Bank* bank;
            String godotPath;
        };

        struct BankData{
            List<String, FMOD::Studio::EventDescription*> eventDescriptions;
            List<String, FMOD::Studio::Bus*> buses;
            List<String, FMOD::Studio::VCA*> VCAs;
        };

        List<LoadingBank*> loadingBanks;
        HashMap<FmodBank, BankData> banks;

        void _load_bank_data(LoadingBank* loadingBank);

        void _load_all_vca(FMOD::Studio::Bank* bank);
        void _load_all_buses(FMOD::Studio::Bank* bank);
        void _load_all_event_descriptions(FMOD::Studio::Bank* bank);

        void _unload_all_vca(FMOD::Studio::Bank* bank);
        void _unload_all_buses(FMOD::Studio::Bank* bank);
        void _unload_all_event_descriptions(FMOD::Studio::Bank* bank);

    public:
        void add_bank();
        void remove_bank();
        void clear();

        bool banks_still_loading();
        void _check_loading_banks();

        Array get_all_vca();
        Array get_all_buses();
        Array get_all_event_descriptions();
        Array get_all_banks();
    };
}


#endif// GODOTFMOD_FMOD_CACHE_H
