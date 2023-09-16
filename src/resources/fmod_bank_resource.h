#ifndef GODOTFMOD_FMOD_BANK_RESOURCE_H
#define GODOTFMOD_FMOD_BANK_RESOURCE_H

#include <classes/resource.hpp>
namespace godot {
    class FmodBankResource : public Resource {
        GDCLASS(FmodBankResource, Resource)

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_BANK_RESOURCE_H
