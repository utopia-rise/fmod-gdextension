#include "constants.h"
#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "fmod_server.h"
#include "nodes/fmod_bank_loader.h"
#include "nodes/fmod_event_emitter_2d.h"
#include "nodes/fmod_event_emitter_3d.h"
#include "nodes/fmod_listener_2d.h"
#include "nodes/fmod_listener_3d.h"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "fmod_string_names.h"

#ifdef TOOLS_ENABLED
#include <tools//fmod_editor_export_plugin.h>
#endif

#include <register_types.h>
#include <resources/fmod_dsp_settings.h>
#include <resources/fmod_software_format_settings.h>
#include <studio/fmod_parameter_description.h>
#include <tools/fmod_editor_plugin.h>

#include <classes/engine.hpp>
#include <classes/project_settings.hpp>

using namespace godot;

static FmodServer* fmod_singleton;

void initialize_fmod_with_settings() {
    Ref<FmodGeneralSettings> general_settings = FmodGeneralSettings::get_from_project_settings();
    Ref<FmodSoftwareFormatSettings> software_format_settings = FmodSoftwareFormatSettings::get_from_project_settings();
    Ref<FmodDspSettings> dsp_settings = FmodDspSettings::get_from_project_settings();
    Ref<FmodSound3DSettings> three_d_settings = FmodSound3DSettings::get_from_project_settings();

    FmodServer::get_singleton()->set_software_format(software_format_settings);
    FmodServer::get_singleton()->set_system_dsp_buffer_size(dsp_settings);
    FmodServer::get_singleton()->init(general_settings);
    FmodServer::get_singleton()->set_sound_3d_settings(three_d_settings);
    FmodServer::get_singleton()->set_system_listener_number(general_settings->get_default_listener_count());

    FmodServer::get_singleton()->load_all_plugins(FmodPluginsSettings::get_from_project_settings());
}

void initialize_fmod() {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) {
        initialize_fmod_with_settings();
        return;
    }
#endif

    bool auto_initialize = ProjectSettings::get_singleton()->get_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FMOD_SETTING_AUTO_INITIALIZE),
      DEFAULT_AUTO_INITIALIZE
    );

    if (auto_initialize) {
        initialize_fmod_with_settings();
    }
}

void initialize_fmod_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
        // initialise filerunner singleton by calling it.
        FmodStringNames::create();
        Callbacks::GodotFileRunner::get_singleton();
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        // Data
        ClassDB::register_class<FmodPerformanceData>();

        // Core
        ClassDB::register_class<FmodSound>();
        ClassDB::register_class<FmodFile>();

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

        // Resources
        ClassDB::register_class<FmodGeneralSettings>();
        ClassDB::register_class<FmodSoftwareFormatSettings>();
        ClassDB::register_class<FmodDspSettings>();
        ClassDB::register_class<FmodSound3DSettings>();
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
        Callbacks::GodotFileRunner::get_singleton()->finish();
        FmodStringNames::free();
    }
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        fmod_singleton->shutdown();

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