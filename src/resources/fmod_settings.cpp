#include "fmod_settings.h"

#include <constants.h>

#include <classes/project_settings.hpp>

using namespace godot;

void FmodGeneralSettings::set_channel_count(const int p_channel_count) {
    _channel_count = p_channel_count;
}

int FmodGeneralSettings::get_channel_count() const {
    return _channel_count;
}

void FmodGeneralSettings::set_is_live_update_enabled(const bool p_enable_live_update) {
    _is_live_update_enabled = p_enable_live_update;
}

bool FmodGeneralSettings::get_is_live_update_enabled() const {
    return _is_live_update_enabled;
}

void FmodGeneralSettings::set_is_memory_tracking_enabled(const bool p_enable_memory_tracking) {
    _is_memory_tracking_enabled = p_enable_memory_tracking;
}

bool FmodGeneralSettings::get_is_memory_tracking_enabled() const {
    return _is_memory_tracking_enabled;
}

void FmodGeneralSettings::set_default_listener_count(int p_listener_count) {
    _default_listener_count = p_listener_count;
}

int FmodGeneralSettings::get_default_listener_count() const {
    return _default_listener_count;
}

void FmodGeneralSettings::set_banks_path(const String& p_paths) {
    _banks_path = p_paths;
}

const String& FmodGeneralSettings::get_banks_path() const {
    return _banks_path;
}

void FmodGeneralSettings::set_should_load_by_name(const bool p_should_load_by_name) {
    _should_load_by_name = p_should_load_by_name;
}

bool FmodGeneralSettings::get_should_load_by_name() const {
    return _should_load_by_name;
}

void FmodGeneralSettings::set_studio_processing(StudioProcessing p_studio_processing) {
    _studio_processing = p_studio_processing;
}

FmodGeneralSettings::StudioProcessing FmodGeneralSettings::get_studio_processing() const {
    return _studio_processing;
}

void FmodGeneralSettings::set_output_routing(OutputRouting p_output_routing) {
    _output_routing = p_output_routing;
}

FmodGeneralSettings::OutputRouting FmodGeneralSettings::get_output_routing() const {
    return _output_routing;
}

void FmodGeneralSettings::set_godot_bus_mixing(GodotBusMixing p_godot_bus_mixing) {
    _godot_bus_mixing = p_godot_bus_mixing;
}

FmodGeneralSettings::GodotBusMixing FmodGeneralSettings::get_godot_bus_mixing() const {
    return _godot_bus_mixing;
}

void FmodGeneralSettings::set_sync_to_godot_mix_rate(const bool p_sync_to_godot_mix_rate) {
    _sync_to_godot_mix_rate = p_sync_to_godot_mix_rate;
}

bool FmodGeneralSettings::get_sync_to_godot_mix_rate() const {
    return _sync_to_godot_mix_rate;
}

void FmodGeneralSettings::set_godot_audio_bus(const String& p_godot_audio_bus) {
    _godot_audio_bus = p_godot_audio_bus;
}

const String& FmodGeneralSettings::get_godot_audio_bus() const {
    return _godot_audio_bus;
}

