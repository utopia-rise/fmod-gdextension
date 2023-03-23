#ifndef GODOTFMOD_FMOD_FILE_H
#define GODOTFMOD_FMOD_FILE_H

#include "classes/ref_counted.hpp"
#include "fmod.hpp"

namespace godot {
    class FmodFile : public RefCounted {
        GDCLASS(FmodFile, RefCounted);

        FMOD::Sound* _wrapped = nullptr;

    public:
        inline static Ref<FmodFile> create_ref(FMOD::Sound* wrapped) {
            Ref<FmodFile> ref;
            if (wrapped) {
                ref.instantiate();
                ref->_wrapped = wrapped;
                wrapped->setUserData(ref.ptr());
            }
            return ref;
        }


        FMOD::Sound* get_wrapped() const{
            return _wrapped;
        }

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_FILE_H
