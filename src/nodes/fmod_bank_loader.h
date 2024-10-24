#ifndef GODOTFMOD_FMOD_BANK_LOADER_H
#define GODOTFMOD_FMOD_BANK_LOADER_H

#include "classes/node.hpp"
#include "studio/fmod_bank.h"

namespace godot {
    class FmodBankLoader : public Node {
        GDCLASS(FmodBankLoader, Node)

    public:
        virtual void _enter_tree() override;

        void set_bank_paths(const Array& p_paths);
        const Array& get_bank_paths() const;

    private:
        Vector<Ref<FmodBank>> bank;
        Array bank_paths;

    public:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_BANK_LOADER_H
