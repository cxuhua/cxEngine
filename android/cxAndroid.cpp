//
//  cxMain.c
//  cxEngine2D
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxTouch.h>
#include "cxAndroid.h"

const EGLint display_attribs24[] = {
    EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
    EGL_BLUE_SIZE,      8,
    EGL_GREEN_SIZE,     8,
    EGL_RED_SIZE,       8,
    EGL_ALPHA_SIZE,     8,
    EGL_DEPTH_SIZE,     24,
    EGL_STENCIL_SIZE,   8,
    EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
    EGL_NONE
};

const EGLint display_attribs16[] = {
    EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
    EGL_BLUE_SIZE,      8,
    EGL_GREEN_SIZE,     8,
    EGL_RED_SIZE,       8,
    EGL_ALPHA_SIZE,     8,
    EGL_DEPTH_SIZE,     16,
    EGL_STENCIL_SIZE,   8,
    EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
    EGL_NONE
};

const EGLint context_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

CX_CPP_BEGIN

cxStr *cxEngine::TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size)
{
    return nullptr;
}

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize)
{
    activity->instance = cxAndroid::Instance();
    cxAndroid::Instance()->Init(activity, savedState, savedStateSize);
}

CX_IMPLEMENT(cxAndroid)

cxAndroid *cxAndroid::instance = nullptr;

cxAndroid *cxAndroid::Instance()
{
    if(instance == nullptr){
        instance = cxAndroid::Alloc();
    }
    return instance;
}

void cxAndroid::Destroy()
{
    if(instance == nullptr){
        return;
    }
    instance->Release();
    instance = nullptr;
    CX_LOGGER("cxAndroid destroy");
}

cxAndroid::cxAndroid()
{
    config = AConfiguration_new();
    state = nullptr;
    size = 0;
    memset(Language, 0, 3);
    memset(Country, 0, 3);
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    int msgpipe[2];
    pipe(msgpipe);
    msgread = msgpipe[0];
    msgwrite = msgpipe[1];
    
    running = 0;
    animating = 0;
    destroyRequested = 0;
    destroyed = 0;
    activityState = 0;
    
    window = nullptr;
    pendingWindow = nullptr;
    
    inputQueue = nullptr;
    pendingInputQueue = nullptr;
    
    vm = nullptr;
    env = nullptr;
    surface = EGL_NO_SURFACE;
    context = EGL_NO_CONTEXT;
    display = EGL_NO_DISPLAY;
    width = 0;
    height = 0;
}

cxAndroid::~cxAndroid()
{
    close(msgread);
    close(msgwrite);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    AConfiguration_delete(config);
    free(state);
}

int cxAndroid::InitDisplay()
{
    if(display == NULL){
       display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(display == NULL){
            CX_ERROR("eglGetDisplay error");
            return -1;
        }
        EGLint major = 0;
        EGLint minor = 0;
        if(!eglInitialize(display, &major, &minor)){
            CX_ERROR("eglInitialize error");
            return -1;
        }
        CX_LOGGER("EGL Version:%d.%d",major,minor);
        eglChooseConfig(display, display_attribs24, &eglConfig, 1, &numConfigs);
        if(!numConfigs){
            eglChooseConfig(display, display_attribs16, &eglConfig, 1, &numConfigs);
        }
        if(!numConfigs){
            CX_ERROR("get egl config error");
            return -1;
        }
        CX_LOGGER("EGL Config Number:%d",numConfigs);
        if(!eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &format)){
            CX_ERROR("eglGetConfigAttrib error");
            return -1;
        }
        context = eglCreateContext(display, eglConfig, NULL, context_attribs);
        if(context == NULL){
            CX_ERROR("eglCreateContext error");
            return -1;
        }
    }
    return 0;
}

