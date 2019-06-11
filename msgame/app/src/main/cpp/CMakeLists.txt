# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CORE_PATH ${CMAKE_SOURCE_DIR}/../../../../../)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

include_directories(
        ${CORE_PATH}
        ${CORE_PATH}/android/src/main/cpp
        ${CORE_PATH}/android/include)

link_directories(
        libs/${ANDROID_ABI}
        ${CORE_PATH}/android/libs/${ANDROID_ABI})

link_libraries(libcxengine.so libgmclient.so)

add_library(
        game
        SHARED
        CGmSktUse.cpp
        Game.cpp)

target_link_libraries(
        game
        log
        gmclient
        cxengine)
