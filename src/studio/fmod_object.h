#ifndef GODOTFMOD_FMOD_OBJECT_H
#define GODOTFMOD_FMOD_OBJECT_H

#include <helpers/common.h>
#include <classes/ref.hpp>

namespace godot {

    template<class Derived, class Owned>
    class FmodObject : public RefCounted {
        GDCLASS(Derived, RefCounted);

    protected:
        Owned* _wrapped = nullptr;

    public:
        inline static Ref<Derived> create_ref(Owned* wrapped) {
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

        Owned* get_wrapped() const {
            return _wrapped;
        }

    protected:
        static void _bind_methods() {}
    };

    template<class Derived, class Owned>
    class FmodObjectWithPath : public FmodObject<Derived, Owned> {
    protected:
        String _path;

    public:
        inline static Ref<Derived> create_ref(Owned* wrapped) {
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
