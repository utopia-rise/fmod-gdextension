#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "nodes/fmod_bank_loader.h"
#include "nodes/fmod_event_emitter_3d.h"
#include "nodes/fmod_listener_2d.h"
#include "nodes/fmod_listener_3d.h"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include <classes/engine.hpp>
#include <fmod_server.h>
#include <nodes/fmod_event_emitter_2d.h>
#include <register_types.h>

using namespace godot;

static FmodServer* fmod_singleton;

void initialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        //Data
        ClassDB::register_class<FmodPerformanceData>();

        // Core
        ClassDB::register_class<FmodSound>();

        // Studio
        ClassDB::register_class<FmodBank>();
        ClassDB::register_class<FmodEvent>();
        ClassDB::register_class<FmodEventDescription>();
        ClassDB::register_class<FmodBus>();
        ClassDB::register_class<FmodVCA>();

        // Nodes
        ClassDB::register_class<FmodListener3D>();
        ClassDB::register_class<FmodListener2D>();
        ClassDB::register_class<FmodEventEmitter2D>();
        ClassDB::register_class<FmodEventEmitter3D>();
        ClassDB::register_class<FmodBankLoader>();

        // Server
        ClassDB::register_class<FmodServer>();
        fmod_singleton = memnew(FmodServer);
        Engine::get_singleton()->register_singleton("Fmod", FmodServer::get_singleton());
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
        // initialise filerunner singleton by calling it.
        Callbacks::GodotFileRunner::get_singleton();
    }
}

void uninitialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        Engine::get_singleton()->unregister_singleton("Fmod");
        memdelete(fmod_singleton);
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
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

    return init_obj.init();
}
}
