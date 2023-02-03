#include <classes/engine.hpp>
#include <godot_fmod.h>
#include <nodes/fmod_event_emitter_2d.h>
#include <register_types.h>

using namespace godot;

static Fmod* _fmod_singleton;

void initialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        // register all classes
        ClassDB::register_class<Fmod>();
        ClassDB::register_class<FmodEventEmitter2D>();

        // setup Fmod singleton
        _fmod_singleton = memnew(Fmod);
        Engine::get_singleton()->register_singleton("Fmod", Fmod::get_singleton());
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
        // initialise filerunner singleton by calling it.
        Callbacks::GodotFileRunner::get_singleton();
    }
}

void uninitialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        Engine::get_singleton()->unregister_singleton("Fmod");
        memdelete(_fmod_singleton);
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
        Callbacks::GodotFileRunner::get_singleton()->finish();
    }
}

extern "C" {

    // Initialization.

    GDExtensionBool GDE_EXPORT fmod_library_init(const GDExtensionInterface* p_interface,
                                            const GDExtensionClassLibraryPtr p_library,
                                            GDExtensionInitialization* r_initialization) {
        GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

        init_obj.register_initializer(initialize_fmod_module);
        init_obj.register_terminator(uninitialize_fmod_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
