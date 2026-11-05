#include "fmod_settings.h"

#include <constants.h>

#include <classes/os.hpp>
#include <classes/project_settings.hpp>

using namespace godot;

Ref<FmodSettings> FmodSettings::cached_project_settings;

void FmodSettings::set_banks_path(const String& p_banks_path) {
    _banks_path = p_banks_path;
}

const String& FmodSettings::get_banks_path() const {
    return _banks_path;
}

void FmodSettings::set_load_by_name(bool p_load_by_name) {
    _load_by_name = p_load_by_name;
}

bool FmodSettings::get_load_by_name() const {
    return _load_by_name;
}

void FmodSettings::set_channel_count(int p_channel_count) {
    _channel_count = p_channel_count;
}

int FmodSettings::get_channel_count() const {
    return _channel_count;
}

void FmodSettings::set_real_channel_count(int p_real_channel_count) {
    _real_channel_count = p_real_channel_count;
}

int FmodSettings::get_real_channel_count() const {
    return _real_channel_count;
}

void FmodSettings::set_per_codec_channel_count(int p_per_codec_channel_count) {
    _per_codec_channel_count = p_per_codec_channel_count;
}

int FmodSettings::get_per_codec_channel_count() const {
    return _per_codec_channel_count;
}

void FmodSettings::set_sample_rate(int p_sample_rate) {
    _sample_rate = p_sample_rate;
}

int FmodSettings::get_sample_rate() const {
    return _sample_rate;
}

void FmodSettings::set_speaker_mode(SpeakerMode p_speaker_mode) {
    _speaker_mode = p_speaker_mode;
}

FmodSettings::SpeakerMode FmodSettings::get_speaker_mode() const {
    return _speaker_mode;
}

void FmodSettings::set_raw_speaker_count(int p_raw_speaker_count) {
    _raw_speaker_count = p_raw_speaker_count;
}

int FmodSettings::get_raw_speaker_count() const {
    return _raw_speaker_count;
}

void FmodSettings::set_output_routing(OutputRouting p_output_routing) {
    _output_routing = p_output_routing;
}

FmodSettings::OutputRouting FmodSettings::get_output_routing() const {
    return _output_routing;
}

void FmodSettings::set_godot_audio_bus(const String& p_godot_audio_bus) {
    _godot_audio_bus = p_godot_audio_bus;
}

const String& FmodSettings::get_godot_audio_bus() const {
    return _godot_audio_bus;
}

void FmodSettings::set_godot_bus_mixing(GodotBusMixing p_godot_bus_mixing) {
    _godot_bus_mixing = p_godot_bus_mixing;
}

FmodSettings::GodotBusMixing FmodSettings::get_godot_bus_mixing() const {
    return _godot_bus_mixing;
}

void FmodSettings::set_sync_to_godot_mix_rate(bool p_sync_to_godot_mix_rate) {
    _sync_to_godot_mix_rate = p_sync_to_godot_mix_rate;
}

bool FmodSettings::get_sync_to_godot_mix_rate() const {
    return _sync_to_godot_mix_rate;
}

void FmodSettings::set_dsp_buffer_size(int p_dsp_buffer_size) {
    _dsp_buffer_size = p_dsp_buffer_size;
}

int FmodSettings::get_dsp_buffer_size() const {
    return _dsp_buffer_size;
}

void FmodSettings::set_dsp_buffer_count(int p_dsp_buffer_count) {
    _dsp_buffer_count = p_dsp_buffer_count;
}

int FmodSettings::get_dsp_buffer_count() const {
    return _dsp_buffer_count;
}

void FmodSettings::set_studio_processing(StudioProcessing p_studio_processing) {
    _studio_processing = p_studio_processing;
}

FmodSettings::StudioProcessing FmodSettings::get_studio_processing() const {
    return _studio_processing;
}

void FmodSettings::set_listener_count(int p_listener_count) {
    _listener_count = p_listener_count;
}

int FmodSettings::get_listener_count() const {
    return _listener_count;
}

void FmodSettings::set_distance_factor_2d(float p_distance_factor) {
    _distance_factor_2d = p_distance_factor;
}

float FmodSettings::get_distance_factor_2d() const {
    return _distance_factor_2d;
}

void FmodSettings::set_distance_factor_3d(float p_distance_factor) {
    _distance_factor_3d = p_distance_factor;
}

