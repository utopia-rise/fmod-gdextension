#include "register_types.h"
#include "core/class_db.h"

#include "godot_fmod.h"

void register_fmod_types() {
	ClassDB::register_class<Fmod>();
}

void unregister_fmod_types() {
	// do nothing
}