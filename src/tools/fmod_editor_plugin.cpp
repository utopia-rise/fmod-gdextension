#ifdef TOOLS_ENABLED

#include "fmod_editor_plugin.h"

#include "classes/os.hpp"
#include "fmod_editor_export_plugin.h"
#include "resources/fmod_plugins_settings.h"

#include <constants.h>
#include <fmod_server.h>
#include <helpers/common.h>
#include <helpers/files.h>
#include <resources/fmod_dsp_settings.h>
#include <resources/fmod_logging_settings.h>
#include <resources/fmod_settings.h>
#include <resources/fmod_software_format_settings.h>
#include <resources/fmod_sound_3d_settings.h>

#include <classes/project_settings.hpp>
#include <classes/scene_tree.hpp>
#include <core/object.hpp>
#include <variant/typed_array.hpp>

using namespace godot;

void FmodEditorPlugin::_ready() {
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FMOD_SETTING_AUTO_INITIALIZE),
      DEFAULT_AUTO_INITIALIZE,
      Variant::Type::BOOL
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::CHANNEL_COUNT_OPTION),
      FmodGeneralSettings::DEFAULT_CHANNEL_COUNT,
      Variant::Type::INT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::IS_LIVE_UPDATE_ENABLED_OPTION),
      FmodGeneralSettings::DEFAULT_IS_LIVEUPDATE,
      Variant::Type::BOOL
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::IS_LIVE_MEMORY_TRACKING_ENABLED_OPTION),
      FmodGeneralSettings::DEFAULT_IS_MEMORY_TRACKING,
      Variant::Type::BOOL
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::SOFTWARE_FORMAT_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::SAMPLE_RATE_OPTION),
      FmodSoftwareFormatSettings::DEFAULT_SAMPLE_RATE,
      Variant::Type::INT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::SOFTWARE_FORMAT_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::SPEAKER_MODE_OPTION),
      FmodSoftwareFormatSettings::DEFAULT_SPEAKER_MODE,
      Variant::Type::INT,
      PROPERTY_HINT_ENUM,
      FmodSoftwareFormatSettings::SPEAKER_MODE_HINT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::SOFTWARE_FORMAT_SETTINGS_BASE_PATH, FmodSoftwareFormatSettings::RAW_SPEAKER_COUNT_OPTION),
      FmodSoftwareFormatSettings::DEFAULT_RAW_SPEAKER_COUNT,
      Variant::Type::INT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::DEFAULT_LISTENER_COUNT_OPTION),
      FmodGeneralSettings::DEFAULT_DEFAULT_LISTENER_COUNT,
      Variant::Type::INT
    );

    String bank_path_option_name =
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::BANKS_PATH_OPTION);

    add_setting(bank_path_option_name,
      FmodGeneralSettings::DEFAULT_BANKS_PATH,
      Variant::Type::STRING,
      PROPERTY_HINT_DIR
    );

    add_setting(
            vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodPluginsSettings::PLUGINS_SETTINGS_BASE_PATH, FmodPluginsSettings::RESOURCE_OPTION),
        FmodPluginsSettings::DEFAULT_RESOURCE_OPTION,
        Variant::Type::STRING,
        PROPERTY_HINT_FILE
    );

    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::INITIALIZE_BASE_PATH, FmodGeneralSettings::SHOULD_LOAD_BY_NAME),
      FmodGeneralSettings::DEFAULT_SHOULD_LOAD_BY_NAME,
      Variant::Type::BOOL
    );

    add_setting(
        vformat(
            "%s/%s/%s",
            FMOD_SETTINGS_BASE_PATH,
            FmodGeneralSettings::INITIALIZE_BASE_PATH,
            FmodGeneralSettings::STUDIO_PROCESSING_OPTION
        ),
        FmodGeneralSettings::DEFAULT_STUDIO_PROCESSING,
        Variant::Type::INT,
        PROPERTY_HINT_ENUM,
        FmodGeneralSettings::STUDIO_PROCESSING_HINT,
        false
    );

    PackedStringArray output_settings;
    for (const char* option :
         {FmodGeneralSettings::OUTPUT_ROUTING_OPTION,
          FmodGeneralSettings::GODOT_AUDIO_BUS_OPTION,
          FmodGeneralSettings::GODOT_BUS_MIXING_OPTION,
          FmodGeneralSettings::SYNC_TO_GODOT_MIX_RATE_OPTION}) {
        output_settings.push_back(
            vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::OUTPUT_BASE_PATH, option)
        );
    }
    add_setting(
        output_settings[0],
        FmodGeneralSettings::DEFAULT_OUTPUT_ROUTING,
        Variant::Type::INT,
        PROPERTY_HINT_ENUM,
        FmodGeneralSettings::OUTPUT_ROUTING_HINT
    );
    add_setting(output_settings[1], FmodGeneralSettings::DEFAULT_GODOT_AUDIO_BUS, Variant::Type::STRING);
    add_setting(
        output_settings[2],
        FmodGeneralSettings::DEFAULT_GODOT_BUS_MIXING,
        Variant::Type::INT,
        PROPERTY_HINT_ENUM,
        FmodGeneralSettings::GODOT_BUS_MIXING_HINT
    );
    add_setting(
        output_settings[3],
        FmodGeneralSettings::DEFAULT_SYNC_TO_GODOT_MIX_RATE,
        Variant::Type::BOOL,
        PROPERTY_HINT_NONE,
        "",
        false
    );

    // Project Settings lists settings in creation order, which differs between new projects and projects that saved
    // some.
    ProjectSettings* project_settings = ProjectSettings::get_singleton();
    for (int i = 1; i < output_settings.size(); ++i) {
        project_settings->set_order(output_settings[i], project_settings->get_order(output_settings[0]) + i);
    }

    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodDspSettings::DSP_SETTINGS_BASE_PATH, FmodDspSettings::DSP_BUFFER_SIZE_OPTION),
      FmodDspSettings::DEFAULT_DSP_BUFFER_SIZE,
      Variant::Type::INT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodDspSettings::DSP_SETTINGS_BASE_PATH, FmodDspSettings::DSP_BUFFER_COUNT_OPTION),
      FmodDspSettings::DEFAULT_DSP_BUFFER_COUNT,
      Variant::Type::INT
    );

    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSound3DSettings::THREE_D_SETTINGS_BASE_PATH, FmodSound3DSettings::DOPPLER_SCALE_OPTION),
      FmodSound3DSettings::DEFAULT_DOPPLER_SCALE,
      Variant::Type::FLOAT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSound3DSettings::THREE_D_SETTINGS_BASE_PATH, FmodSound3DSettings::DISTANCE_FACTOR_OPTION),
      FmodSound3DSettings::DEFAULT_DISTANCE_FACTOR,
      Variant::Type::FLOAT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodSound3DSettings::THREE_D_SETTINGS_BASE_PATH, FmodSound3DSettings::ROLLOFF_SCALE_OPTION),
      FmodSound3DSettings::DEFAULT_ROLLOFF_SCALE,
      Variant::Type::FLOAT
    );
    add_setting(
    vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodLoggingSettings::LOGGING_SETTINGS_BASE_PATH, FmodLoggingSettings::DEBUG_LEVEL_OPTION),
    FmodLoggingSettings::DEFAULT_DEBUG_LEVEL,
    Variant::Type::INT,
    PROPERTY_HINT_ENUM,
    FmodLoggingSettings::DEBUG_LEVEL_HINT
  );
  add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodLoggingSettings::LOGGING_SETTINGS_BASE_PATH, FmodLoggingSettings::LOG_OUTPUT_OPTION),
      FmodLoggingSettings::DEFAULT_LOG_OUTPUT,
      Variant::Type::INT,
      PROPERTY_HINT_ENUM,
      FmodLoggingSettings::LOG_OUTPUT_HINT
    );
    add_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodLoggingSettings::LOGGING_SETTINGS_BASE_PATH, FmodLoggingSettings::LOG_FILE_PATH_OPTION),
      FmodLoggingSettings::DEFAULT_LOG_FILE_PATH,
      Variant::Type::STRING,
      PROPERTY_HINT_FILE,
      "*.txt,*.log"
    );

    update_godot_bus_settings_visibility();
    ProjectSettings::get_singleton()->connect(
        "settings_changed",
        callable_mp(this, &FmodEditorPlugin::update_godot_bus_settings_visibility)
    );
    get_tree()->connect("node_added", callable_mp(this, &FmodEditorPlugin::on_node_added));
}

