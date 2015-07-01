
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := libopenal.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pcre
LOCAL_SRC_FILES := libpcre.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jpeg
LOCAL_SRC_FILES := libjpeg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := png
LOCAL_SRC_FILES := libpng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := uv
LOCAL_SRC_FILES := libuv.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jansson
LOCAL_SRC_FILES := libjansson.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := libfreetype.a
include $(PREBUILT_STATIC_LIBRARY)

