LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cxEngineAndroid
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_SRC_FILES := \
	/Users/xuhua/Documents/cxengine/android/project/app/src/main/jni/Android.mk \
	/Users/xuhua/Documents/cxengine/android/project/app/src/main/jni/Application.mk \
	/Users/xuhua/Documents/cxengine/android/project/app/src/main/jni/debug \
	/Users/xuhua/Documents/cxengine/android/project/app/src/main/jni/release \

LOCAL_C_INCLUDES += /Users/xuhua/Documents/cxengine/android/project/app/src/main/jni
LOCAL_C_INCLUDES += /Users/xuhua/Documents/cxengine/android/project/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
