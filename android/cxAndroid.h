//
//  cxMain.c
//  cxEngine2D
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//


#ifndef __cxEngine__cxAndroid__
#define __cxEngine__cxAndroid__

#include <core/cxDefine.h>
#include <jni.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <EGL/egl.h>
#include <android/input.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android/configuration.h>
#include <engine/cxEngine.h>

CX_CPP_BEGIN

typedef struct
{
    JNIEnv      *env;
    jclass      classID;
    jmethodID   methodID;
    jobject     object;
}JNIMethodInfo;

void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize);

AAssetManager *cxEngineGetAssetManager();

cxStr jstringTocxStr(JNIEnv *env,jstring jstr);

jstring cxStrTojstring(JNIEnv *env,cxStr str);

JavaVM *JavaVMInstance();

JNIEnv *JNIEnvInstance();

cxBool JNIAllocMethodInfo(JNIMethodInfo *info,cxConstChars methodName,cxConstChars paramCode);

void JNIFreeMethodInfo(JNIMethodInfo *info);

struct android_app;
struct android_poll_source {
    int32_t id;
    struct android_app* app;
    void (*process)(struct android_app* app, struct android_poll_source* source);
};
struct android_app {
    void* userData;
    void (*onAppCmd)(struct android_app* app, int32_t cmd);
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);
    ANativeActivity* activity;
    AConfiguration* config;
    void* savedState;
    size_t savedStateSize;
    ALooper* looper;
    char *recvJson;
    const char *pendingJson;
    AInputQueue* inputQueue;
    ANativeWindow* window;
    ARect contentRect;
    int activityState;
    int destroyRequested;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int msgread;
    int msgwrite;
    pthread_t thread;
    struct android_poll_source cmdPollSource;
    struct android_poll_source inputPollSource;
    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
    ARect pendingContentRect;
};

int8_t android_app_read_cmd(struct android_app* android_app);

void android_app_write_cmd(struct android_app* android_app, int8_t cmd);

enum {
    LOOPER_ID_MAIN  = 1,
    LOOPER_ID_INPUT = 2,
    LOOPER_ID_USER  = 3,
};

enum {
    APP_CMD_INPUT_CHANGED,
    APP_CMD_INIT_WINDOW,
    APP_CMD_TERM_WINDOW,
    APP_CMD_WINDOW_RESIZED,
    APP_CMD_WINDOW_REDRAW_NEEDED,
    APP_CMD_CONTENT_RECT_CHANGED,
    APP_CMD_GAINED_FOCUS,
    APP_CMD_LOST_FOCUS,
    APP_CMD_CONFIG_CHANGED,
    APP_CMD_LOW_MEMORY,
    APP_CMD_START,
    APP_CMD_RESUME,
    APP_CMD_SAVE_STATE,
    APP_CMD_PAUSE,
    APP_CMD_STOP,
    APP_CMD_DESTROY,
    APP_CMD_RECV_JSON,
};

struct androidEngine {
    struct android_app* app;
    int animating;
    EGLDisplay display;
    int format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    char Language[3];
    char Country[3];
    JNIEnv *env;
};

CX_CPP_END

#endif
