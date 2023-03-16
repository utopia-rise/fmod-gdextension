#ifndef GODOTFMOD_FMOD_EVENT_H
#define GODOTFMOD_FMOD_EVENT_H


namespace godot {
    class FmodEvent : public FmodObject<FmodEvent, FMOD::Studio::EventInstance>  {

        float distanceScale = 0;
        Callable eventCallback;

        FMOD::Studio::EventInstance* wrapped = nullptr;

    public:
        FmodEvent() = default;
        ~FmodEvent() = default;

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
        void set_3d_attributes(const Transform3D& transform) const;
        Transform3D get_3d_attributes() const;
        Transform2D get_2d_attributes() const;
        void set_2d_attributes(Transform2D position) const;

        void set_callback(Callable& callback, int callbackMask);
        Callable get_callback() const;
    };
}// namespace godot
#endif// GODOTFMOD_FMOD_EVENT_H
