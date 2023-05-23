#ifndef GODOTFMOD_FMOD_EVENT_H
#define GODOTFMOD_FMOD_EVENT_H

#include "classes/ref_counted.hpp"
#include "fmod_studio.hpp"
#include "helpers/common.h"

namespace godot {
    class FmodEvent : public RefCounted {
        FMODCLASS(FmodEvent, RefCounted, FMOD::Studio::EventInstance);

        float distanceScale = 1.0f;
        Callable eventCallback;

    public:
        FmodEvent() = default;
        ~FmodEvent() override = default;

        float get_parameter_by_name(const String& parameterName) const;
        void set_parameter_by_name(const String& parameterName, float value) const;
        float get_parameter_by_id(const Array& idPair) const;
        void set_parameter_by_id(const Array& idPair, float value) const;
        void release() const;
        void start() const;
        void stop(int stopMode) const;
        void event_key_off() const;
        int get_playback_state() const;
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
        void set_callback(const Callable& callback, int callbackMask);
        const Callable& get_callback() const;
        void set_distance_scale(float scale);

    protected:
        static void _bind_methods();
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_H
