
#set ndk path and src root path

NDK_PATH := /Users/xuhua/android-sdk-macosx/ndk-bundle
CX_ENGINE_PATH := /Users/xuhua/cxengine

NDK_TOOLCHAIN_VERSION := clang
APP_STL := c++_static
APP_PLATFORM := android-21
APP_ABI := x86 x86_64 armeabi-v7a arm64-v8a

NDK_MODULE_PATH += $(CX_ENGINE_PATH)
