//
// Created by ptmeisels on 27/02/19.
//

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
#include <vector>
#include <map>

namespace godot {
    class GodotFmod : public Node {
        FMOD::Studio::System *system;
        FMOD::System *lowLevelSystem;

        Object *listener;

        bool nullListenerWarning = true;

        std::map<String, FMOD::Studio::Bank *> banks;
        std::map<String, FMOD::Studio::EventDescription *> eventDescriptions;
        std::map<String, FMOD::Studio::Bus *> buses;
        std::map<String, FMOD::Studio::VCA *> VCAs;
        std::map<String, FMOD::Sound *> sounds;
        std::map<FMOD::Sound *, FMOD::Channel *> channels;

        std::map<std::string, FMOD_INITFLAGS> fmodInitFlags;
        std::map<std::string, FMOD_STUDIO_INITFLAGS > fmodStudioInitFlags;
        std::map<std::string, FMOD_SPEAKERMODE> fmodSpeakerModeFlags;
        std::map<std::string, FMOD_STUDIO_LOAD_BANK_FLAGS> fmodLoadBankFlags;
        std::map<std::string, int > fmodSoundConstants;
        std::map<std::string, FMOD_STUDIO_STOP_MODE> fmodStudioStopModes;

        // keep track of one shot instances internally
        std::vector<FMOD::Studio::EventInstance *> oneShotInstances;
        struct AttachedOneShot {
            FMOD::Studio::EventInstance *instance;
            Object *gameObj;
        };
        std::vector<AttachedOneShot> attachedOneShots;

        // events not directly managed by the integration
        // referenced through uuids generated in script
        std::map<String, FMOD::Studio::EventInstance *> unmanagedEvents;

    GODOT_CLASS(GodotFmod, Node)

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

    public:
        GodotFmod();

        ~GodotFmod();

        static void _register_methods();
        void _init();
        void init(int numOfChannels, String studioFlags, String flags);
        void update();
        void shutdown();
        void addListener(Object *gameObj);
        void setSoftwareFormat(int sampleRate, const String& speakerMode, int numRawSpeakers);
        String loadbank(const String pathToBank, const String flags);
        void unloadBank(const String &pathToBank);
        int getBankLoadingState(const String &pathToBank);
        int getBankBusCount(const String &pathToBank);
        int getBankEventCount(const String &pathToBank);
        int getBankStringCount(const String &pathToBank);
        int getBankVCACount(const String &pathToBank);
        String createEventInstance(const String &uuid, const String &eventPath);
        float getEventParameter(const String &uuid, const String &parameterName);
        void setEventParameter(const String &uuid, const String &parameterName, float value);
        void releaseEvent(const String &uuid);
        void startEvent(const String &uuid);
        void stopEvent(const String &uuid, String stopModeStr);
        void triggerEventCue(const String &uuid);
        int getEventPlaybackState(const String &uuid);
        bool getEventPaused(const String &uuid);
        void setEventPaused(const String &uuid, bool paused);
        float getEventPitch(const String &uuid);
        void setEventPitch(const String &uuid, float pitch);
        float getEventVolume(const String &uuid);
        void setEventVolume(const String &uuid, float volume);
        int getEventTimelinePosition(const String &uuid);
        void setEventTimelinePosition(const String &uuid, int position);
        float getEventReverbLevel(const String &uuid, int index);
        void setEventReverbLevel(const String &uuid, int index, float level);
        bool isEventVirtual(const String &uuid);

        /* bus functions */
        bool getBusMute(const String &busPath);
        bool getBusPaused(const String &busPath);
        float getBusVolume(const String &busPath);
        void setBusMute(const String &busPath, bool mute);
        void setBusPaused(const String &busPath, bool paused);
        void setBusVolume(const String &busPath, float volume);
        void stopAllBusEvents(const String &busPath, int stopMode);

        /* VCA functions */
        float getVCAVolume(const String &VCAPath);
        void setVCAVolume(const String &VCAPath, float volume);
        void playOneShot(const String eventName, Object *gameObj);
        void playOneShotWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void playOneShotAttached(String eventName, Object *gameObj);
        void playOneShotAttachedWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void attachInstanceToNode(const String &uuid, Object *gameObj);
        void detachInstanceFromNode(const String &uuid);

        void playSound(const String &uuid);
        String loadSound(const String &uuid, const String path, const String modeStr);
        void releaseSound(const String &path);
        void setSoundPaused(const String &uuid, bool paused);
        void stopSound(const String &uuid);
        bool isSoundPlaying(const String &uuid);
        void setSoundVolume(const String &uuid, float volume);
        float getSoundVolume(const String &uuid);
        float getSoundPitch(const String &uuid);
        void setSoundPitch(const String &uuid, float pitch);

    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
