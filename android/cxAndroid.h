//
//  cxMain.c
//  cxEngine2D
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//


#ifndef __cxEngine__cxAndroid__
#define __cxEngine__cxAndroid__

#include <core/cxObject.h>
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

class cxAndroid;
class cxAndroidUtil;

struct AndroidPollSource
{
    int32_t id;
    cxAndroid *app;
    void (*process)(cxAndroid *app, AndroidPollSource *source);
};

class cxAndroid : public cxObject
{
public:
    CX_DECLARE(cxAndroid);
    friend cxAndroidUtil;
protected:
    explicit cxAndroid();
    virtual ~cxAndroid();
private:
    static cxAndroid *instance;
    ANativeActivity* activity;
    AConfiguration* config;
    void *state;
    size_t size;
    char Language[3];
    char Country[3];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int msgread;
    int msgwrite;
    int8_t readcmd();
    void writecmd(int8_t cmd);
    
    int destroyed;
    int animating;
    int destroyRequested;
    int tapCount;
    
    
    ALooper *looper;
    int running;
    pthread_t thread;
    static void *AndroidEntry(void *data);
    
    AndroidPollSource cmd;
    AndroidPollSource input;
    
    ANativeWindow* window;
    ANativeWindow* pendingWindow;
    void SetWindow(ANativeWindow* window);
    
    AInputQueue *inputQueue;
    AInputQueue* pendingInputQueue;
    void SetInput(AInputQueue* inputQueue);
    
    int8_t activityState;
    void SetActivityState(int8_t cmd);
    //input event
    int32_t HandleMotionInput(AInputEvent* event);
    int32_t HandleKeyInput(AInputEvent* event);
    int32_t HandleInput(AInputEvent* event);
    //cmd event
    void onAppCmd(int8_t cmd);
    //main entry
    void cxAndroidMain();
    
    static void cxAndroidPreExec(cxAndroid *app, int8_t cmd);
    static void cxAndroidExec(cxAndroid *app, AndroidPollSource *source);
    static void cxAndroidPostExec(cxAndroid *app, int8_t cmd);
    
    static void cxAndroidInputExec(cxAndroid *app, AndroidPollSource *source);
private:
    JavaVM *vm;
    JNIEnv *env;
    EGLDisplay display;
    int format;
    EGLint numConfigs;
    EGLConfig eglConfig;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    int InitDisplay();
    int InitSurface();
    void DrawFrame();
    void DestroySurface();
    void DestroyDisplay();
    cxBool ProcessInput();
private:
    static void onDestroy(ANativeActivity* activity);
    static void onStart(ANativeActivity* activity);
    static void onResume(ANativeActivity* activity);
    static void *onSaveInstanceState(ANativeActivity* activity, size_t* outLen);
    static void onPause(ANativeActivity* activity);
    static void onStop(ANativeActivity* activity);
    static void onConfigurationChanged(ANativeActivity* activity);
    static void onLowMemory(ANativeActivity* activity);
    static void onWindowFocusChanged(ANativeActivity* activity, int focused);
    static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);
    static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window);
    static void onContentRectChanged(ANativeActivity* activity, const ARect* rect);
    static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);
    static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);
    static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);
public:
    static cxAndroid *Instance();
    static void Destroy();
    void Init(ANativeActivity *a,void *d, size_t l);
public:
    void Logger(const char* type,const char*file,int line,const char* format,va_list ap);
    const cxStr *GetLang() const;
    const cxStr *GetCountry() const;
    const cxStr *DocumentPath(cchars file);
    const cxStr *AssetsData(cchars file);
};

CX_CPP_END

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize);

#endif
