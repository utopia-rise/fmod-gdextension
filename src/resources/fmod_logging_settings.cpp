#include "fmod_logging_settings.h"

#include <constants.h>

#include <classes/project_settings.hpp>

using namespace godot;

void FmodLoggingSettings::set_debug_level(int p_debug_level) {
    _debug_level = p_debug_level;
}

int FmodLoggingSettings::get_debug_level() const {
    return _debug_level;
}

void FmodLoggingSettings::set_log_output(int p_log_output) {
    _log_output = p_log_output;
}

int FmodLoggingSettings::get_log_output() const {
    return _log_output;
}

void FmodLoggingSettings::set_log_file_path(const String& p_log_file_path) {
    _log_file_path = p_log_file_path;
}

const String& FmodLoggingSettings::get_log_file_path() const {
    return _log_file_path;
}

Ref<FmodLoggingSettings> FmodLoggingSettings::get_from_project_settings() {
    Ref<FmodLoggingSettings> settings;
    settings.instantiate();

    ProjectSettings* project_settings = ProjectSettings::get_singleton();

    String debug_level_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, LOGGING_SETTINGS_BASE_PATH, DEBUG_LEVEL_OPTION);
    settings->set_debug_level(project_settings->get_setting(debug_level_setting_path, DEFAULT_DEBUG_LEVEL));

    String log_output_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, LOGGING_SETTINGS_BASE_PATH, LOG_OUTPUT_OPTION);
    settings->set_log_output(project_settings->get_setting(log_output_setting_path, DEFAULT_LOG_OUTPUT));

    String log_file_path_setting_path = vformat("%s/%s/%s", FMOD_SETTINGS_BASE_PATH, LOGGING_SETTINGS_BASE_PATH, LOG_FILE_PATH_OPTION);
    settings->set_log_file_path(project_settings->get_setting(log_file_path_setting_path, DEFAULT_LOG_FILE_PATH));

    return settings;
}

void FmodLoggingSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_debug_level", "debug_level"), &FmodLoggingSettings::set_debug_level);
    ClassDB::bind_method(D_METHOD("get_debug_level"), &FmodLoggingSettings::get_debug_level);

    ClassDB::bind_method(D_METHOD("set_log_output", "log_output"), &FmodLoggingSettings::set_log_output);
    ClassDB::bind_method(D_METHOD("get_log_output"), &FmodLoggingSettings::get_log_output);

    ClassDB::bind_method(D_METHOD("set_log_file_path", "log_file_path"), &FmodLoggingSettings::set_log_file_path);
    ClassDB::bind_method(D_METHOD("get_log_file_path"), &FmodLoggingSettings::get_log_file_path);

    // Debug level property
    ADD_PROPERTY(
      PropertyInfo(Variant::INT, "debug_level", PROPERTY_HINT_ENUM, "None,Error Only,Error and Warning,Full Log,Verbose"),
      "set_debug_level",
      "get_debug_level"
    );

    // Log output property
    ADD_PROPERTY(PropertyInfo(Variant::INT, "log_output", PROPERTY_HINT_ENUM, "TTY,Godot,File"), "set_log_output", "get_log_output");

    // Log file path property
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "log_file_path", PROPERTY_HINT_FILE, "*.txt,*.log", PROPERTY_USAGE_DEFAULT), "set_log_file_path", "get_log_file_path");

    // Bind enum constants
    BIND_ENUM_CONSTANT(DEBUG_NONE);
    BIND_ENUM_CONSTANT(DEBUG_ERROR);
    BIND_ENUM_CONSTANT(DEBUG_WARNING);
    BIND_ENUM_CONSTANT(DEBUG_LOG);
    BIND_ENUM_CONSTANT(DEBUG_VERBOSE);

    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_TTY);
    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_CALLBACK);
    BIND_ENUM_CONSTANT(FMOD_DEBUG_MODE_FILE);
}
