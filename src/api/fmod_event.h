#ifndef GODOTFMOD_FMOD_EVENT_H
#define GODOTFMOD_FMOD_EVENT_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"

namespace godot {
    class FmodEvent : public RefCounted {
        GDCLASS(FmodEvent, RefCounted);

        FMOD::Studio::EventInstance* event;

        float get_event_parameter_by_name(const String& parameterName) const;
        void set_event_parameter_by_name(const String& parameterName, float value) const;
        float get_event_parameter_by_id(const Array& idPair) const;
        void set_event_parameter_by_id(const Array& idPair, float value) const;
        void release_event() const;
        void start_event() const;
        void stop_event(int stopMode) const;
        void event_key_off() const;
        int get_event_playback_state() const;
        bool get_event_paused() const;
        void set_event_paused(bool paused) const;
        float get_event_pitch() const;
        void set_event_pitch(float pitch) const;
        float get_event_volume();
        void set_event_volume(float volume) const;
        int get_event_timeline_position() const;
        void set_event_timeline_position(int position) const;
        float get_event_reverb_level(int index) const;
        void set_event_reverb_level(int index, float level) const;
        bool is_event_virtual() const;
        void set_event_listener_mask(unsigned int mask) const;
        uint32_t get_event_listener_mask() const;
        void set_event_3d_attributes(const Transform3D& transform) const;
        Dictionary get_event_3d_attributes() const;
        Dictionary get_event_2d_attributes() const;
        void set_event_2d_attributes(Transform2D position) const;

        void attach_instance_to_node(Object* gameObj) const;
        void detach_instance_from_node() const;
        Object* get_object_attached_to_instance() const;

    public:
        FmodEvent() = default;
        ~FmodEvent() = default;

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_H
