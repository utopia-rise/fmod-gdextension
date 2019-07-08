#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

#include <Godot.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <Spatial.hpp>
#include <Object.hpp>
#include <CanvasItem.hpp>
#include <Node.hpp>
#include <gen/Mutex.hpp>
#include "callbacks.h"
#include "current_function.h"


#define FIND_AND_CHECK_WITH_RETURN(instanceId, type, coll, defaultReturn) \
if (!coll.has(instanceId)) return defaultReturn; \
auto instance = (type) (uint64_t) coll[instanceId]; \
if (!instance) { \
    Godot::print_error("FMOD Sound System: cannot find instanceId in coll", BOOST_CURRENT_FUNCTION, __FILE__, __LINE__); \
    return defaultReturn; \
} \

#define FIND_AND_CHECK_WITHOUT_RETURN(instanceId, type, coll) FIND_AND_CHECK_WITH_RETURN(instanceId, type, coll, void())

#define CHOOSE_FROM_ARG_COUNT(...) FUNC_RECOMPOSER((__VA_ARGS__, FIND_AND_CHECK_WITH_RETURN, FIND_AND_CHECK_WITHOUT_RETURN, ))
#define FUNC_CHOOSER(_f1, _f2, _f3, _f4, _f5, ...) _f5
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define MACRO_CHOOSER(...) CHOOSE_FROM_ARG_COUNT(__VA_ARGS__ ())
#define FIND_AND_CHECK(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

namespace godot {

    struct EventInfo {
        //Is the event oneshot
        bool isOneShot = false;
        //GameObject to which this event is attached
        Object *gameObj = nullptr;
        // Callback info associated with this event
        Callbacks::CallbackInfo callbackInfo = Callbacks::CallbackInfo();
    };

    struct SoundPair {
        FMOD::Sound *sound;
        FMOD::Channel *channel;
    };

    class Fmod : public Node {

    private:
        FMOD::Studio::System *system;
        FMOD::System *coreSystem;

        bool isInitialized;
        bool isNotinitPrinted;

        float distanceScale;

        Object *listener;

        bool nullListenerWarning = true;

        Dictionary banks;
        Dictionary eventDescriptions;
        Dictionary buses;
        Dictionary VCAs;

        Array sounds;
        Array events;

        //Store disctionnary of performance data
        Dictionary performanceData;

    GODOT_CLASS(Fmod, Node)

    private:
        int checkErrors(FMOD_RESULT result);
        void setListenerAttributes();
        FMOD_VECTOR toFmodVector(Vector3 &vec);
        FMOD_3D_ATTRIBUTES get3DAttributes(const FMOD_VECTOR &pos, const FMOD_VECTOR &up, const FMOD_VECTOR &forward,
                                           const FMOD_VECTOR &vel);
        bool isNull(Object *o);
        void loadBus(const String &busPath);
        void loadVCA(const String &VCAPath);
        void updateInstance3DAttributes(FMOD::Studio::EventInstance *instance, Object *o);
        void runCallbacks();

        FMOD::Studio::EventInstance *createInstance(String eventName, bool isOneShot, Object *gameObject);
        EventInfo *getEventInfo(FMOD::Studio::EventInstance *eventInstance);
        void releaseOneEvent(FMOD::Studio::EventInstance *eventInstance);

    public:
        Fmod();

        ~Fmod();

        static void _register_methods();
        void _init();
        void update();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale);
        void shutdown();
        void addListener(Object *gameObj);
        void setSoftwareFormat(int sampleRate, int speakerMode, int numRawSpeakers);
        String loadbank(String pathToBank, unsigned int flag);
        void unloadBank(String pathToBank);
        int getBankLoadingState(String pathToBank);
        int getBankBusCount(String pathToBank);
        int getBankEventCount(String pathToBank);
        int getBankStringCount(String pathToBank);
        int getBankVCACount(String pathToBank);
        const int createEventInstance(String eventPath);
        float getEventParameter(uint64_t instanceId, String parameterName);
        void setEventParameter(uint64_t instanceId, String parameterName, float value);
        void releaseEvent(uint64_t instanceId);
        void startEvent(uint64_t instanceId);
        void stopEvent(uint64_t instanceId, int stopMode);
        void triggerEventCue(uint64_t instanceId);
        int getEventPlaybackState(uint64_t instanceId);
        bool getEventPaused(uint64_t instanceId);
        void setEventPaused(uint64_t instanceId, bool paused);
        float getEventPitch(uint64_t instanceId);
        void setEventPitch(uint64_t instanceId, float pitch);
        float getEventVolume(uint64_t instanceId);
        void setEventVolume(uint64_t instanceId, float volume);
        int getEventTimelinePosition(uint64_t instanceId);
        void setEventTimelinePosition(uint64_t instanceId, int position);
        float getEventReverbLevel(uint64_t instanceId, int index);
        void setEventReverbLevel(uint64_t instanceId, int index, float level);
        bool isEventVirtual(uint64_t instanceId);

        /* bus functions */
        bool getBusMute(String busPath);
        bool getBusPaused(String busPath);
        float getBusVolume(String busPath);
        void setBusMute(String busPath, bool mute);
        void setBusPaused(String busPath, bool paused);
        void setBusVolume(String busPath, float volume);
        void stopAllBusEvents(String busPath, int stopMode);

        /* VCA functions */
        float getVCAVolume(String VCAPath);
        void setVCAVolume(String VCAPath, float volume);
        /* Helper methods */
        void playOneShot(String eventName, Object *gameObj);
        void playOneShotWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void playOneShotAttached(String eventName, Object *gameObj);
        void playOneShotAttachedWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void attachInstanceToNode(uint64_t instanceId, Object *gameObj);
        void detachInstanceFromNode(uint64_t instanceId);
        void pauseAllEvents(bool pause);
        void muteAllEvents();
        void unmuteAllEvents();
        bool banksStillLoading();

        void playSound(int instanceId);
        const int loadSound(String path, int mode);
        void releaseSound(uint64_t instanceId);
        void setSoundPaused(uint64_t instanceId, bool paused);
        void stopSound(uint64_t instanceId);
        bool isSoundPlaying(uint64_t instanceId);
        void setSoundVolume(uint64_t instanceId, float volume);
        float getSoundVolume(uint64_t instanceId);
        float getSoundPitch(uint64_t instanceId);
        void setSoundPitch(uint64_t instanceId, float pitch);
        void waitForAllLoads();

        Array getAvailableDrivers();
        int getDriver();
        void setDriver(int id);
        Dictionary getPerformanceData();
        void setGlobalParameter(String parameterName, float value);
        float getGlobalParameter(String parameterName);

        void setCallback(uint64_t instanceId, int callbackMask);
    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
