# Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fmod-core-prebuilt
LOCAL_SRC_FILES := ../libs/fmod/android/core/lib/$(TARGET_ARCH_ABI)/libfmod.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := fmod-studio-prebuilt
LOCAL_SRC_FILES := ../libs/fmod/android/studio/lib/$(TARGET_ARCH_ABI)/libfmodstudio.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := godot-prebuilt
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := ../godot-cpp/bin/libgodot-cpp.android.release.i686.a
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := ../godot-cpp/bin/libgodot-cpp.android.release.armv7-a.a
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
    LOCAL_SRC_FILES := ../godot-cpp/bin/libgodot-cpp.android.release.armv8-a.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := android_fmod_gdnative
LOCAL_CPPFLAGS := -std=c++14
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := \
godot_fmod.cpp \
gdlibrary.cpp \

LOCAL_SHARED_LIBRARIES := \
fmod-core-prebuilt \
fmod-studio-prebuilt

LOCAL_C_INCLUDES := \
../godot-cpp/godot_headers \
../godot-cpp/include/ \
../godot-cpp/include/core \
../godot-cpp/include/gen \
../libs/fmod/android/studio/inc \
../libs/fmod/android/core/inc

LOCAL_STATIC_LIBRARIES := godot-prebuilt

include $(BUILD_SHARED_LIBRARY)