int cxAndroid::InitSurface()
{
    
    EGLint w, h;
    if(InitDisplay() != 0){
        return -1;
    }
    if(ANativeWindow_setBuffersGeometry(window, 0, 0, format) != 0){
        CX_ERROR("ANativeWindow_setBuffersGeometry error");
        return -1;
    }
    surface = eglCreateWindowSurface(display, eglConfig, window, NULL);
    if(surface == NULL){
        CX_ERROR("eglCreateWindowSurface error");
        return -1;
    }
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        CX_ERROR("Unable to eglMakeCurrent");
        return -1;
    }
    if(!eglQuerySurface(display, surface, EGL_WIDTH, &w)){
        CX_ERROR("eglQuerySurface error");
        return -1;
    }
    if(!eglQuerySurface(display, surface, EGL_HEIGHT, &h)){
        CX_ERROR("eglQuerySurface error");
        return -1;
    }
    width = w;
    height = h;
    return 0;
}

void cxAndroid::DrawFrame()
{
    cxEngine *engine = cxEngine::Instance();
    if(engine == NULL){
        return;
    }
    if(display == EGL_NO_DISPLAY) {
        return;
    }
    if(surface == EGL_NO_SURFACE){
        return;
    }
    engine->Run();
    eglSwapBuffers(display, surface);
}

void cxAndroid::DestroySurface()
{
    if(surface != EGL_NO_SURFACE){
        eglDestroySurface(display, surface);
        surface = EGL_NO_SURFACE;
    }
}

void cxAndroid::DestroyDisplay()
{
    DestroySurface();
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if(context != EGL_NO_CONTEXT){
        eglDestroyContext(display, context);
        context = EGL_NO_CONTEXT;
    }
    if(display != EGL_NO_DISPLAY){
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
}

int8_t cxAndroid::readcmd()
{
    int8_t cmd;
    read(msgread, &cmd, sizeof(cmd));
    return cmd;
}

void cxAndroid::SetActivityState(int8_t cmd)
{
    pthread_mutex_lock(&mutex);
    writecmd(cmd);
    while (activityState != cmd) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void cxAndroid::writecmd(int8_t cmd)
{
    write(msgwrite, &cmd, sizeof(cmd));
}

const cxStr *cxAndroid::GetLang() const
{
    AConfiguration_getLanguage(config, (char *)Language);
    return cxStr::Create()->Init(Language);
}

const cxStr *cxAndroid::GetCountry() const
{
    AConfiguration_getCountry(config, (char *)Country);
    return cxStr::Create()->Init(Country);
}

const cxStr *cxAndroid::DocumentPath(cchars file)
{
    CX_ASSERT(activity->internalDataPath != nullptr, "activity internal data path null");
    return cxStr::Create()->AppFmt("%s/%s",activity->internalDataPath,file);
}

const cxStr *cxAndroid::AssetsData(cchars file)
{
    AAsset *asset = AAssetManager_open(activity->assetManager, file, AASSET_MODE_UNKNOWN);
    if(asset == nullptr){
        return nullptr;
    }
    cxInt l = (cxInt)AAsset_getLength(asset);
    if(l <= 0){
        return nullptr;
    }
    cxStr *ret = cxStr::Create()->Init(l);
    AAsset_read(asset, ret->Buffer(), (size_t)l);
    AAsset_close(asset);
    return ret;
}

void cxAndroid::Logger(const char* type,const char*file,int line,const char* format,va_list ap)
{
    char *buffer = nullptr;
    vasprintf(&buffer, format, ap);
    if(strcmp(type, "ERROR") == 0 || strcmp(type, "ASSERT") == 0){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else if(strcmp(type, "WARN") == 0){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }
    free(buffer);
}

void cxAndroid::cxAndroidPreExec(cxAndroid *app, int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_INPUT_CHANGED:
            pthread_mutex_lock(&app->mutex);
            if (app->inputQueue != NULL) {
                AInputQueue_detachLooper(app->inputQueue);
            }
            app->inputQueue = app->pendingInputQueue;
            if (app->inputQueue != NULL) {
                AInputQueue_attachLooper(app->inputQueue,app->looper, LOOPER_ID_INPUT, NULL,&app->input);
            }
            pthread_cond_broadcast(&app->cond);
            pthread_mutex_unlock(&app->mutex);
            break;
        case APP_CMD_INIT_WINDOW:
            pthread_mutex_lock(&app->mutex);
            app->window = app->pendingWindow;
            pthread_cond_broadcast(&app->cond);
            pthread_mutex_unlock(&app->mutex);
            break;
        case APP_CMD_TERM_WINDOW:
            pthread_cond_broadcast(&app->cond);
            break;
        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            pthread_mutex_lock(&app->mutex);
            app->activityState = cmd;
            pthread_cond_broadcast(&app->cond);
            pthread_mutex_unlock(&app->mutex);
            break;
        case APP_CMD_CONFIG_CHANGED:
            AConfiguration_fromAssetManager(app->config,app->activity->assetManager);
            break;
        case APP_CMD_DESTROY:
            app->destroyRequested = 1;
            break;
    }
}

void cxAndroid::cxAndroidPostExec(cxAndroid *app, int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:
            pthread_mutex_lock(&app->mutex);
            app->window = NULL;
            pthread_cond_broadcast(&app->cond);
            pthread_mutex_unlock(&app->mutex);
            break;
    }
}

void cxAndroid::onAppCmd(int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:{
            InitSurface();
            break;
        }
        case APP_CMD_TERM_WINDOW:{
            animating = 0;
            DestroySurface();
            break;
        }
        case APP_CMD_INPUT_CHANGED:{
            break;
        }
        case APP_CMD_WINDOW_RESIZED:{
            cxEngine::Instance()->Layout(width, height);
            animating = 1;
            break;
        }
        case APP_CMD_WINDOW_REDRAW_NEEDED:{
            break;
        }
        case APP_CMD_CONTENT_RECT_CHANGED:{
            break;
        }
        case APP_CMD_GAINED_FOCUS:{
            break;
        }
        case APP_CMD_LOST_FOCUS:{
            animating = 0;
            break;
        }
        case APP_CMD_CONFIG_CHANGED:{
            break;
        }
        case APP_CMD_LOW_MEMORY:{
            cxEngine::Instance()->MemoryWarning();
            break;
        }
        case APP_CMD_START:{
            break;
        }
        case APP_CMD_RESUME:{
            cxEngine::Instance()->Resume();
            animating = 1;
            break;
        }
        case APP_CMD_SAVE_STATE:{
            break;
        }
        case APP_CMD_PAUSE:{
            animating = 0;
            cxEngine::Instance()->Pause();
            break;
        }
        case APP_CMD_STOP:{
            animating = 0;
            break;
        }
        case APP_CMD_DESTROY:{
            animating = 0;
            destroyRequested = 1;
            break;
        }
        default:{
            break;
        }
    }
}

