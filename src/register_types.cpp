#include "register_types.h"

#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "godot_fmod.h"

using namespace godot;

static Fmod *_fmod_singleton;

void initialize_fmod_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		Callbacks::GodotFileRunner::get_singleton();
		ClassDB::register_class<Fmod>();
		_fmod_singleton = memnew(Fmod);
		Engine::get_singleton()->register_singleton("Fmod", Fmod::get_singleton());
	}
}

void uninitialize_fmod_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		Callbacks::GodotFileRunner::get_singleton()->finish();
		Engine::get_singleton()->unregister_singleton("Fmod");
		memdelete(_fmod_singleton);
	}
}

extern "C" {

// Initialization.

GDNativeBool GDN_EXPORT fmod_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_fmod_module);
	init_obj.register_terminator(uninitialize_fmod_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
