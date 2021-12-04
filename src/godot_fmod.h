#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <Spatial.hpp>
#include <Object.hpp>
#include <CanvasItem.hpp>
#include <Node.hpp>
#include <gen/Mutex.hpp>
#include "callback/event_callbacks.h"
#include "callback/file_callbacks.h"
#include "helpers/containers.h"
#include "helpers/constants.h"
#include "helpers/current_function.h"

#define CUSTOM_FILESYSTEM

#ifndef CUSTOM_FILESYSTEM
    #ifdef __ANDROID__
        #define DRIVE_PATH(path)\
path = path.replace("res://", "file:///android_asset/");
    #else
        #define DRIVE_PATH(path)\
path = path.replace("res://", "./");
    #endif
#else
    #define DRIVE_PATH(path)
#endif

#define MAX_PATH_SIZE 512
#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_INSTANCE 128
#define MAX_EVENT_COUNT 256
#define MAX_DRIVER_NAME_SIZE 256

namespace godot {

#define GODOT_LOG(level, message)\
    switch (level) {\
        case 0:\
            Godot::print(message);\
            break;\
        case 1:\
            Godot::print_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);\
            break;\
        case 2:\
            Godot::print_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);\
            break;\
    }\

#define FIND_AND_CHECK_WITH_RETURN(instanceId, cont, defaultReturn) \
    auto instance = (cont).get(instanceId); \
    if (!instance) { \
        String message = String("FMOD Sound System: cannot find " + String(instanceId) + " in ##cont collection.");\
        GODOT_LOG(2, message)\
        return defaultReturn; \
    }
