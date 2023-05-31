#ifndef GODOTFMOD_FMOD_BANK_LOADER_H
#define GODOTFMOD_FMOD_BANK_LOADER_H

#include "classes/node.hpp"
#include "studio/fmod_bank.h"

namespace godot {
    class FmodBankLoader : public Node {
        GDCLASS(FmodBankLoader, Node)

        Ref<FmodBank> bank;
        String path;

    public:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_BANK_LOADER_H
