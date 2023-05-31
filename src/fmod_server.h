#ifndef GODOTFMOD_FMOD_SERVER_H
#define GODOTFMOD_FMOD_SERVER_H

#include "core/fmod_file.h"
#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "fmod_cache.h"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"
#include "variant/string.hpp"

#include <callback/event_callbacks.h>
#include <callback/file_callbacks.h>
#include <helpers/constants.h>

#include <classes/canvas_item.hpp>
#include <classes/global_constants.hpp>
#include <classes/mutex.hpp>
#include <classes/object.hpp>
#include <core/object.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot.hpp>
#include <variant/utility_functions.hpp>

namespace godot {

    struct OneShot {
        Node* gameObj = nullptr;
        Ref<FmodEvent> instance;
    };

    struct Listener {
        Object* gameObj = nullptr;
        bool listenerLock = false;
        float weight = 1.0;
    };

    class FmodServer : public Object {
        GDCLASS(FmodServer, Object);

        static FmodServer* singleton;

    private:
        FMOD::Studio::System* system;
        FMOD::System* coreSystem;

        bool isInitialized;
        bool isNotInitializedPrinted;

        float distanceScale;

        FmodCache* cache;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        Listener listeners[FMOD_MAX_LISTENERS];
        bool listenerWarning = true;

        List<OneShot*> oneShots;
        List<Ref<FmodEvent>> runningEvents;

        Ref<FmodPerformanceData> performanceData;

        void _set_listener_attributes();

        Ref<FmodEvent> _create_instance(const String& eventName, bool isOneShot, Node* gameObject);

        void _update_performance_data();

    public:
        FmodServer();

        ~FmodServer() override;

        static FmodServer* get_singleton();

        // LIFECYCLE
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void update();
        void shutdown();

        // SETTINGS
        void set_software_format(int sampleRate, int speakerMode, int numRawSpeakers);
        void set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale);
        void set_system_dsp_buffer_size(unsigned int bufferLength, int numBuffers);
        Array get_system_dsp_buffer_size();
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();

        // OBJECTS
        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);
        Ref<FmodVCA> get_vca(const String& vcaPath);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event(const String& eventPath);
        Array get_all_vca();
        Array get_all_buses();
        Array get_all_event_descriptions();
        Array get_all_banks();

        // DEBUGGING
        Array get_available_drivers();
        int get_driver();
        void set_driver(int id);
        Ref<FmodPerformanceData> get_performance_data();

        // GLOBAL PARAMETERS
        void set_global_parameter_by_name(const String& parameterName, float value);
        float get_global_parameter_by_name(const String& parameterName);
        void set_global_parameter_by_id(const Array& idPair, float value);
        float get_global_parameter_by_id(const Array& idPair);
        Dictionary get_global_parameter_desc_by_name(const String& parameterName);
        Dictionary get_global_parameter_desc_by_id(const Array& idPair);
        int get_global_parameter_desc_count();
        Array get_global_parameter_desc_list();

        // LISTENERS
        void add_listener(int index, Object* gameObj);
        void remove_listener(int index);
        void set_system_listener_number(int listenerNumber);
        int get_system_listener_number() const;
        float get_system_listener_weight(int index);
        void set_system_listener_weight(int index, float weight);
        Transform3D get_listener_transform3d(int index);
        Transform2D get_listener_transform2d(int index);
        Vector3 get_listener_3d_velocity(int index);
        Vector2 get_listener_2d_velocity(int index);
        void set_listener_transform3d(int index, const Transform3D& transform);
        void set_listener_transform2d(int index, const Transform2D& transform);
        void set_listener_lock(int index, bool isLocked);
        bool get_listener_lock(int index);
        Object* get_object_attached_to_listener(int index);

        // BANKS
        Ref<FmodBank> load_bank(const String& pathToBank, unsigned int flag);
        void unload_bank(const String& pathToBank);
        bool banks_still_loading();
        Ref<FmodEvent> create_event_instance(const String& eventPath);

        // SOUNDS
        Ref<FmodFile> load_file_as_sound(const String& path);
        Ref<FmodFile> load_file_as_music(const String& path);
        void unload_file(const String& path);
        Ref<FmodSound> create_sound_instance(const String& path);

        /* Helper methods */
        void play_one_shot(const String& eventName, Node* gameObj);
        void play_one_shot_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void play_one_shot_attached(const String& eventName, Node* gameObj);
        void play_one_shot_attached_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void pause_all_events();
        void unpause_all_events();
        void mute_all_events();
        void unmute_all_events();
        void wait_for_all_loads();

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SERVER_H
