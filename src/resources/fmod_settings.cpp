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

    return settings;
}

void FmodGeneralSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_channel_count", "p_channel_count"), &FmodGeneralSettings::set_channel_count);
    ClassDB::bind_method(D_METHOD("get_channel_count"), &FmodGeneralSettings::get_channel_count);

    ClassDB::bind_method(D_METHOD("set_is_live_update_enabled", "p_enable_live_update"), &FmodGeneralSettings::set_is_live_update_enabled);
    ClassDB::bind_method(D_METHOD("get_is_live_update_enabled"), &FmodGeneralSettings::get_is_live_update_enabled);

    ClassDB::bind_method(D_METHOD("set_is_memory_tracking_enabled", "p_enable_memory_tracking"), &FmodGeneralSettings::set_is_memory_tracking_enabled);
    ClassDB::bind_method(D_METHOD("get_is_memory_tracking_enabled"), &FmodGeneralSettings::get_is_memory_tracking_enabled);

    ClassDB::bind_method(D_METHOD("set_default_listener_count", "p_listener_count"), &FmodGeneralSettings::set_default_listener_count);
    ClassDB::bind_method(D_METHOD("get_default_listener_count"), &FmodGeneralSettings::get_default_listener_count);

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
}