void cxAndroid::cxAndroidExec(cxAndroid *app, AndroidPollSource *source)
{
    int8_t cmd = app->readcmd();
    cxAndroidPreExec(app, cmd);
    app->onAppCmd(cmd);
    cxAndroidPostExec(app, cmd);
}

int32_t cxAndroid::HandleInput(AInputEvent* event)
{
//    int type = AInputEvent_getType(event);
//    //touch
//    if (type == AINPUT_EVENT_TYPE_MOTION) {
//        cxInt action = AMotionEvent_getAction(event);
//        cxInt atype =  action & AMOTION_EVENT_ACTION_MASK;
//        if(atype == AMOTION_EVENT_ACTION_POINTER_DOWN){
//            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
//            cxTouchId id = AMotionEvent_getPointerId(event, idx);
//            cxFloat x = AMotionEvent_getX(event, idx);
//            cxFloat y = AMotionEvent_getY(event, idx);
//            cxEngine::Instance()->Dispatch(id, cxTouchPoint::Began, 0, x, y);
//        }else if(atype == AMOTION_EVENT_ACTION_POINTER_UP){
//            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
//            cxTouchId id = AMotionEvent_getPointerId(event, idx);
//            cxFloat x = AMotionEvent_getX(event, idx);
//            cxFloat y = AMotionEvent_getY(event, idx);
//            cxEngine::Instance()->Dispatch(id, cxTouchPoint::Ended, 0, x, y);
//        }else if(atype == AMOTION_EVENT_ACTION_MOVE){
//            cxInt count = AMotionEvent_getPointerCount(event);
//            for(cxInt i=0; i < count; i++){
//                cxTouchPoint *p = &points[num++];
//                p->id = AMotionEvent_getPointerId(event, i);
//                p->xy.x = AMotionEvent_getX(event, i);
//                p->xy.y =AMotionEvent_getY(event, i);
//                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Moved, 0, x, y);
//            }
//            type = cxTouchTypeMove;
//        }else if(atype == AMOTION_EVENT_ACTION_DOWN){
//            cxInt count = AMotionEvent_getPointerCount(event);
//            for(cxInt i=0; i < count; i++){
//                cxTouchPoint *p = &points[num++];
//                p->id = AMotionEvent_getPointerId(event, i);
//                p->xy.x = AMotionEvent_getX(event, i);
//                p->xy.y =AMotionEvent_getY(event, i);
//                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Began, 0, x, y);
//            }
//            type = cxTouchTypeDown;
//        }else if(atype == AMOTION_EVENT_ACTION_UP){
//            cxInt count = AMotionEvent_getPointerCount(event);
//            for(cxInt i=0; i < count; i++){
//                cxTouchPoint *p = &points[num++];
//                p->id = AMotionEvent_getPointerId(event, i);
//                p->xy.x = AMotionEvent_getX(event, i);
//                p->xy.y =AMotionEvent_getY(event, i);
//                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Ended, 0, x, y);
//            }
//        }
//        return cxEngineFireTouch(type, num, points);
//    }
//    //key
//    if(type == AINPUT_EVENT_TYPE_KEY){
//        cxKeyType type = cxKeyTypeDown;
//        cxInt action = AKeyEvent_getAction(event);
//        if(action == AKEY_EVENT_ACTION_DOWN){
//            type = cxKeyTypeDown;
//        }else if(action == AKEY_EVENT_ACTION_UP){
//            type = cxKeyTypeUp;
//        }else if(action == AKEY_EVENT_ACTION_MULTIPLE){
//            type = cxKeyTypeMultiple;
//        }
//        return cxEngineFireKey(type, AKeyEvent_getKeyCode(event));
//    }
    return 0;
}

