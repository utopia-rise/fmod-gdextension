#include "constants.h"
#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "fmod_server.h"
#include "fmod_string_names.h"
#include "nodes/fmod_bank_loader.h"
#include "nodes/fmod_event_emitter_2d.h"
#include "nodes/fmod_event_emitter_3d.h"
#include "nodes/fmod_listener_2d.h"
#include "nodes/fmod_listener_3d.h"
#include "nodes/fmod_runtime.h"
#include "resources/fmod_plugins_settings.h"
#include "resources/fmod_settings.h"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"

#ifdef TOOLS_ENABLED
#include <tools//fmod_editor_export_plugin.h>
#endif

#include <register_types.h>
#include <studio/fmod_parameter_description.h>
#include <tools/fmod_editor_plugin.h>

#include <classes/engine.hpp>
#include <classes/project_settings.hpp>

using namespace godot;

static FmodServer* fmod_singleton;

void initialize_fmod() {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) {
        FmodServer::get_singleton()->init();
        return;
    }
#endif

    bool auto_initialize = ProjectSettings::get_singleton()->get_setting(
        vformat("%s/%s", FMOD_SETTINGS_BASE_PATH, FMOD_SETTING_AUTO_INITIALIZE),
        DEFAULT_AUTO_INITIALIZE
    );

    if (auto_initialize) { FmodServer::get_singleton()->init(); }
}

void initialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
        // initialise filerunner singleton by calling it.
        FmodStringNames::create();
#ifndef WEB_ENABLED
        Callbacks::GodotFileRunner::get_singleton();
#endif
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        // Data
        ClassDB::register_class<FmodPerformanceData>();

        // Core
        ClassDB::register_class<FmodSound>();
        ClassDB::register_class<FmodFile>();
        ClassDB::register_class<FmodChannelGroup>();

        // Studio
        ClassDB::register_class<FmodBank>();
        ClassDB::register_class<FmodEventDescription>();
        ClassDB::register_class<FmodParameterDescription>();

        ClassDB::register_class<FmodEvent>();
        ClassDB::register_class<FmodBus>();
        ClassDB::register_class<FmodVCA>();

        // Nodes
        ClassDB::register_class<FmodListener3D>();
        ClassDB::register_class<FmodListener2D>();
        ClassDB::register_class<FmodEventEmitter2D>();
        ClassDB::register_class<FmodEventEmitter3D>();
        ClassDB::register_class<FmodBankLoader>();
        ClassDB::register_internal_class<FmodRuntime>();

        // Resources
        ClassDB::register_class<FmodSettings>();
        ClassDB::register_class<FmodStaticPluginMethod>();
        ClassDB::register_class<FmodPluginsSettings>();

        // Server
        ClassDB::register_class<FmodServer>();
        fmod_singleton = memnew(FmodServer);
        Engine::get_singleton()->register_singleton("FmodServer", FmodServer::get_singleton());
        initialize_fmod();
    }
#ifdef TOOLS_ENABLED
    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        ClassDB::register_class<FmodEditorExportPlugin>();
        ClassDB::register_class<FmodEditorPlugin>();
        EditorPlugins::add_by_type<FmodEditorPlugin>();
    }
#endif
}

void uninitialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
#ifndef WEB_ENABLED
        Callbacks::GodotFileRunner::get_singleton()->finish();
#endif
        FmodStringNames::free();
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        fmod_singleton->shutdown();
        FmodSettings::free_project_settings();

        Engine::get_singleton()->unregister_singleton("FmodServer");
        memdelete(fmod_singleton);
    }
}

extern "C" {

// Initialization.

GDExtensionBool GDE_EXPORT
fmod_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_fmod_module);
    init_obj.register_terminator(uninitialize_fmod_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

    return init_obj.init();
}
}
