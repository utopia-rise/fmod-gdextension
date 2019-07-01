#include "godot_fmod.h"

using namespace godot;

Mutex *Callbacks::mut;

extern "C" void GDN_EXPORT fmod_gdnative_init(godot_gdnative_init_options *o){
    Callbacks::mut = new Mutex();
    Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT fmod_gdnative_terminate(godot_gdnative_terminate_options *o){
    delete Callbacks::mut;
    Callbacks::mut = nullptr;
    Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT fmod_nativescript_init(void *handle){
    Godot::nativescript_init(handle);
    register_class<GodotFmod>();
}