cxBool cxAndroid::ProcessInput()
{
    int ident = 0;
    int events = 0;
    struct AndroidPollSource *source = NULL;
    while((ident = ALooper_pollAll(animating?0:-1, NULL, &events,(void**)&source)) >= 0){
        if(source != NULL){
            source->process(this,source);
        }
        if(destroyRequested != 0) {
            return true;
        }
    }
    return false;
}

void cxAndroid::cxAndroidInputExec(cxAndroid *app, AndroidPollSource *source)
{
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = app->HandleInput(event);
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

void *cxAndroid::AndroidEntry(void *data)
{
    cxAndroid *app = (cxAndroid *)data;
    AConfiguration_fromAssetManager(app->config, app->activity->assetManager);
    
    app->cmd.id = LOOPER_ID_MAIN;
    app->cmd.app = app;
    app->cmd.process = cxAndroid::cxAndroidExec;
    
    app->input.id = LOOPER_ID_INPUT;
    app->input.app = app;
    app->input.process = cxAndroid::cxAndroidInputExec;
    
    app->looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(app->looper, app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,&app->cmd);
    
    pthread_mutex_lock(&app->mutex);
    app->running = 1;
    pthread_cond_broadcast(&app->cond);
    pthread_mutex_unlock(&app->mutex);
    
    app->cxAndroidMain();
    
    pthread_mutex_lock(&app->mutex);
    if (app->inputQueue != NULL) {
        AInputQueue_detachLooper(app->inputQueue);
        app->inputQueue = NULL;
    }
    AConfiguration_delete(app->config);
    app->config = NULL;
    app->destroyed = 1;
    pthread_cond_broadcast(&app->cond);
    pthread_mutex_unlock(&app->mutex);

    return data;
}

void cxAndroid::SetWindow(ANativeWindow* window)
{
    pthread_mutex_lock(&mutex);
    if (pendingWindow != NULL) {
        writecmd(APP_CMD_TERM_WINDOW);
    }
    pendingWindow = window;
    if (window != NULL) {
        writecmd(APP_CMD_INIT_WINDOW);
    }
    while (window != pendingWindow) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void cxAndroid::SetInput(AInputQueue* inputQueue)
{
    pthread_mutex_lock(&mutex);
    pendingInputQueue = inputQueue;
    writecmd(APP_CMD_INPUT_CHANGED);
    while (inputQueue != pendingInputQueue) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void cxAndroid::cxAndroidMain()
{
    vm = activity->vm;
    vm->AttachCurrentThread(&env, nullptr);
    CX_ASSERT(env != nullptr, "attach current thread java env error");
    cxEngine::Startup(false);
    CX_LOGGER("cxengine start");
    while(!destroyRequested){
        if(ProcessInput()){
            break;
        }
        if(animating) {
            DrawFrame();
        }
    }
    CX_LOGGER("cxengie exit");
    cxEngine::Destroy();
    DestroyDisplay();
    if(env != nullptr){
        vm->DetachCurrentThread();
        env = NULL;
    }
    
}

void cxAndroid::onDestroy(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    pthread_mutex_lock(&app->mutex);
    app->writecmd(APP_CMD_DESTROY);
    while (!app->destroyed) {
        pthread_cond_wait(&app->cond, &app->mutex);
    }
    pthread_mutex_unlock(&app->mutex);
    cxAndroid::Destroy();
}

void cxAndroid::onStart(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetActivityState(APP_CMD_START);
}

void cxAndroid::onResume(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetActivityState(APP_CMD_RESUME);
}

void *cxAndroid::onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    *outLen = 0;
    return NULL;
}

void cxAndroid::onPause(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetActivityState(APP_CMD_PAUSE);
}

void cxAndroid::onStop(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetActivityState(APP_CMD_STOP);
}

void cxAndroid::onConfigurationChanged(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->writecmd(APP_CMD_CONFIG_CHANGED);
}

void cxAndroid::onLowMemory(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->writecmd(APP_CMD_LOW_MEMORY);
}

void cxAndroid::onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->writecmd(focused?APP_CMD_GAINED_FOCUS:APP_CMD_LOST_FOCUS);
}

void cxAndroid::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetWindow(window);
}

