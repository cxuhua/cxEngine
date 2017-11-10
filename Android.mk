
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := \
	-std=gnu99 \
	-I$(LOCAL_PATH) \
	-I$(LOCAL_PATH)/libs/android/include

LOCAL_CPPFLAGS := \
	-fno-rtti \
	-fno-exceptions \
	-std=gnu++11 \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include

LOCAL_MODULE := cxengine

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/ext/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/android/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/core/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/engine/*.cpp)

include $(BUILD_STATIC_LIBRARY)

