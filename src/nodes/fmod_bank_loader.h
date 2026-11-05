#ifndef GODOTFMOD_FMOD_BANK_LOADER_H
#define GODOTFMOD_FMOD_BANK_LOADER_H

#include "classes/node.hpp"
#include "studio/fmod_bank.h"

namespace godot {
    class FmodBankLoader : public Node {
        GDCLASS(FmodBankLoader, Node)

    public:
        virtual void _enter_tree() override;
        virtual void _exit_tree() override;

        void set_bank_paths(const Array& p_paths);
        const Array& get_bank_paths() const;

        void set_preload_sample_data(bool p_preload_sample_data);
        bool is_preload_sample_data() const;

        bool _property_can_revert(const StringName& p_property) const;
        bool _property_get_revert(const StringName& p_property, Variant& result) const;

    private:
        Vector<Ref<FmodBank>> banks;
        Array bank_paths;
        bool preload_sample_data = false;

        void load_banks(Vector<Ref<FmodBank>>& p_banks);
        void unload_banks(Vector<Ref<FmodBank>>& p_banks);

    public:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_BANK_LOADER_H