Ref<FmodGeneralSettings> FmodGeneralSettings::get_from_project_settings() {
    Ref<FmodGeneralSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String channel_count_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, CHANNEL_COUNT_OPTION);
    settings->set_channel_count(project_settings->get_setting(channel_count_setting_path, DEFAULT_CHANNEL_COUNT));

    String is_liveupdate_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, IS_LIVE_UPDATE_ENABLED_OPTION);
    settings->set_is_live_update_enabled(project_settings->get_setting(is_liveupdate_setting_path, DEFAULT_IS_LIVEUPDATE));

    String is_memory_tracking_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, IS_LIVE_MEMORY_TRACKING_ENABLED_OPTION);
    settings->set_is_memory_tracking_enabled(project_settings->get_setting(is_memory_tracking_setting_path, DEFAULT_IS_MEMORY_TRACKING));

    String default_listener_count_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, DEFAULT_LISTENER_COUNT_OPTION);
    settings->set_default_listener_count(project_settings->get_setting(default_listener_count_setting_path, DEFAULT_DEFAULT_LISTENER_COUNT));

    String banks_paths_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, BANKS_PATH_OPTION);
    settings->set_banks_path(project_settings->get_setting(banks_paths_setting_path, DEFAULT_BANKS_PATH));

    String should_load_by_name_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, SHOULD_LOAD_BY_NAME);
    settings->set_should_load_by_name(project_settings->get_setting(should_load_by_name_setting_path, DEFAULT_SHOULD_LOAD_BY_NAME));

    String studio_processing_setting_path =
        vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, INITIALIZE_BASE_PATH, STUDIO_PROCESSING_OPTION);
    settings->set_studio_processing(
        static_cast<StudioProcessing>(
            static_cast<int>(project_settings->get_setting(studio_processing_setting_path, DEFAULT_STUDIO_PROCESSING))
        )
    );

    String output_routing_setting_path =
        vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, OUTPUT_BASE_PATH, OUTPUT_ROUTING_OPTION);
    settings->set_output_routing(
      static_cast<OutputRouting>(static_cast<int>(project_settings->get_setting(output_routing_setting_path, DEFAULT_OUTPUT_ROUTING)))
    );

    String godot_audio_bus_setting_path =
        vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, OUTPUT_BASE_PATH, GODOT_AUDIO_BUS_OPTION);
    settings->set_godot_audio_bus(project_settings->get_setting(godot_audio_bus_setting_path, DEFAULT_GODOT_AUDIO_BUS));

    String godot_bus_mixing_setting_path =
        vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, OUTPUT_BASE_PATH, GODOT_BUS_MIXING_OPTION);
    settings->set_godot_bus_mixing(
        static_cast<GodotBusMixing>(
            static_cast<int>(project_settings->get_setting(godot_bus_mixing_setting_path, DEFAULT_GODOT_BUS_MIXING))
        )
    );

    String sync_to_godot_mix_rate_setting_path =
        vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, OUTPUT_BASE_PATH, SYNC_TO_GODOT_MIX_RATE_OPTION);
    settings->set_sync_to_godot_mix_rate(
        project_settings->get_setting(sync_to_godot_mix_rate_setting_path, DEFAULT_SYNC_TO_GODOT_MIX_RATE)
    );

    return settings;
}

void FmodGeneralSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_channel_count", "channel_count"), &FmodGeneralSettings::set_channel_count);
    ClassDB::bind_method(D_METHOD("get_channel_count"), &FmodGeneralSettings::get_channel_count);

    ClassDB::bind_method(D_METHOD("set_is_live_update_enabled", "enable_live_update"), &FmodGeneralSettings::set_is_live_update_enabled);
    ClassDB::bind_method(D_METHOD("get_is_live_update_enabled"), &FmodGeneralSettings::get_is_live_update_enabled);

    ClassDB::bind_method(D_METHOD("set_is_memory_tracking_enabled", "enable_memory_tracking"), &FmodGeneralSettings::set_is_memory_tracking_enabled);
    ClassDB::bind_method(D_METHOD("get_is_memory_tracking_enabled"), &FmodGeneralSettings::get_is_memory_tracking_enabled);

    ClassDB::bind_method(D_METHOD("set_default_listener_count", "listener_count"), &FmodGeneralSettings::set_default_listener_count);
    ClassDB::bind_method(D_METHOD("get_default_listener_count"), &FmodGeneralSettings::get_default_listener_count);

    ClassDB::bind_method(D_METHOD("set_banks_path", "paths"), &FmodGeneralSettings::set_banks_path);
    ClassDB::bind_method(D_METHOD("get_banks_path"), &FmodGeneralSettings::get_banks_path);

    ClassDB::bind_method(D_METHOD("set_should_load_by_name", "should_load_by_name"), &FmodGeneralSettings::set_should_load_by_name);
    ClassDB::bind_method(D_METHOD("get_should_load_by_name"), &FmodGeneralSettings::get_should_load_by_name);

    ClassDB::bind_method(
        D_METHOD("set_studio_processing", "studio_processing"),
        &FmodGeneralSettings::set_studio_processing
    );
    ClassDB::bind_method(D_METHOD("get_studio_processing"), &FmodGeneralSettings::get_studio_processing);

    ClassDB::bind_method(D_METHOD("set_output_routing", "output_routing"), &FmodGeneralSettings::set_output_routing);
    ClassDB::bind_method(D_METHOD("get_output_routing"), &FmodGeneralSettings::get_output_routing);

    ClassDB::bind_method(
        D_METHOD("set_godot_bus_mixing", "godot_bus_mixing"),
        &FmodGeneralSettings::set_godot_bus_mixing
    );
    ClassDB::bind_method(D_METHOD("get_godot_bus_mixing"), &FmodGeneralSettings::get_godot_bus_mixing);

    ClassDB::bind_method(
      D_METHOD("set_sync_to_godot_mix_rate", "sync_to_godot_mix_rate"),
      &FmodGeneralSettings::set_sync_to_godot_mix_rate
    );
    ClassDB::bind_method(D_METHOD("get_sync_to_godot_mix_rate"), &FmodGeneralSettings::get_sync_to_godot_mix_rate);

    ClassDB::bind_method(D_METHOD("set_godot_audio_bus", "godot_audio_bus"), &FmodGeneralSettings::set_godot_audio_bus);
    ClassDB::bind_method(D_METHOD("get_godot_audio_bus"), &FmodGeneralSettings::get_godot_audio_bus);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "channel_count",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_channel_count",
      "get_channel_count"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::BOOL,
        "is_live_update_enabled",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_is_live_update_enabled",
      "get_is_live_update_enabled"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::BOOL,
        "is_memory_tracking_enabled",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_is_memory_tracking_enabled",
      "get_is_memory_tracking_enabled"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "default_listener_count",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_default_listener_count",
      "get_default_listener_count"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::STRING,
        "banks_path",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_banks_path",
      "get_banks_path"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::BOOL,
        "should_load_by_name",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_should_load_by_name",
      "get_should_load_by_name"
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "studio_processing",
            PROPERTY_HINT_ENUM,
            STUDIO_PROCESSING_HINT,
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodGeneralSettings.StudioProcessing"
        ),
        "set_studio_processing",
        "get_studio_processing"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::INT,
        "output_routing",
        PROPERTY_HINT_ENUM,
        OUTPUT_ROUTING_HINT,
        PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
        "FmodGeneralSettings.OutputRouting"
      ),
      "set_output_routing",
      "get_output_routing"
    );

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "godot_bus_mixing",
            PROPERTY_HINT_ENUM,
            GODOT_BUS_MIXING_HINT,
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodGeneralSettings.GodotBusMixing"
        ),
        "set_godot_bus_mixing",
        "get_godot_bus_mixing"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::BOOL,
        "sync_to_godot_mix_rate",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_sync_to_godot_mix_rate",
      "get_sync_to_godot_mix_rate"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::STRING,
        "godot_audio_bus",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_godot_audio_bus",
      "get_godot_audio_bus"
    );

    BIND_ENUM_CONSTANT(BACKGROUND_THREAD);
    BIND_ENUM_CONSTANT(MAIN_THREAD);
    BIND_ENUM_CONSTANT(DEVICE);
    BIND_ENUM_CONSTANT(GODOT_BUS);
    BIND_ENUM_CONSTANT(MIX_AHEAD);
    BIND_ENUM_CONSTANT(ON_DEMAND);
}