void FmodEditorPlugin::update_godot_bus_settings_visibility() {
    ProjectSettings* project_settings = ProjectSettings::get_singleton();
    int output_routing = project_settings->get_setting(
        vformat(
            "%s/%s/%s",
            FMOD_SETTINGS_BASE_PATH,
            FmodGeneralSettings::OUTPUT_BASE_PATH,
            FmodGeneralSettings::OUTPUT_ROUTING_OPTION
        ),
        FmodGeneralSettings::DEFAULT_OUTPUT_ROUTING
    );
    bool hidden = output_routing != FmodGeneralSettings::GODOT_BUS;
    if (hidden == godot_bus_settings_hidden) { return; }
    godot_bus_settings_hidden = hidden;

    for (const char* option :
         {FmodGeneralSettings::GODOT_AUDIO_BUS_OPTION,
          FmodGeneralSettings::GODOT_BUS_MIXING_OPTION,
          FmodGeneralSettings::SYNC_TO_GODOT_MIX_RATE_OPTION}) {
        project_settings->set_as_internal(
            vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, FmodGeneralSettings::OUTPUT_BASE_PATH, option),
            hidden
        );
    }

    // The Project Settings dialog rereads which settings it shows only when it rebuilds its sections.
    TypedArray<Node> inspectors =
        get_node_or_null(NodePath("/root"))->find_children("*", "SectionedInspector", true, false);
    for (int i = 0; i < inspectors.size(); ++i) {
        Object::cast_to<Node>(inspectors[i])->call("update_category_list");
    }
}