#define FIND_AND_CHECK_WITHOUT_RETURN(instanceId, set) FIND_AND_CHECK_WITH_RETURN(instanceId, set, void())
#define FUNC_CHOOSER(_f1, _f2, _f3, _f4, ...) _f4
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define MACRO_CHOOSER(...) FUNC_RECOMPOSER((__VA_ARGS__, FIND_AND_CHECK_WITH_RETURN, FIND_AND_CHECK_WITHOUT_RETURN, ))
#define FIND_AND_CHECK(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CHECK_SIZE(maxSize, actualSize, type) \
    if((actualSize) > (maxSize)){\
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " + String::num(actualSize) + " entries";\
        GODOT_LOG(2, message)\
        (actualSize) = maxSize;\
    }\

    struct EventInfo {
        //Is the event oneshot
        bool isOneShot = false;
        //GameObject to which this event is attached
        Node* gameObj = nullptr;
        // Callback info associated with this event
        Callbacks::CallbackInfo callbackInfo = Callbacks::CallbackInfo();
    };

    struct LoadingBank {
        FMOD::Studio::Bank*  bank;
        String godotPath;
    };

    struct Listener {
        Node* gameObj = nullptr;
        bool listenerLock = false;
        float weight = 1.0;
    };

    class Fmod : public Node {
    GODOT_CLASS(Fmod, Node)
        DECLARE_ALL_CONSTANTS

    private:
        FMOD::Studio::System *system;
        FMOD::System *coreSystem;

        bool isInitialized;
        bool isNotinitPrinted;

        float distanceScale;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        Listener listeners[FMOD_MAX_LISTENERS];
        bool listenerWarning = true;

        Vector<LoadingBank* > loadingBanks;
        Map<String, FMOD::Studio::Bank*> banks;
        Map<String, FMOD::Studio::EventDescription*> eventDescriptions;
        Map<String, FMOD::Sound*> sounds;
        Map<String, FMOD::Studio::Bus*> buses;
        Map<String, FMOD::Studio::VCA*> VCAs;

        Vector<FMOD::Channel*> channels;
        Vector<FMOD::Studio::EventInstance*> events;

        //Store dictionary of performance data
        Dictionary performanceData;

        void checkLoadingBanks();
        void setListenerAttributes();

        static bool checkErrors(FMOD_RESULT result, const char *function, const char *file, int line);
#define ERROR_CHECK(_result) checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

        static FMOD_VECTOR toFmodVector(Vector3 &vec);
        static FMOD_3D_ATTRIBUTES get3DAttributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward,
                                                  const FMOD_VECTOR& vel);
        FMOD_3D_ATTRIBUTES get3DAttributesFromTransform(const Transform& transform) const;
        FMOD_3D_ATTRIBUTES get3DAttributesFromTransform2D(const Transform2D& transform) const;
        Dictionary getTransformInfoFrom3DAttribut(FMOD_3D_ATTRIBUTES& attribut) const;
        Dictionary getTransform2DInfoFrom3DAttribut(FMOD_3D_ATTRIBUTES& attribut) const;

        static bool isDead(Node* node);
        static bool isFmodValid(Node* node);
        void updateInstance3DAttributes(FMOD::Studio::EventInstance* instance, Node* node);
        void runCallbacks();

        FMOD::Studio::EventInstance* createInstance(const String& eventName, bool isOneShot, Node* gameObject);
        static EventInfo *getEventInfo(FMOD::Studio::EventInstance* eventInstance);
        void releaseOneEvent(FMOD::Studio::EventInstance* eventInstance);
        void loadBankData(LoadingBank* loadingBank);
        void loadAllVCAs(FMOD::Studio::Bank* bank);
        void loadAllBuses(FMOD::Studio::Bank* bank);
        void loadAllEventDescriptions(FMOD::Studio::Bank* bank);
        void unloadAllVCAs(FMOD::Studio::Bank* bank);
        void unloadAllBuses(FMOD::Studio::Bank* bank);
        void unloadAllEventDescriptions(FMOD::Studio::Bank* bank);
        static bool isChannelValid(FMOD::Channel* channel);

    public:
        Fmod();

        ~Fmod();

        static void _register_methods();

        void _init();
        void _process(float delta);
        void shutdown();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale);
        void setSoftwareFormat(int sampleRate, int speakerMode, int numRawSpeakers);

        void addListener(int index, Node* gameObj);
        void removeListener(int index);
        void setListenerNumber(int listenerNumber);
        int getSystemNumListeners() const;
        float getSystemListenerWeight(int index);
        void setSystemListenerWeight(int index, float weight);
        Dictionary getSystemListener3DAttributes(int index);
        Dictionary getSystemListener2DAttributes(int index);
        void setSystemListener3DAttributes(int index, const Transform& transform);
        void setSystemListener2DAttributes(int index, const Transform2D& transform);
        void setListenerLock(int index, bool isLocked);
        bool getListenerLock(int index);
        Node* getObjectAttachedToListener(int index);
        void setSystemDSPBufferSize(unsigned int bufferlength, int numbuffers);
        unsigned int getSystemDSPBufferLength();
        int getSystemDSPNumBuffers();
        Array getSystemDSPBufferSize();

        String loadBank(const String& pathToBank, unsigned int flag);
        void unloadBank(const String& pathToBank);
        bool checkVCAPath(const String& vcaPath);
        bool checkBusPath(const String& busPath);
        bool checkEventPath(const String& eventPath);
        int getBankLoadingState(const String& pathToBank);
        int getBankBusCount(const String& pathToBank);
        int getBankEventCount(const String& pathToBank);
        int getBankStringCount(const String& pathToBank);
        int getBankVCACount(const String& pathToBank);
        uint64_t createEventInstance(const String& eventPath);
        float getEventParameterByName(uint64_t instanceId, const String& parameterName);
        void setEventParameterByName(uint64_t instanceId, const String& parameterName, float value);
        float getEventParameterByID(uint64_t instanceId, const Array& idPair);
        void setEventParameterByID(uint64_t instanceId, const Array& idPair, float value);
        void releaseEvent(uint64_t instanceId);
        void startEvent(uint64_t instanceId);
        void stopEvent(uint64_t instanceId, int stopMode);
        void eventKeyOff(uint64_t instanceId);
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
        void setEventListenerMask(uint64_t instanceId, unsigned int mask);
        uint32_t getEventListenerMask(uint64_t instanceId);
        void setEvent3DAttributes(uint64_t instanceId, const Transform& transform);
        Dictionary getEvent3DAttributes(uint64_t instanceId);
        Dictionary getEvent2DAttributes(uint64_t instanceId);
        void setEvent2DAttributes(uint64_t instanceId, Transform2D position);

        /* event descriptions functions */
        int descGetLength(const String& eventPath);
        Array descGetInstanceList(const String& eventPath);
        int descGetInstanceCount(const String& eventPath);
        void descReleaseAllInstances(const String& eventPath);
        void descLoadSampleData(const String& eventPath);
        void descUnloadSampleData(const String& eventPath);
        int descGetSampleLoadingState(const String& eventPath);
        bool descIs3D(const String& eventPath);
        bool descIsOneShot(const String& eventPath);
        bool descIsSnapshot(const String& eventPath);
        bool descIsStream(const String& eventPath);
        bool descHasSustainPoint(const String& eventPath);
        Array descGetMinMaxDistance(const String& eventPath);
        float descGetSoundSize(const String& eventPath);
        Dictionary descGetParameterDescriptionByName(const String& eventPath, const String& name);
        Dictionary descGetParameterDescriptionByID(const String& eventPath, const Array& idPair);
        int descGetParameterDescriptionCount(const String& eventPath);
        Dictionary descGetParameterDescriptionByIndex(const String& eventPath, int index);
        Dictionary descGetUserProperty(const String& eventPath, const String& name);
        int descGetUserPropertyCount(const String& eventPath);
        Dictionary descUserPropertyByIndex(const String& eventPath, int index);

        /* bus functions */
        bool getBusMute(const String& busPath);
        bool getBusPaused(const String& busPath);
        float getBusVolume(const String& busPath);
        void setBusMute(const String& busPath, bool mute);
        void setBusPaused(const String& busPath, bool paused);
        void setBusVolume(const String& busPath, float volume);
        void stopAllBusEvents(const String& busPath, int stopMode);

        /* VCA functions */
        float getVCAVolume(const String& VCAPath);
        void setVCAVolume(const String& VCAPath, float volume);
        /* Helper methods */
        void playOneShot(const String& eventName, Node* gameObj);
        void playOneShotWithParams(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void playOneShotAttached(const String& eventName, Node* gameObj);
        void playOneShotAttachedWithParams(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void attachInstanceToNode(uint64_t instanceId, Node* gameObj);
        void detachInstanceFromNode(uint64_t instanceId);
        Node* getObjectAttachedToInstance(uint64_t instanceId);
        void pauseAllEvents(bool pause);
        void muteAllEvents();
        void unmuteAllEvents();
        bool banksStillLoading();


        //LOW LEVEL API
        //Load and release memory
        void loadFileAsSound(const String& path);
        void loadFileAsMusic(const String& path);
        void unloadFile(const String& path);
        //Check validity of an instance
        uint64_t createSoundInstance(const String& path);
        bool checkSoundInstance(uint64_t instanceId);
        void releaseSound(uint64_t instanceId);
        //Setting the sound
        void setSoundPaused(uint64_t instanceId, bool paused);
        void stopSound(uint64_t instanceId);
        bool isSoundPlaying(uint64_t instanceId);
        void setSoundVolume(uint64_t instanceId, float volume);
        float getSoundVolume(uint64_t instanceId);
        float getSoundPitch(uint64_t instanceId);
        void setSoundPitch(uint64_t instanceId, float pitch);
        //Playing a sound
        void playSound(uint64_t instanceId);

        //MISC
        void waitForAllLoads();
        Array getAvailableDrivers();
        int getDriver();
        void setDriver(int id);
        Dictionary getPerformanceData();
        void setGlobalParameterByName(const String& parameterName, float value);
        float getGlobalParameterByName(const String& parameterName);
        void setGlobalParameterByID(const Array& idPair, float value);
        float getGlobalParameterByID(const Array& idPair);
        Dictionary getGlobalParameterDescByName(const String& parameterName);
        Dictionary getGlobalParameterDescByID(const Array& idPair);
        int getGlobalParameterDescCount();
        Array getGlobalParameterDescList();

        void setCallback(uint64_t instanceId, int callbackMask);
    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