float FmodSettings::get_distance_factor_3d() const {
    return _distance_factor_3d;
}

void FmodSettings::set_rolloff_scale(float p_rolloff_scale) {
    _rolloff_scale = p_rolloff_scale;
}

float FmodSettings::get_rolloff_scale() const {
    return _rolloff_scale;
}

void FmodSettings::set_doppler_scale(float p_doppler_scale) {
    _doppler_scale = p_doppler_scale;
}

float FmodSettings::get_doppler_scale() const {
    return _doppler_scale;
}

void FmodSettings::set_live_update(bool p_live_update) {
    _live_update = p_live_update;
}

bool FmodSettings::get_live_update() const {
    return _live_update;
}

void FmodSettings::set_memory_tracking(bool p_memory_tracking) {
    _memory_tracking = p_memory_tracking;
}

bool FmodSettings::get_memory_tracking() const {
    return _memory_tracking;
}

void FmodSettings::set_log_level(LogLevel p_log_level) {
    _log_level = p_log_level;
}

FmodSettings::LogLevel FmodSettings::get_log_level() const {
    return _log_level;
}

void FmodSettings::set_log_output(LogOutput p_log_output) {
    _log_output = p_log_output;
}

FmodSettings::LogOutput FmodSettings::get_log_output() const {
    return _log_output;
}

void FmodSettings::set_log_file_path(const String& p_log_file_path) {
    _log_file_path = p_log_file_path;
}

const String& FmodSettings::get_log_file_path() const {
    return _log_file_path;
}

unsigned int FmodSettings::get_fmod_debug_flags() const {
    switch (_log_level) {
        case LOG_LEVEL_NONE:
            return FMOD_DEBUG_LEVEL_NONE;
        case LOG_LEVEL_ERROR:
            return FMOD_DEBUG_LEVEL_ERROR;
        case LOG_LEVEL_LOG:
            return FMOD_DEBUG_LEVEL_ERROR | FMOD_DEBUG_LEVEL_WARNING | FMOD_DEBUG_LEVEL_LOG;
        case LOG_LEVEL_VERBOSE:
            return FMOD_DEBUG_LEVEL_ERROR | FMOD_DEBUG_LEVEL_WARNING | FMOD_DEBUG_LEVEL_LOG | FMOD_DEBUG_TYPE_TRACE;
        case LOG_LEVEL_INHERIT:
            if (OS::get_singleton()->is_stdout_verbose()) {
                return FMOD_DEBUG_LEVEL_ERROR | FMOD_DEBUG_LEVEL_WARNING | FMOD_DEBUG_LEVEL_LOG | FMOD_DEBUG_TYPE_TRACE;
            }
            return FMOD_DEBUG_LEVEL_ERROR | FMOD_DEBUG_LEVEL_WARNING;
        default:
            return FMOD_DEBUG_LEVEL_ERROR | FMOD_DEBUG_LEVEL_WARNING;
    }
}

void FmodSettings::set_plugins(const Ref<FmodPluginsSettings>& p_plugins) {
    _plugins = p_plugins;
}

Ref<FmodPluginsSettings> FmodSettings::get_plugins() const {
    return _plugins;
}

Ref<FmodSettings> FmodSettings::get_from_project_settings() {
    if (cached_project_settings.is_valid()) { return cached_project_settings; }

    ProjectSettings* project_settings = ProjectSettings::get_singleton();
    Ref<FmodSettings> settings;
    settings.instantiate();
    for (const FmodProjectSetting& setting : PROJECT_SETTINGS) {
        String path = vformat("%s/%s", FMOD_SETTINGS_BASE_PATH, setting.path);
        if (project_settings->has_setting(path)) {
            settings->set(setting.property, project_settings->get_setting(path));
        }
    }
    settings->set_plugins(FmodPluginsSettings::get_from_project_settings());
    cached_project_settings = settings;
    return settings;
}

void FmodSettings::free_project_settings() {
    cached_project_settings.unref();
}

