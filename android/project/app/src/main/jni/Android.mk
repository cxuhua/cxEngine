
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cxEngineAndroid

LOCAL_WHOLE_STATIC_LIBRARIES += cxengine

LOCAL_STATIC_LIBRARIES := expat pcre jpeg png uv jansson mpg123 openal

LOCAL_LDLIBS := -llog -lGLESv2 -lz -landroid -lEGL -lOpenSLES -latomic

LOCAL_CFLAGS := \
	-O2 \
	-std=gnu99 \
	-DANDROID \
	-I$(CX_ENGINE_PATH) \
	-I$(CX_ENGINE_PATH)/libs/android/include \
	-I$(LOCAL_PATH)

LOCAL_CPPFLAGS := \
	-O2 \
	-fno-rtti \
	-fno-exceptions \
	-DANDROID \
	-std=gnu++11 \
    -I$(CX_ENGINE_PATH) \
    -I$(CX_ENGINE_PATH)/libs/android/include \
    -I$(LOCAL_PATH)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/*.cpp)


include $(BUILD_SHARED_LIBRARY)

$(call import-module,libs/android/lib/$(TARGET_ARCH_ABI))
$(call import-module,.)
