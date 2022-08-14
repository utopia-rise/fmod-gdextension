#ifndef FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
#define FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

    class FmodEventEmitter2D : public Node2D {
    GDCLASS(FmodEventEmitter2D, Node2D)

    protected:
        static void _bind_methods();

    private:
        String event_name;
        uint64_t event_id = -1;
        bool attached = true;
        bool autoplay = false;
        bool looped = false;
        bool allow_fadeout = true;
        bool preload_event = true;

        Dictionary params;
        
        void _unpause();
        void _play_one_shot();
        void _play_looped();
        void _set_param_internally(const String &key, const float &value);

    public:
        FmodEventEmitter2D();

        ~FmodEventEmitter2D();

        void _ready() override;

        void set_param(const String &key, const float &value);
        bool is_paused() const;
        void play();
        void pause();
        
        void set_event_name(const String &name);
        String get_event_name() const;
        void set_attached(const bool &attached);
        bool is_attached() const;
        void set_autoplay(const bool &autoplay);
        bool is_autoplay() const;
        void set_looped(const bool &looped);
        bool is_looped() const;
        void set_allow_fadeout(const bool &allow_fadeout);
        bool is_allow_fadeout() const;
        void set_preload_event(const bool &preload_event);
        bool is_preload_event() const;
    };
}

#endif //FMOD_EVENT_EMITTER_2D_GODOT_FMOD_H
