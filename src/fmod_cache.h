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

    class FmodCache {

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

            void load_bank_data();
            void unload_bank_data();

            void load_all_vca();
            void load_all_buses();
            void load_all_event_descriptions();

            void unload_all_vca();
            void unload_all_buses();
            void unload_all_event_descriptions();
        };

        List<LoadingBank*> loadingBanks;

        HashMap<String, BankData*> banks;
        HashMap<String, Ref<FmodEventDescription>> eventDescriptions;
        HashMap<String, Ref<FmodBus>> buses;
        HashMap<String, Ref<FmodVCA>> VCAs;

    public:
        void add_bank(const String& bankPath, Ref<FmodBank> bank);
        bool has_bank(const String& bankPath);
        bool get_bank(const String& bankPath);
        void remove_bank(const String& bankPath);
        void clear();

        void update_pending();
        void force_loading();
        bool is_loading();

        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_CACHE_H
