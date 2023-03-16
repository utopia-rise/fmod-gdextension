#ifndef GODOTFMOD_FMOD_OBJECT_H
#define GODOTFMOD_FMOD_OBJECT_H

#include "common.h"
#include "classes/ref_counted.hpp"

namespace godot{

    template<class Derived, class T>
    class FmodObject: public RefCounted {
        GDCLASS(Derived, RefCounted);

    protected:
        T* wrapped = nullptr;

    public:
        static Ref<Derived> create_ref(T* wrapped);
        T* get_wrapped();
        bool is_valid();

    protected:
        static void _bind_methods();
    };

    template<class Derived, class T>
    Ref<Derived> FmodObject<Derived, T>::create_ref(T* wrapped) {
        Ref<Derived> ref;
        ref.instantiate();
        ref->wrapped = wrapped;
        wrapped->setUserData(ref.ptr());
        return ref;
    }

    template<class Derived, class T>
    bool FmodObject<Derived, T>::is_valid() {
        return wrapped != nullptr && wrapped->isValid();
    }

    template<class Derived, class T>
    T* FmodObject<Derived, T>::get_wrapped(){
        return wrapped;
    }
}


#endif// GODOTFMOD_FMOD_OBJECT_H
