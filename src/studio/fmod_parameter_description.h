#ifndef GODOTFMOD_FMOD_PARAMETER_DESCRIPTION_H
#define GODOTFMOD_FMOD_PARAMETER_DESCRIPTION_H

#include "fmod_studio_common.h"
#include <classes/ref_counted.hpp>

namespace godot {
    class FmodParameterDescription : public RefCounted {
        GDCLASS(FmodParameterDescription, RefCounted)

        String _name;
        FMOD_STUDIO_PARAMETER_DESCRIPTION _wrapped;

    public:
        const String& get_name() const;
        uint64_t get_id() const;
        float get_minimum() const;
        float get_maximum() const;
        float get_default_value() const;
        bool is_read_only() const;
        bool is_automatic() const;
        bool is_global() const;
        bool is_discrete() const;
        bool is_labeled() const;

        inline static Ref<FmodParameterDescription> create_ref(const FMOD_STUDIO_PARAMETER_DESCRIPTION& wrapped) {
            Ref<FmodParameterDescription> ref;
            ref.instantiate();
            ref->_wrapped = wrapped;
            ref->_name = wrapped.name;
            return ref;
        }

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_PARAMETER_DESCRIPTION_H
