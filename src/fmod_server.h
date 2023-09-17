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
#include <resources/fmod_dsp_settings.h>
#include <resources/fmod_settings.h>
#include <resources/fmod_software_format_settings.h>
#include <resources/fmod_sound_3d_settings.h>

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

    struct Callback {
        FMOD_STUDIO_EVENT_CALLBACK_TYPE type;
        godot::Callable callable;
        Dictionary fmod_callback_properties;
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
        bool listenerWarning = true;
        Listener listeners[FMOD_MAX_LISTENERS];

        List<OneShot*> oneShots;
        List<Ref<FmodEvent>> runningEvents;

        Ref<FmodPerformanceData> performanceData;

        // Direct call from fmod thread cannot be made as we cannot interact with scene tree from another thread (thread
        // guard).
        // call_deferred is not implemented in godot-cpp.
        // Would have prefered a SpinLock but does not seems to exist in godot-cpp.
        // TODO: Change when https://github.com/godotengine/godot-cpp/pull/1091 is merged.
        Ref<Mutex> callback_mutex;
        List<Callback> callbacks_to_process;


        void _set_listener_attributes();

        Ref<FmodEventDescription> _get_event_description(const String& event_name);
        Ref<FmodEventDescription> _get_event_description(const FMOD_GUID& guid);

        template<bool is_guid = false>
        Ref<FmodEvent> _create_instance(const String& identifier, bool isOneShot, Node* gameObject);

        void _update_performance_data();

    public:
        FmodServer();

        ~FmodServer() override;

        static FmodServer* get_singleton();

        // LIFECYCLE
        void init(const Ref<FmodGeneralSettings>& p_settings);
        void update();
        void shutdown();

        // SETTINGS
        void set_software_format(const Ref<FmodSoftwareFormatSettings>& p_settings);
        void set_sound_3d_settings(const Ref<FmodSound3DSettings>& p_settings);
        void set_system_dsp_buffer_size(const Ref<FmodDspSettings>& p_settings);
        Ref<FmodDspSettings> get_system_dsp_buffer_settings();
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();

        // OBJECTS
        bool check_vca_guid(const String& guid);
        bool check_vca_path(const String& vcaPath);
        bool check_bus_guid(const String& guid);
        bool check_bus_path(const String& busPath);
        bool check_event_guid(const String& guid);
        bool check_event_path(const String& eventPath);
        Ref<FmodVCA> get_vca_from_guid(const String& guid);
        Ref<FmodVCA> get_vca(const String& vcaPath);
        Ref<FmodBus> get_bus_from_guid(const String& guid);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event_from_guid(const String& guid);
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

        // EVENTS
    private:
        template<bool is_guid = false>
        Ref<FmodEvent> _create_event_instance(const String& identifier);
    public:
        Ref<FmodEvent> create_event_instance_with_guid(const String& guid);
        Ref<FmodEvent> create_event_instance(const String& eventPath);

        // SOUNDS
        Ref<FmodFile> load_file_as_sound(const String& path);
        Ref<FmodFile> load_file_as_music(const String& path);
        void unload_file(const String& path);
        Ref<FmodSound> create_sound_instance(const String& path);

        //CALLBACKS
        void add_callback(const Callback& callback);

        /* Helper methods */
    private:
        template<bool is_guid, bool with_params, bool is_attached>
        void _play_one_shot(const String& identifier, Node* game_obj, const Dictionary& parameters = Dictionary());
    public:
        void play_one_shot_using_guid(const String& guid, Node* game_obj);
        void play_one_shot(const String& event_name, Node* game_obj);
        void play_one_shot_using_guid_with_params(const String& guid, Node* game_obj, const Dictionary& parameters);
        void play_one_shot_with_params(const String& event_name, Node* game_obj, const Dictionary& parameters);
        void play_one_shot_using_guid_attached(const String& guid, Node* game_obj);
        void play_one_shot_attached(const String& event_name, Node* game_obj);
        void play_one_shot_using_guid_attached_with_params(const String& guid, Node* game_obj, const Dictionary& parameters);
        void play_one_shot_attached_with_params(const String& event_name, Node* game_obj, const Dictionary& parameters);
        void pause_all_events();
        void unpause_all_events();
        void mute_all_events();
        void unmute_all_events();
        void wait_for_all_loads();

    protected:
        static void _bind_methods();
    };

    template<bool is_guid>
    Ref<FmodEvent> FmodServer::_create_instance(const String& identifier, bool isOneShot, Node* gameObject) {
        Ref<FmodEventDescription> desc;
        if (is_guid) {
            FMOD_GUID guid {string_to_fmod_guid(identifier)};
            desc = _get_event_description(guid);
        } else {
            desc = _get_event_description(identifier);
        }
        FMOD::Studio::EventInstance* eventInstance = nullptr;
        ERROR_CHECK(desc->get_wrapped()->createInstance(&eventInstance));
        Ref<FmodEvent> ref = FmodEvent::create_ref(eventInstance);
        if (ref.is_valid()) {
            runningEvents.push_back(ref);
            if (isOneShot || gameObject) {
                auto* oneShot = new OneShot();
                oneShot->gameObj = gameObject;
                oneShots.push_back(oneShot);
            }
        }

        return ref;
    }

    template<bool is_guid>
    Ref<FmodEvent> FmodServer::_create_event_instance(const String& identifier) {
        Ref<FmodEvent> ref = _create_instance<is_guid>(identifier, false, nullptr);
        ref->get_wrapped()->setUserData(ref.ptr());
        ref->set_distance_scale(distanceScale);
        return ref;
    }

    template<bool is_guid, bool with_params, bool is_attached>
    void FmodServer::_play_one_shot(const String& identifier, Node* game_obj, const Dictionary& parameters) {
        if (is_attached && !is_fmod_valid(game_obj)) { return; }

        Ref<FmodEvent> ref = _create_instance<is_guid>(
          identifier,
          true,
          is_attached ? game_obj : nullptr
        );
        if (!ref.is_valid()) { return; }

        if (!is_attached) { ref->set_node_attributes(game_obj); }

        if (with_params) {
            // set the initial parameter values
            Array keys = parameters.keys();
            for (int i = 0; i < keys.size(); ++i) {
                String k = keys[i];
                float v = parameters[keys[i]];
                ref->set_parameter_by_name(k.utf8().get_data(), v);
            }
        }

        ref->start();
        ref->release();
    }
}// namespace godot

#endif// GODOTFMOD_FMOD_SERVER_H
