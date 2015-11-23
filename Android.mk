
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES += cpufeatures

LOCAL_CFLAGS := \
	-std=gnu99 \
	-DANDROID \
	-I$(LOCAL_PATH) \
	-I$(LOCAL_PATH)/libs/android/include

LOCAL_CPPFLAGS := \
	-fno-rtti \
	-fno-exceptions \
	-DANDROID \
	-std=gnu++11 \
    -I$(LOCAL_PATH) \
    -I$(LOCAL_PATH)/libs/android/include

LOCAL_MODULE := cxengine


LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Collision/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Collision/Shapes/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Common/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Dynamics/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Dynamics/Contacts/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Dynamics/Joints/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/Box2D/Rope/*.cpp)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/ext/*.c)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/kazmath/*.c)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/kazmath/GL/*.c)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/spine/*.c)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/poly2tri/common/*.cc)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/poly2tri/sweep/*.cc)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/android/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/core/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/math/*.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/engine/*.cpp)

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)