void FmodSettings::_bind_methods() {
    // One binding per line, some wider than the column limit, to read as a list.
    // clang-format off
    ClassDB::bind_method(D_METHOD("set_banks_path", "banks_path"), &FmodSettings::set_banks_path);
    ClassDB::bind_method(D_METHOD("get_banks_path"), &FmodSettings::get_banks_path);
    ClassDB::bind_method(D_METHOD("set_load_by_name", "load_by_name"), &FmodSettings::set_load_by_name);
    ClassDB::bind_method(D_METHOD("get_load_by_name"), &FmodSettings::get_load_by_name);
    ClassDB::bind_method(D_METHOD("set_channel_count", "channel_count"), &FmodSettings::set_channel_count);
    ClassDB::bind_method(D_METHOD("get_channel_count"), &FmodSettings::get_channel_count);
    ClassDB::bind_method(D_METHOD("set_real_channel_count", "real_channel_count"), &FmodSettings::set_real_channel_count);
    ClassDB::bind_method(D_METHOD("get_real_channel_count"), &FmodSettings::get_real_channel_count);
    ClassDB::bind_method(D_METHOD("set_per_codec_channel_count", "per_codec_channel_count"), &FmodSettings::set_per_codec_channel_count);
    ClassDB::bind_method(D_METHOD("get_per_codec_channel_count"), &FmodSettings::get_per_codec_channel_count);
    ClassDB::bind_method(D_METHOD("set_sample_rate", "sample_rate"), &FmodSettings::set_sample_rate);
    ClassDB::bind_method(D_METHOD("get_sample_rate"), &FmodSettings::get_sample_rate);
    ClassDB::bind_method(D_METHOD("set_speaker_mode", "speaker_mode"), &FmodSettings::set_speaker_mode);
    ClassDB::bind_method(D_METHOD("get_speaker_mode"), &FmodSettings::get_speaker_mode);
    ClassDB::bind_method(D_METHOD("set_raw_speaker_count", "raw_speaker_count"), &FmodSettings::set_raw_speaker_count);
    ClassDB::bind_method(D_METHOD("get_raw_speaker_count"), &FmodSettings::get_raw_speaker_count);
    ClassDB::bind_method(D_METHOD("set_output_routing", "output_routing"), &FmodSettings::set_output_routing);
    ClassDB::bind_method(D_METHOD("get_output_routing"), &FmodSettings::get_output_routing);
    ClassDB::bind_method(D_METHOD("set_godot_audio_bus", "godot_audio_bus"), &FmodSettings::set_godot_audio_bus);
    ClassDB::bind_method(D_METHOD("get_godot_audio_bus"), &FmodSettings::get_godot_audio_bus);
    ClassDB::bind_method(D_METHOD("set_godot_bus_mixing", "godot_bus_mixing"), &FmodSettings::set_godot_bus_mixing);
    ClassDB::bind_method(D_METHOD("get_godot_bus_mixing"), &FmodSettings::get_godot_bus_mixing);
    ClassDB::bind_method(D_METHOD("set_sync_to_godot_mix_rate", "sync_to_godot_mix_rate"), &FmodSettings::set_sync_to_godot_mix_rate);
    ClassDB::bind_method(D_METHOD("get_sync_to_godot_mix_rate"), &FmodSettings::get_sync_to_godot_mix_rate);
    ClassDB::bind_method(D_METHOD("set_dsp_buffer_size", "dsp_buffer_size"), &FmodSettings::set_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("get_dsp_buffer_size"), &FmodSettings::get_dsp_buffer_size);
    ClassDB::bind_method(D_METHOD("set_dsp_buffer_count", "dsp_buffer_count"), &FmodSettings::set_dsp_buffer_count);
    ClassDB::bind_method(D_METHOD("get_dsp_buffer_count"), &FmodSettings::get_dsp_buffer_count);
    ClassDB::bind_method(D_METHOD("set_studio_processing", "studio_processing"), &FmodSettings::set_studio_processing);
    ClassDB::bind_method(D_METHOD("get_studio_processing"), &FmodSettings::get_studio_processing);
    ClassDB::bind_method(D_METHOD("set_listener_count", "listener_count"), &FmodSettings::set_listener_count);
    ClassDB::bind_method(D_METHOD("get_listener_count"), &FmodSettings::get_listener_count);
    ClassDB::bind_method(D_METHOD("set_distance_factor_2d", "distance_factor"), &FmodSettings::set_distance_factor_2d);
    ClassDB::bind_method(D_METHOD("get_distance_factor_2d"), &FmodSettings::get_distance_factor_2d);
    ClassDB::bind_method(D_METHOD("set_distance_factor_3d", "distance_factor"), &FmodSettings::set_distance_factor_3d);
    ClassDB::bind_method(D_METHOD("get_distance_factor_3d"), &FmodSettings::get_distance_factor_3d);
    ClassDB::bind_method(D_METHOD("set_rolloff_scale", "rolloff_scale"), &FmodSettings::set_rolloff_scale);
    ClassDB::bind_method(D_METHOD("get_rolloff_scale"), &FmodSettings::get_rolloff_scale);
    ClassDB::bind_method(D_METHOD("set_doppler_scale", "doppler_scale"), &FmodSettings::set_doppler_scale);
    ClassDB::bind_method(D_METHOD("get_doppler_scale"), &FmodSettings::get_doppler_scale);
    ClassDB::bind_method(D_METHOD("set_live_update", "live_update"), &FmodSettings::set_live_update);
    ClassDB::bind_method(D_METHOD("get_live_update"), &FmodSettings::get_live_update);
    ClassDB::bind_method(D_METHOD("set_memory_tracking", "memory_tracking"), &FmodSettings::set_memory_tracking);
    ClassDB::bind_method(D_METHOD("get_memory_tracking"), &FmodSettings::get_memory_tracking);
    ClassDB::bind_method(D_METHOD("set_log_level", "log_level"), &FmodSettings::set_log_level);
    ClassDB::bind_method(D_METHOD("get_log_level"), &FmodSettings::get_log_level);
    ClassDB::bind_method(D_METHOD("set_log_output", "log_output"), &FmodSettings::set_log_output);
    ClassDB::bind_method(D_METHOD("get_log_output"), &FmodSettings::get_log_output);
    ClassDB::bind_method(D_METHOD("set_log_file_path", "log_file_path"), &FmodSettings::set_log_file_path);
    ClassDB::bind_method(D_METHOD("get_log_file_path"), &FmodSettings::get_log_file_path);
    ClassDB::bind_method(D_METHOD("set_plugins", "plugins"), &FmodSettings::set_plugins);
    ClassDB::bind_method(D_METHOD("get_plugins"), &FmodSettings::get_plugins);
    // clang-format on

    ADD_GROUP("Banks", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "banks_path", PROPERTY_HINT_DIR, ""),
        "set_banks_path",
        "get_banks_path"
    );
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "load_by_name"), "set_load_by_name", "get_load_by_name");

    ADD_GROUP("Mixer", "");
    ADD_SUBGROUP("Voices", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "channel_count", PROPERTY_HINT_RANGE, "1,4095"),
        "set_channel_count",
        "get_channel_count"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "real_channel_count", PROPERTY_HINT_RANGE, "1,4095"),
        "set_real_channel_count",
        "get_real_channel_count"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "per_codec_channel_count", PROPERTY_HINT_RANGE, "0,256"),
        "set_per_codec_channel_count",
        "get_per_codec_channel_count"
    );
    ADD_SUBGROUP("Format", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "sample_rate", PROPERTY_HINT_RANGE, "8000,192000"),
        "set_sample_rate",
        "get_sample_rate"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "speaker_mode",
            PROPERTY_HINT_ENUM,
            "Default,Raw,Mono,Stereo,Quad,Surround,5.1,7.1,7.1.4",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.SpeakerMode"
        ),
        "set_speaker_mode",
        "get_speaker_mode"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "raw_speaker_count", PROPERTY_HINT_RANGE, "0,32"),
        "set_raw_speaker_count",
        "get_raw_speaker_count"
    );
    ADD_SUBGROUP("Output", "");
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "output_routing",
            PROPERTY_HINT_ENUM,
            "Direct to Device,Godot Bus",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.OutputRouting"
        ),
        "set_output_routing",
        "get_output_routing"
    );
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "godot_audio_bus"), "set_godot_audio_bus", "get_godot_audio_bus");
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "godot_bus_mixing",
            PROPERTY_HINT_ENUM,
            "Mix Ahead,On Demand",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.GodotBusMixing"
        ),
        "set_godot_bus_mixing",
        "get_godot_bus_mixing"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "sync_to_godot_mix_rate"),
        "set_sync_to_godot_mix_rate",
        "get_sync_to_godot_mix_rate"
    );
    ADD_SUBGROUP("Latency", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "dsp_buffer_size", PROPERTY_HINT_RANGE, "64,8192"),
        "set_dsp_buffer_size",
        "get_dsp_buffer_size"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "dsp_buffer_count", PROPERTY_HINT_RANGE, "2,16"),
        "set_dsp_buffer_count",
        "get_dsp_buffer_count"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "studio_processing",
            PROPERTY_HINT_ENUM,
            "Background Thread,Main Thread",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.StudioProcessing"
        ),
        "set_studio_processing",
        "get_studio_processing"
    );

    ADD_GROUP("Spatialization", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "listener_count", PROPERTY_HINT_RANGE, "1,8"),
        "set_listener_count",
        "get_listener_count"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::FLOAT, "distance_factor_2d", PROPERTY_HINT_RANGE, "0.001,1000,or_greater"),
        "set_distance_factor_2d",
        "get_distance_factor_2d"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::FLOAT, "distance_factor_3d", PROPERTY_HINT_RANGE, "0.001,1000,or_greater"),
        "set_distance_factor_3d",
        "get_distance_factor_3d"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::FLOAT, "rolloff_scale", PROPERTY_HINT_RANGE, "0,10,or_greater"),
        "set_rolloff_scale",
        "get_rolloff_scale"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::FLOAT, "doppler_scale", PROPERTY_HINT_RANGE, "0,10,or_greater"),
        "set_doppler_scale",
        "get_doppler_scale"
    );

    ADD_GROUP("Debugging", "");
    ADD_SUBGROUP("Live Update", "");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "live_update"), "set_live_update", "get_live_update");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "memory_tracking"), "set_memory_tracking", "get_memory_tracking");
    ADD_SUBGROUP("Logging", "");
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "log_level",
            PROPERTY_HINT_ENUM,
            "Inherit,None,Error,Warning,Log,Verbose",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.LogLevel"
        ),
        "set_log_level",
        "get_log_level"
    );
    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "log_output",
            PROPERTY_HINT_ENUM,
            "TTY,File,Godot",
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM,
            "FmodSettings.LogOutput"
        ),
        "set_log_output",
        "get_log_output"
    );
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "log_file_path", PROPERTY_HINT_FILE, "*.txt,*.log"),
        "set_log_file_path",
        "get_log_file_path"
    );

    ADD_GROUP("Plugins", "");
    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "plugins", PROPERTY_HINT_RESOURCE_TYPE, "FmodPluginsSettings"),
        "set_plugins",
        "get_plugins"
    );
    BIND_ENUM_CONSTANT(DEFAULT);
    BIND_ENUM_CONSTANT(RAW);
    BIND_ENUM_CONSTANT(MONO);
    BIND_ENUM_CONSTANT(STEREO);
    BIND_ENUM_CONSTANT(QUAD);
    BIND_ENUM_CONSTANT(SURROUND);
    BIND_ENUM_CONSTANT(SPEAKERMODE_5POINT1);
    BIND_ENUM_CONSTANT(SPEAKERMODE_7POINT1);
    BIND_ENUM_CONSTANT(SPEAKERMODE_7POINT1POINT4);
    BIND_ENUM_CONSTANT(DEVICE);
    BIND_ENUM_CONSTANT(GODOT_BUS);
    BIND_ENUM_CONSTANT(MIX_AHEAD);
    BIND_ENUM_CONSTANT(ON_DEMAND);
    BIND_ENUM_CONSTANT(BACKGROUND_THREAD);
    BIND_ENUM_CONSTANT(MAIN_THREAD);
    BIND_ENUM_CONSTANT(LOG_LEVEL_INHERIT);
    BIND_ENUM_CONSTANT(LOG_LEVEL_NONE);
    BIND_ENUM_CONSTANT(LOG_LEVEL_ERROR);
    BIND_ENUM_CONSTANT(LOG_LEVEL_WARNING);
    BIND_ENUM_CONSTANT(LOG_LEVEL_LOG);
    BIND_ENUM_CONSTANT(LOG_LEVEL_VERBOSE);
    BIND_ENUM_CONSTANT(LOG_OUTPUT_TTY);
    BIND_ENUM_CONSTANT(LOG_OUTPUT_FILE);
    BIND_ENUM_CONSTANT(LOG_OUTPUT_GODOT);
    ;
}
