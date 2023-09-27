#ifndef GODOTFMOD_FMOD_PARAMETER_IDENTIFIER_H
#define GODOTFMOD_FMOD_PARAMETER_IDENTIFIER_H

#include <classes/resource.hpp>

namespace godot {
    class FmodParameterIdentifier : public Resource {
        GDCLASS(FmodParameterIdentifier, Resource)

        uint64_t id;
        String name;
        bool load_by_name;

    public:
        void set_parameter_id(const uint64_t p_id);
        uint64_t get_parameter_id() const;

        void set_parameter_name(const String& p_name);
        const String& get_parameter_name() const;

        void set_load_by_name(const bool p_load_by_name);
        bool get_load_by_name() const;

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_PARAMETER_IDENTIFIER_H
