#include "fmod_plugins_settings.h"
#include "classes/project_settings.hpp"
#include "constants.h"

using namespace godot;

void FmodPluginsSettings::set_plugins_base_path(const String& p_base_path) {
    _plugins_base_path = p_base_path;
}

const String& FmodPluginsSettings::get_plugins_base_path() const {
    return _plugins_base_path;
}

void FmodPluginsSettings::set_dynamic_plugin_list(const PackedStringArray& p_dynamic_plugin_list) {
    _dynamic_plugin_list = p_dynamic_plugin_list;
}

const PackedStringArray& FmodPluginsSettings::get_dynamic_plugin_list() const {
    return _dynamic_plugin_list;
}

Ref<FmodPluginsSettings> FmodPluginsSettings::get_from_project_settings() {
    Ref<FmodPluginsSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    settings->set_plugins_base_path(
            project_settings->get_setting(vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, PLUGINS_SETTINGS_BASE_PATH, BASE_PATH_OPTION), DEFAULT_BASE_PATH)
    );

    settings->set_dynamic_plugin_list(
            project_settings->get_setting(vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, PLUGINS_SETTINGS_BASE_PATH, DYNAMIC_PLUGIN_LIST_OPTION), PackedStringArray())
    );

    return settings;
}

void FmodPluginsSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_plugins_base_path", "p_base_path"), &FmodPluginsSettings::set_plugins_base_path);
    ClassDB::bind_method(D_METHOD("get_plugins_base_path"), &FmodPluginsSettings::get_plugins_base_path);

    ClassDB::bind_method(D_METHOD("set_dynamic_plugin_list", "p_dynamic_plugin_list"), &FmodPluginsSettings::set_dynamic_plugin_list);
    ClassDB::bind_method(D_METHOD("get_dynamic_plugin_list"), &FmodPluginsSettings::get_dynamic_plugin_list);

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::STRING,
                    "plugins_base_path",
                    PROPERTY_HINT_DIR,
                    "",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_plugins_base_path",
            "get_plugins_base_path"
    );

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::PACKED_STRING_ARRAY,
                    "dynamic_plugin_list",
                    PROPERTY_HINT_NONE,
                    "",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_dynamic_plugin_list",
            "get_dynamic_plugin_list"
    );
}