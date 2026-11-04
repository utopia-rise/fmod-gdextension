#ifndef GODOTFMOD_FMOD_EVENT_H
#define GODOTFMOD_FMOD_EVENT_H

#include "classes/ref_counted.hpp"
#include "core/binder_common.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"
#include "core/fmod_channel_group.h"

namespace godot {
    class FmodEvent : public RefCounted {
        FMODCLASS(FmodEvent, RefCounted, FMOD::Studio::EventInstance);

        Callable eventCallback;
        String programmers_callback_sound_key;
        float distanceScale = 1.0f;
        uint32_t callback_mask;

    public:
        enum StudioStopMode {
            ALLOWFADEOUT = FMOD_STUDIO_STOP_ALLOWFADEOUT,
            IMMEDIATE = FMOD_STUDIO_STOP_IMMEDIATE,
        };

        enum StudioPlaybackState {
            PLAYING = FMOD_STUDIO_PLAYBACK_PLAYING,
            SUSTAINING = FMOD_STUDIO_PLAYBACK_SUSTAINING,
            PLAYBACK_STOPPED = FMOD_STUDIO_PLAYBACK_STOPPED,
            PLAYBACK_STARTING = FMOD_STUDIO_PLAYBACK_STARTING,
            STOPPING = FMOD_STUDIO_PLAYBACK_STOPPING,
        };

        enum StudioEventCallbackType : uint32_t {
            CREATED = FMOD_STUDIO_EVENT_CALLBACK_CREATED,
            DESTROYED = FMOD_STUDIO_EVENT_CALLBACK_DESTROYED,
            CALLBACK_STARTING = FMOD_STUDIO_EVENT_CALLBACK_STARTING,
            STARTED = FMOD_STUDIO_EVENT_CALLBACK_STARTED,
            RESTARTED = FMOD_STUDIO_EVENT_CALLBACK_RESTARTED,
            CALLBACK_STOPPED = FMOD_STUDIO_EVENT_CALLBACK_STOPPED,
            START_FAILED = FMOD_STUDIO_EVENT_CALLBACK_START_FAILED,
            CREATE_PROGRAMMER_SOUND = FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND,
            DESTROY_PROGRAMMER_SOUND = FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND,
            PLUGIN_CREATED = FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_CREATED,
            PLUGIN_DESTROYED = FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED,
            TIMELINE_MARKER = FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER,
            TIMELINE_BEAT = FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT,
            SOUND_PLAYED = FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED,
            SOUND_STOPPED = FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED,
            REAL_TO_VIRTUAL = FMOD_STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL,
            VIRTUAL_TO_REAL = FMOD_STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL,
            START_EVENT_COMMAND = FMOD_STUDIO_EVENT_CALLBACK_START_EVENT_COMMAND,
            NESTED_TIMELINE_BEAT = FMOD_STUDIO_EVENT_CALLBACK_NESTED_TIMELINE_BEAT,
            ALL = FMOD_STUDIO_EVENT_CALLBACK_ALL,
        };

        FmodEvent() = default;
        ~FmodEvent() override;

        float get_parameter_by_name(const String& parameter_name) const;
        void set_parameter_by_name(const String& parameter_name, float value) const;
        void set_parameter_by_name_with_label(const String& parameter_name, const String& label, bool ignoreseekspeed = false) const;
        float get_parameter_by_id(uint64_t long_id) const;
        float get_parameter_by_fmod_id(const FMOD_STUDIO_PARAMETER_ID& parameter_id) const;
        float get_parameter_user_value_by_name(const String& parameter_name) const;
        float get_parameter_user_value_by_id(uint64_t long_id) const;
        void set_parameter_by_id(uint64_t long_id, float value) const;
        void set_parameter_by_fmod_id(const FMOD_STUDIO_PARAMETER_ID& parameter_id, float value) const;
        void set_parameter_by_fmod_id_with_label(const FMOD_STUDIO_PARAMETER_ID& parameter_id, const String& label, bool ignoreseekspeed = false) const;
        void set_parameter_by_id_with_label(uint64_t parameter_id, const String& label, bool ignoreseekspeed = false) const;
        void release() const;
        void start() const;
        void stop(StudioStopMode stop_mode) const;
        void event_key_off() const;
        StudioPlaybackState get_playback_state() const;
        bool get_paused() const;
        void set_paused(bool paused) const;
        float get_pitch() const;
        void set_pitch(float pitch) const;
        float get_volume();
        void set_volume(float volume) const;
        int get_timeline_position() const;
        void set_timeline_position(int position) const;
        float get_reverb_level(int index) const;
        void set_reverb_level(int index, float level) const;
        bool is_virtual() const;
        void set_listener_mask(unsigned int mask) const;
        uint32_t get_listener_mask() const;
        Transform3D get_3d_attributes() const;
        Transform2D get_2d_attributes() const;
        void set_2d_attributes(const Transform2D& position) const;
        void set_3d_attributes(const Transform3D& transform) const;
        void set_node_attributes(Node* node) const;
        void set_callback(const Callable& callback, BitField<StudioEventCallbackType> p_callback_mask);
        const Callable& get_callback() const;
        void set_programmer_callback(const String& p_programmers_callback_sound_key);
        const String& get_programmers_callback_sound_key() const;
        void set_distance_scale(float scale);
        Ref<FmodChannelGroup> get_channel_group() const;

    protected:
        static void _bind_methods();
    };
}// namespace godot

VARIANT_BITFIELD_CAST(FmodEvent::StudioEventCallbackType);
VARIANT_ENUM_CAST(FmodEvent::StudioStopMode);
VARIANT_ENUM_CAST(FmodEvent::StudioPlaybackState);

#endif// GODOTFMOD_FMOD_EVENT_H
