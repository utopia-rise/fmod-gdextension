#ifndef GODOTFMOD_FMOD_OBJECT_H
#define GODOTFMOD_FMOD_OBJECT_H

#include "classes/ref_counted.hpp"
#include "common.h"

namespace godot {

    template<class Derived, class T>
    class FmodObject : public RefCounted {
        GDCLASS(Derived, RefCounted);

    protected:
        T* _wrapped = nullptr;

    public:
        inline static Ref<Derived> create_ref(T* wrapped) {
            Ref<Derived> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
            }
            return ref;
        }

        bool is_valid() const {
            return _wrapped != nullptr && _wrapped->isValid();
        }

        T* get_wrapped() const {
            return _wrapped;
        }

    protected:
        static void _bind_methods() {}
    };

    template<class Derived, class T>
    class FmodObjectWithPath : public FmodObject<Derived, T> {
    protected:
        String _path;

    public:
        inline static Ref<Derived> create_ref(T* wrapped) {
            Ref<Derived> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
                char path[MAX_PATH_SIZE];
                ERROR_CHECK(wrapped->getPath(path, MAX_PATH_SIZE, nullptr));
                ref->_path = String(path);
            }
            return ref;
        }

        String get_path() {
            return _path;
        }
    };

}// namespace godot

#endif// GODOTFMOD_FMOD_OBJECT_H