static String get_settings_class(const String& p_section) {
    if (p_section == FmodGeneralSettings::INITIALIZE_BASE_PATH || p_section == FmodGeneralSettings::OUTPUT_BASE_PATH) {
        return FmodGeneralSettings::get_class_static();
    }
    if (p_section == FmodSoftwareFormatSettings::SOFTWARE_FORMAT_SETTINGS_BASE_PATH) {
        return FmodSoftwareFormatSettings::get_class_static();
    }
    if (p_section == FmodDspSettings::DSP_SETTINGS_BASE_PATH) { return FmodDspSettings::get_class_static(); }
    if (p_section == FmodSound3DSettings::THREE_D_SETTINGS_BASE_PATH) {
        return FmodSound3DSettings::get_class_static();
    }
    if (p_section == FmodLoggingSettings::LOGGING_SETTINGS_BASE_PATH) {
        return FmodLoggingSettings::get_class_static();
    }
    return String();
}

void FmodEditorPlugin::on_node_added(Node* p_node) {
    if (p_node->is_class("EditorProperty")) {
        callable_mp(this, &FmodEditorPlugin::point_tooltip_to_settings_class).call_deferred(p_node->get_instance_id());
    }
}

// A Project Settings tooltip shows the documentation of "property|ProjectSettings|<setting>", which an extension cannot
// add to: point the tooltips of FMOD's settings to the settings resource documenting the same property.
void FmodEditorPlugin::point_tooltip_to_settings_class(uint64_t p_property_id) {
    Object* property = ObjectDB::get_instance(p_property_id);
    if (!property) { return; }

    PackedStringArray tooltip = String(property->get("tooltip_text")).split("|");
    if (tooltip.size() != 3 || tooltip[0] != "property" || tooltip[1] != "ProjectSettings") { return; }

    PackedStringArray setting = tooltip[2].split("/");
    if (setting.size() != 3 || setting[0] != FMOD_SETTINGS_BASE_PATH) { return; }

    String settings_class = get_settings_class(setting[1]);
    if (!settings_class.is_empty()) {
        property->set("tooltip_text", vformat("property|%s|%s", settings_class, setting[2]));
    }
}

void FmodEditorPlugin::add_setting(
    const String& p_name,
    const Variant& p_default_value,
    Variant::Type p_type,
    PropertyHint p_hint,
    const String& p_hint_string,
    bool p_basic
) {
    Dictionary setting;
    setting["name"] = p_name;
    setting["type"] = p_type;
    setting["hint"] = p_hint;
    setting["hint_string"] = p_hint_string;

    if (!ProjectSettings::get_singleton()->has_setting(p_name))
    {
        ProjectSettings::get_singleton()->set_setting(p_name, p_default_value);
    }

    ProjectSettings::get_singleton()->add_property_info(setting);
    ProjectSettings::get_singleton()->set_as_basic(p_name, p_basic);
    ProjectSettings::get_singleton()->set_initial_value(p_name, p_default_value);
}

void FmodEditorPlugin::_bind_methods() {}

#endif
