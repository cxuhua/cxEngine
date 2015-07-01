
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := \
	-std=gnu99 \
	-DANDROID \
	-I$(LOCAL_PATH) \
	-I$(LOCAL_PATH)/libs/android/include \
	-I/Users/xuhua/android/android-ndk-r10d/sources/android/cpufeatures

LOCAL_CPPFLAGS := \
	-fno-rtti \
	-fno-exceptions \
	-DANDROID \
	-std=gnu++11 \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include \
    -I/Users/xuhua/android/android-ndk-r10d/sources/android/cpufeatures

LOCAL_MODULE := cxengine

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/ext/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/chipmunk/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/kazmath/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/kazmath/GL/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/android/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/core/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/*.cpp)

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)

