#ifdef TOOLS_ENABLED

#include "fmod_editor_plugin.h"

#include "resources/fmod_plugins_settings.h"

#include <constants.h>
#include <resources/fmod_settings.h>

#include <classes/project_settings.hpp>
#include <variant/typed_array.hpp>

using namespace godot;

static String get_setting_path(const char* p_option) {
    return vformat("%s/%s", FMOD_SETTINGS_BASE_PATH, p_option);
}

static bool move_old_setting(const char* p_old_option, const char* p_option) {
    ProjectSettings* project_settings = ProjectSettings::get_singleton();
    if (!project_settings->has_setting(get_setting_path(p_old_option))) { return false; }
    project_settings->set_setting(
        get_setting_path(p_option),
        project_settings->get_setting(get_setting_path(p_old_option))
    );
    project_settings->set_setting(get_setting_path(p_old_option), Variant());
    return true;
}

void FmodEditorPlugin::_ready() {
    ProjectSettings* project_settings = ProjectSettings::get_singleton();
    Ref<FmodSettings> defaults;
    defaults.instantiate();

    bool moved = false;
    for (const FmodProjectSetting& setting : FmodSettings::PROJECT_SETTINGS) {
        if (setting.old_path) { moved |= move_old_setting(setting.old_path, setting.path); }
    }
    moved |= move_old_setting("Plugins/path_to_plugin_configuration", FmodPluginsSettings::RESOURCE_OPTION);
    if (moved) { project_settings->save(); }

    String auto_initialize = get_setting_path(FMOD_SETTING_AUTO_INITIALIZE);
    add_setting(auto_initialize, DEFAULT_AUTO_INITIALIZE, Variant::BOOL);
    // Project Settings lists settings in creation order, which differs between new projects and projects that saved
    // some.
    int order = project_settings->get_order(auto_initialize);
    Dictionary properties;
    TypedArray<Dictionary> property_list = defaults->get_property_list();
    for (int i = 0; i < property_list.size(); ++i) {
        Dictionary property = property_list[i];
        properties[property["name"]] = property;
    }
    for (const FmodProjectSetting& setting : FmodSettings::PROJECT_SETTINGS) {
        String path = get_setting_path(setting.path);
        Dictionary property = properties[setting.property];
        add_setting(
            path,
            defaults->get(setting.property),
            static_cast<Variant::Type>(static_cast<int>(property["type"])),
            static_cast<PropertyHint>(static_cast<int>(property["hint"])),
            property["hint_string"],
            setting.basic
        );
        project_settings->set_order(path, ++order);
    }
    String plugins = get_setting_path(FmodPluginsSettings::RESOURCE_OPTION);
    add_setting(plugins, FmodPluginsSettings::DEFAULT_RESOURCE_OPTION, Variant::STRING, PROPERTY_HINT_FILE);
    project_settings->set_order(plugins, ++order);
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
