#include "fmod_plugins_settings.h"
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
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
    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String resource_path = project_settings->get_setting(
      vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, PLUGINS_SETTINGS_BASE_PATH, RESOURCE_OPTION),
      FmodPluginsSettings::DEFAULT_RESOURCE_OPTION
    );
    if (resource_path.is_empty()) {
        Ref<FmodPluginsSettings> settings;
        settings.instantiate();
        return settings;
    }

    Ref<FmodPluginsSettings> settings = ResourceLoader::get_singleton()->load(resource_path);
    return settings;
}

void FmodPluginsSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_plugins_base_path", "p_base_path"), &FmodPluginsSettings::set_plugins_base_path);
    ClassDB::bind_method(D_METHOD("get_fmod_plugins_base_path"), &FmodPluginsSettings::get_plugins_base_path);

    ClassDB::bind_method(D_METHOD("set_dynamic_plugin_list", "p_dynamic_plugin_list"), &FmodPluginsSettings::set_dynamic_plugin_list);
    ClassDB::bind_method(D_METHOD("get_dynamic_plugin_list"), &FmodPluginsSettings::get_dynamic_plugin_list);

    ClassDB::bind_method(D_METHOD("set_static_plugins_settings", "p_static_plugins_settings"), &FmodPluginsSettings::set_static_plugins_settings);
    ClassDB::bind_method(D_METHOD("get_static_plugins_settings"), &FmodPluginsSettings::get_static_plugins_settings);

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::STRING,
                    "plugins_base_path",
                    PROPERTY_HINT_DIR,
                    "",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_plugins_base_path",
            "get_fmod_plugins_base_path"
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

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::ARRAY,
                    "static_plugins_settings",
                    PROPERTY_HINT_ARRAY_TYPE,
                    vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "FmodStaticPluginSetting"),
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_static_plugins_settings",
            "get_static_plugins_settings"
    );
}

void FmodPluginsSettings::set_static_plugins_settings(const Array& p_static_plugins_settings) {
    _static_plugins_settings = p_static_plugins_settings;
}

const Array& FmodPluginsSettings::get_static_plugins_settings() const {
    return _static_plugins_settings;
}

void FmodStaticPluginMethod::set_type(FmodStaticPluginMethod::Type p_type) {
    _type = p_type;
}

FmodStaticPluginMethod::Type FmodStaticPluginMethod::get_type() const {
    return _type;
}

void FmodStaticPluginMethod::set_method_name(const String& p_method_name) {
    _method_name = p_method_name;
}

const String& FmodStaticPluginMethod::get_method_name() const {
    return _method_name;
}

void FmodStaticPluginMethod::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_type", "p_type"), &FmodStaticPluginMethod::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &FmodStaticPluginMethod::get_type);

    ClassDB::bind_method(D_METHOD("set_method_name", "p_method_name"), &FmodStaticPluginMethod::set_method_name);
    ClassDB::bind_method(D_METHOD("get_method_name"), &FmodStaticPluginMethod::get_method_name);

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::INT,
                    "type",
                    PROPERTY_HINT_ENUM,
                    "CODEC,DSP,OUTPUT",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_type",
            "get_type"
    );

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::STRING,
                    "method_name",
                    PROPERTY_HINT_NONE,
                    "",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_method_name",
            "get_method_name"
    );

    BIND_ENUM_CONSTANT(CODEC);
    BIND_ENUM_CONSTANT(DSP);
    BIND_ENUM_CONSTANT(OUTPUT);
}

void FmodStaticPluginSetting::set_plugin_name(const String& p_plugin_name) {
    _plugin_name = p_plugin_name;
}

const String& FmodStaticPluginSetting::get_plugin_name() const {
    return _plugin_name;
}

void FmodStaticPluginSetting::set_methods(const Array& p_methods) {
    _methods = p_methods;
}

const Array& FmodStaticPluginSetting::get_methods() const {
    return _methods;
}

void FmodStaticPluginSetting::set_dependent_libraries(const PackedStringArray& p_dependent_libraries) {
    _dependent_libraries = p_dependent_libraries;
}

const PackedStringArray& FmodStaticPluginSetting::get_dependent_libraries() const {
    return _dependent_libraries;
}

void FmodStaticPluginSetting::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_plugin_name", "p_plugin_name"), &FmodStaticPluginSetting::set_plugin_name);
    ClassDB::bind_method(D_METHOD("get_plugin_name"), &FmodStaticPluginSetting::get_plugin_name);

    ClassDB::bind_method(D_METHOD("set_methods", "p_methods"), &FmodStaticPluginSetting::set_methods);
    ClassDB::bind_method(D_METHOD("get_methods"), &FmodStaticPluginSetting::get_methods);

    ClassDB::bind_method(D_METHOD("set_dependent_libraries", "p_dependent_libraries"), &FmodStaticPluginSetting::set_dependent_libraries);
    ClassDB::bind_method(D_METHOD("get_dependent_libraries"), &FmodStaticPluginSetting::get_dependent_libraries);

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::STRING,
                    "plugin_name",
                    PROPERTY_HINT_NONE,
                    "",
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_plugin_name",
            "get_plugin_name"
    );

    ADD_PROPERTY(
            PropertyInfo(
                    Variant::ARRAY,
                    "methods",
                    PROPERTY_HINT_ARRAY_TYPE,
                    vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "FmodStaticPluginMethod"),
                    PROPERTY_USAGE_DEFAULT
            ),
            "set_methods",
            "get_methods"
    );

    ADD_PROPERTY(
      PropertyInfo(
        Variant::PACKED_STRING_ARRAY,
        "dependent_libraries",
        PROPERTY_HINT_NONE
      ),
      "set_dependent_libraries",
      "get_dependent_libraries"
    );
}