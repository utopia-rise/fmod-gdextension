#include "fmod_runtime.h"

#include <callback/audio_callbacks.h>
#include <fmod_server.h>

#include <classes/audio_stream_player.hpp>
#include <classes/display_server.hpp>
#include <classes/engine.hpp>
#include <classes/os.hpp>
#include <classes/performance.hpp>
#include <limits>

using namespace godot;

static const char* const PERFORMANCE_MONITORS[][2] = {
    {"FMOD [Core]/Cpu DSP", "dsp"},
    {"FMOD [Core]/Cpu Geometry", "geometry"},
    {"FMOD [Core]/Cpu Stream", "stream"},
    {"FMOD/[Core] Cpu Update", "update"},
    {"FMOD/[Core] Cpu convolution Thread 1", "convolution1"},
    {"FMOD/[Core] Cpu convolution Thread 2", "convolution2"},
    {"FMOD/[Studio] Cpu Update", "studio"},
    {"FMOD/[Memory] Currently allocated", "currently_allocated"},
    {"FMOD/[Memory] Max allocated", "max_allocated"},
    {"FMOD/[File] Sample bytes read", "sample_bytes_read"},
    {"FMOD/[File] Stream bytes read", "stream_bytes_read"},
    {"FMOD/[File] Other bytes read", "other_bytes_read"},
};

FmodRuntime::FmodRuntime() {
    set_name(get_class_static());
    set_process_mode(PROCESS_MODE_ALWAYS);
    set_process_priority(std::numeric_limits<int32_t>::min());
}

void FmodRuntime::_enter_tree() {
    // FMOD only measures its performance in a running game, whose debugger sends the monitors to the editor.
    if (Engine::get_singleton()->is_editor_hint()) { return; }
    Performance* performance = Performance::get_singleton();
    for (const auto& monitor : PERFORMANCE_MONITORS) {
        if (performance->has_custom_monitor(monitor[0])) { continue; }
        Array arguments;
        arguments.push_back(StringName(monitor[1]));
        performance->add_custom_monitor(monitor[0], callable_mp(this, &FmodRuntime::get_performance_value), arguments);
    }
}

void FmodRuntime::_ready() {
    String godot_audio_bus = FmodServer::get_singleton()->get_godot_audio_bus();
    if (godot_audio_bus.is_empty()) { return; }
    Ref<FmodAudioStream> stream;
    stream.instantiate();
    AudioStreamPlayer* player = memnew(AudioStreamPlayer);
    player->set_stream(stream);
    player->set_bus(godot_audio_bus);
    add_child(player);
    player->play();
}

void FmodRuntime::_exit_tree() {
    Performance* performance = Performance::get_singleton();
    for (const auto& monitor : PERFORMANCE_MONITORS) {
        if (performance->has_custom_monitor(monitor[0])) { performance->remove_custom_monitor(monitor[0]); }
    }
}

void FmodRuntime::_process(double p_delta) {
    FmodServer::get_singleton()->update();
}

void FmodRuntime::_notification(int p_what) {
    // Godot exposes every node to screen readers, which would announce this internal one and its player.
    if (p_what == NOTIFICATION_ACCESSIBILITY_UPDATE) {
        DisplayServer::get_singleton()
            ->accessibility_update_set_flag(get_accessibility_element(), DisplayServer::FLAG_HIDDEN, true);
    }
    if (!OS::get_singleton()->has_feature("mobile")) { return; }
    if (p_what == NOTIFICATION_APPLICATION_FOCUS_OUT) { FmodServer::get_singleton()->mixer_suspend(); }
    if (p_what == NOTIFICATION_APPLICATION_FOCUS_IN) { FmodServer::get_singleton()->mixer_resume(); }
}

Variant FmodRuntime::get_performance_value(const StringName& p_property) const {
    return FmodServer::get_singleton()->get_performance_data()->get(p_property);
}

void FmodRuntime::_bind_methods() {}
