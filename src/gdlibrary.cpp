#include "godot_fmod.h"

using namespace godot;

extern "C" void GDN_EXPORT fmod_gdnative_init(godot_gdnative_init_options *o){
    Godot::gdnative_init(o);
    //Just to initialize the singleton;
    Callbacks::GodotFileRunner::get_singleton();
}

extern "C" void GDN_EXPORT fmod_gdnative_terminate(godot_gdnative_terminate_options *o){
    Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT fmod_nativescript_init(void *handle){
    Godot::nativescript_init(handle);
    register_class<Fmod>();
}

