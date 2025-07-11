#ifdef TOOLS_ENABLED

#include "fmod_editor_plugin.h"

#include "fmod_editor_export_plugin.h"
#include "classes/os.hpp"
#include "resources/fmod_plugins_settings.h"

#include <constants.h>
#include <fmod_server.h>
#include <helpers/common.h>
#include <helpers/files.h>
#include <resources/fmod_dsp_settings.h>
#include <resources/fmod_settings.h>
#include <resources/fmod_software_format_settings.h>
#include <resources/fmod_sound_3d_settings.h>

#include <classes/project_settings.hpp>

using namespace godot;

static constexpr const char* MASTER_BANK_NAME = "Master.bank";
static constexpr const char* MASTER_STRINGS_BANK_NAME = "Master.strings.bank";

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
      "DEFAULT,RAW,MONO,STEREO,QUAD,SURROUND,5POINT1,7POINT1,7POINT1POINT4"
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

    String banks_root = ProjectSettings::get_singleton()->get_setting(bank_path_option_name);

    const String master_strings_bank_path {vformat("%s/%s", banks_root, MASTER_STRINGS_BANK_NAME)};
    if (!FileAccess::file_exists(master_strings_bank_path)) {
        GODOT_LOG_WARNING(vformat("Cannot find master strings bank at %s", master_strings_bank_path));
        return;
    }

    banks.append(
            FmodServer::get_singleton()->load_bank(master_strings_bank_path, FMOD_STUDIO_LOAD_BANK_NORMAL)
    );

    const String master_bank_path {vformat("%s/%s", banks_root, MASTER_BANK_NAME)};
    if (!FileAccess::file_exists(master_bank_path)) {
        GODOT_LOG_WARNING(vformat("Cannot find master bank at %s", master_bank_path));
        return;
    }

    banks.append(
            FmodServer::get_singleton()->load_bank(master_bank_path, FMOD_STUDIO_LOAD_BANK_NORMAL)
    );

    PackedStringArray banks_path;
    list_files_in_folder(banks_path, banks_root, ".bank");
    for (const String& bank_path : banks_path) {
        if (bank_path.ends_with(MASTER_BANK_NAME) || bank_path.ends_with(MASTER_STRINGS_BANK_NAME)) {
            continue;
        }
        banks.append(
                FmodServer::get_singleton()->load_bank(bank_path, FMOD_STUDIO_LOAD_BANK_NORMAL)
        );
    }
}

void FmodEditorPlugin::add_setting(
  const String& p_name,
  const Variant& p_default_value,
  Variant::Type p_type,
  PropertyHint p_hint,
  const String& p_hint_string,
  int p_usage
) {
    Dictionary setting;
    setting["name"] = p_name;
    setting["type"] = p_type;
    setting["hint"] = p_hint;
    setting["hint_string"] = p_hint_string;
    setting["usage"] = p_usage;

    if (ProjectSettings::get_singleton()->has_setting(p_name))
    {
        ProjectSettings::get_singleton()->add_property_info(setting);
        return;
    }

    ProjectSettings::get_singleton()->set_setting(p_name, p_default_value);
    ProjectSettings::get_singleton()->add_property_info(setting);
}

void FmodEditorPlugin::_bind_methods() {}

#endif