void cxAndroid::onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->writecmd(APP_CMD_WINDOW_RESIZED);
}

void cxAndroid::onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->writecmd(APP_CMD_CONTENT_RECT_CHANGED);
}

void cxAndroid::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetWindow(nullptr);
}

void cxAndroid::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetInput(queue);
}

void cxAndroid::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->SetInput(nullptr);
}

void cxAndroid::Init(ANativeActivity *a,void *d, size_t l)
{
    activity = a;
    if(l > 0){
        state = malloc(l);
        memcpy(state, d, l);
    }

    //set event
    activity->callbacks->onDestroy = cxAndroid::onDestroy;
    activity->callbacks->onStart = cxAndroid::onStart;
    activity->callbacks->onResume = cxAndroid::onResume;
    activity->callbacks->onSaveInstanceState = cxAndroid::onSaveInstanceState;
    activity->callbacks->onPause = cxAndroid::onPause;
    activity->callbacks->onStop = cxAndroid::onStop;
    activity->callbacks->onConfigurationChanged = cxAndroid::onConfigurationChanged;
    activity->callbacks->onLowMemory = cxAndroid::onLowMemory;
    activity->callbacks->onWindowFocusChanged = cxAndroid::onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = cxAndroid::onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = cxAndroid::onNativeWindowResized;
    activity->callbacks->onContentRectChanged = cxAndroid::onContentRectChanged;
    activity->callbacks->onNativeWindowDestroyed = cxAndroid::onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = cxAndroid::onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = cxAndroid::onInputQueueDestroyed;
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, cxAndroid::AndroidEntry, this);
    
    //wait AndroidEntry run
    pthread_mutex_lock(&mutex);
    while (!running) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

CX_CPP_END
