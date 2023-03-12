#ifndef GODOTFMOD_FMODEVENT_H
#define GODOTFMOD_FMODEVENT_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodEvent : public RefCounted {
        GDCLASS(FmodEvent, RefCounted);

        FMOD::Studio::EventInstance* event;

        float get_event_parameter_by_name(uint64_t instanceId, const String& parameterName);
        void set_event_parameter_by_name(uint64_t instanceId, const String& parameterName, float value);
        float get_event_parameter_by_id(uint64_t instanceId, const Array& idPair);
        void set_event_parameter_by_id(uint64_t instanceId, const Array& idPair, float value);
        void release_event(uint64_t instanceId);
        void start_event(uint64_t instanceId);
        void stop_event(uint64_t instanceId, int stopMode);
        void event_key_off(uint64_t instanceId);
        int get_event_playback_state(uint64_t instanceId);
        bool get_event_paused(uint64_t instanceId);
        void set_event_paused(uint64_t instanceId, bool paused);
        float get_event_pitch(uint64_t instanceId);
        void set_event_pitch(uint64_t instanceId, float pitch);
        float get_event_volume(uint64_t instanceId);
        void set_event_volume(uint64_t instanceId, float volume);
        int get_event_timeline_position(uint64_t instanceId);
        void set_event_timeline_position(uint64_t instanceId, int position);
        float get_event_reverb_level(uint64_t instanceId, int index);
        void set_event_reverb_level(uint64_t instanceId, int index, float level);
        bool is_event_virtual(uint64_t instanceId);
        void set_event_listener_mask(uint64_t instanceId, unsigned int mask);
        uint32_t get_event_listener_mask(uint64_t instanceId);
        void set_event_3d_attributes(uint64_t instanceId, const Transform3D& transform);
        Dictionary get_event_3d_attributes(uint64_t instanceId);
        Dictionary get_event_2d_attributes(uint64_t instanceId);
        void set_event_2d_attributes(uint64_t instanceId, Transform2D position);

        void attach_instance_to_node(uint64_t instanceId, Object* gameObj);
        void detach_instance_from_node(const uint64_t instanceId);
        Object* get_object_attached_to_instance(uint64_t instanceId);


    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMODEVENT_H
