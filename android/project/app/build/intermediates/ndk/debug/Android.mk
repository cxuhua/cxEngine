LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := app
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_SRC_FILES := \
	/Users/xuhua/cxengine/android/project/app/src/main/jni/Android.mk \
	/Users/xuhua/cxengine/android/project/app/src/main/jni/Application.mk \
	/Users/xuhua/cxengine/android/project/app/src/main/jni/debug \
	/Users/xuhua/cxengine/android/project/app/src/main/jni/Game.cpp \
	/Users/xuhua/cxengine/android/project/app/src/main/jni/release \

LOCAL_C_INCLUDES += /Users/xuhua/cxengine/android/project/app/src/main/jni
LOCAL_C_INCLUDES += /Users/xuhua/cxengine/android/project/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
