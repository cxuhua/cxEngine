
#set ndk path and src root path

NDK_PATH := /Users/xuhua/android/android-ndk
CX_ENGINE_PATH := /Users/xuhua/cxengine

NDK_TOOLCHAIN_VERSION := clang
APP_STL := gnustl_static
APP_PLATFORM := android-14
APP_ABI := armeabi

NDK_MODULE_PATH += $(CX_ENGINE_PATH)
