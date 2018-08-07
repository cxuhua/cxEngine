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

extern "C" JNIEXPORT void JNICALL Java_com_xginx_cxengine_EngineActivity_PushEvent(JNIEnv *env, jobject instance, jlong key, jstring jstr) {
    jboolean copy = JNI_FALSE;
    cchars chars = env->GetStringUTFChars(jstr,&copy);
    cxStr *txt = cxStr::Alloc()->Init(chars);
    env->ReleaseStringUTFChars(jstr,chars);
    cxEngine::Instance()->PushEvent(key,txt);
    txt->Release();
}

JNIMethodInfo::JNIMethodInfo()
{
    env  = nullptr;
    classID = nullptr;
    methodID = nullptr;
    object = nullptr;
}

JNIMethodInfo::~JNIMethodInfo()
{
    if(classID != nullptr){
        env->DeleteLocalRef(classID);
        classID = nullptr;
    }
    methodID = nullptr;
}

jobject JNIMethodInfo::CallObjectMethod(cxAndroid *app,...)
{
    va_list ap;
    va_start(ap, app);
    jobject ret = env->CallObjectMethodV(object, methodID, ap);
    va_end(ap);
    return ret;
}

jint JNIMethodInfo::CallIntMethod(cxAndroid *app,...)
{
    va_list ap;
    va_start(ap, app);
    jint ret = env->CallIntMethodV(object, methodID, ap);
    va_end(ap);
    return ret;
}

jstring JNIMethodInfo::CallStringMethod(cxAndroid *app,...)
{
    va_list ap;
    va_start(ap, app);
    jstring ret = (jstring)env->CallObjectMethod(object, methodID, ap);
    va_end(ap);
    return ret;
}


void JNIMethodInfo::CallVoidMethod(cxAndroid *app,...)
{
    va_list ap;
    va_start(ap, app);
    env->CallVoidMethodV(object, methodID, ap);
    va_end(ap);
}

//invoke java methpd engineTerminate
void cxEngine::Exit()
{
    cxAndroid *app = cxAndroid::Instance();
    if(app == nullptr){
        return;
    }
    JNIMethodInfo m = app->JNIMethod("EngineTerminate", "()V");
    m.CallVoidMethod(app);
}

const cxStr *cxEngine::TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &texSize)
{
    cxAndroid *app = cxAndroid::Instance();
    if(app == nullptr){
        return nullptr;
    }
    JNIMethodInfo m = app->JNIMethod("CreateTextBitmap", "(Ljava/lang/String;Ljava/lang/String;ZIIIFFFFFFFFFFFF)[B");
    cxInt32 *sp=nullptr;
    cxStr *rv = NULL;
    jstring strText = app->Tojstring(txt);
    jstring fontName = NULL;
    
    jint size = attr.size;
    jint align = attr.align;
    jint fmt = attr.format;
    //fixwidth
    jfloat fw = attr.fixWidth;
    //font color
    jfloat tr = attr.color.r;
    jfloat tg = attr.color.g;
    jfloat tb = attr.color.b;
    jfloat ta = attr.color.a;
    //stroke width
    jfloat sw = attr.strokeWidth;
    //stroke color
    jfloat sr = attr.strokeColor.r;
    jfloat sg = attr.strokeColor.g;
    jfloat sb = attr.strokeColor.b;
    jfloat sa = attr.strokeColor.a;
    //stroke offset
    jfloat sx = attr.strokeOffset.x;
    jfloat sy = attr.strokeOffset.y;
    //bold
    jboolean bold = attr.boldFont;
    jsize length = 0;
    jbyteArray bytes=(jbyteArray)m.CallObjectMethod(app,strText,fontName,bold,size,align,fmt,fw,tr,tg,tb,ta,sw,sr,sg,sb,sa,sx,sy);
    if(bytes == nullptr){
        goto finished;
    }
    length = m.env->GetArrayLength(bytes);
    if(length == 0){
        goto finished;
    }
    CX_ASSERT(length >= 8, "bytes length error");
    rv = cxStr::Create()->Init(length);
    m.env->GetByteArrayRegion(bytes, 0, length, (jbyte *)rv->Buffer());
    sp = (cxInt32 *)(rv->Data() + length - 8);
    texSize.w = *sp;
    texSize.h = *(++sp);
    rv->KeepBytes(length - 8);
finished:
    if(bytes != NULL){
        m.env->DeleteLocalRef(bytes);
    }
    if(fontName != NULL){
        m.env->DeleteLocalRef(fontName);
    }
    if(strText != NULL){
        m.env->DeleteLocalRef(strText);
    }
    return rv;
}

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize)
{
    cxAndroid *android = cxAndroid::Instance();
    activity->instance = android;
    android->Init(activity, savedState, savedStateSize);
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

JNIMethodInfo cxAndroid::JNIMethod(cchars name,cchars param)
{
    JNIMethodInfo info;
    info.env = env;
    info.object = activity->clazz;
    jclass clasz = info.env->GetObjectClass(info.object);
    if(clasz == NULL){
        CX_ASSERT(false, "get object nativeclass error");
    }
    info.classID = clasz;
    jmethodID methodID = info.env->GetMethodID(clasz,name,param);
    if(methodID == NULL){
        CX_ASSERT(false, "get methodId %s(%s) error", name, param);
    }
    info.methodID = methodID;
    return info;
}

void cxAndroid::Destroy()
{
    if(instance == nullptr){
        return;
    }
    instance->Release();
    instance = nullptr;
}

cxAndroid::cxAndroid()
{
    config = AConfiguration_new();
    state = nullptr;
    size = 0;
    memset(Language, 0, 3);
    memset(Country, 0, 3);
    
    int msgpipe[2];
    pipe(msgpipe);
    msgread = msgpipe[0];
    msgwrite = msgpipe[1];
    
    running = false;
    animating = false;
    destroyReq = false;
    destroyed = false;
    activityState = -1;
    
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
    AConfiguration_delete(config);
    free(state);
}

int cxAndroid::InitDisplay()
{
    if(display != NULL){
        return 0;
    }
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
        CX_ERROR("eglQuerySurface w error");
        return -1;
    }
    if(!eglQuerySurface(display, surface, EGL_HEIGHT, &h)){
        CX_ERROR("eglQuerySurface h error");
        return -1;
    }
    width = w;
    height = h;
    return 0;
}

void cxAndroid::DrawFrame()
{
    if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
        return;
    }
    cxEngine *engine = cxEngine::Instance();
    if(engine == NULL){
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
    mutex.Lock();
    writecmd(cmd);
    while (activityState != cmd) {
        cond.Wait(mutex);
    }
    mutex.Unlock();
}

void cxAndroid::writecmd(int8_t cmd)
{
    write(msgwrite, &cmd, sizeof(cmd));
}

cxStr *cxAndroid::TocxStr(jstring jstr)
{
    if(jstr == nullptr){
        return nullptr;
    }
    jboolean copy = JNI_FALSE;
    cchars chars = env->GetStringUTFChars(jstr,&copy);
    cxStr *rv = cxStr::Create()->Init(chars);
    env->ReleaseStringUTFChars(jstr,chars);
    return rv;
}

jstring cxAndroid::Tojstring(const cxStr *str)
{
    if(!cxStr::IsOK(str)){
        return NULL;
    }
    return env->NewStringUTF(str->ToChars());
}

cxStr *cxAndroid::UUID()
{
    JNIMethodInfo m = JNIMethod("NewUUID", "()Ljava/lang/String;");
    jstring uuid = (jstring)m.CallObjectMethod(this);
    cxStr *ret = TocxStr(uuid);
    env->DeleteLocalRef(uuid);
    return ret;
}

cxStr *cxAndroid::GetLang()
{
    AConfiguration_getLanguage(config, (char *)Language);
    return cxStr::Create()->Init(Language);
}

cxInt cxAndroid::GetVersionCode()
{
    JNIMethodInfo m = JNIMethod("getVersionCode", "()I");
    return m.CallIntMethod(this);
}

cxStr *cxAndroid::GetVersionName()
{
    JNIMethodInfo m = JNIMethod("getVersionName", "()Ljava/lang/String;");
    jstring uuid = (jstring)m.CallObjectMethod(this);
    cxStr *ret = TocxStr(uuid);
    env->DeleteLocalRef(uuid);
    return ret;
}



cxStr *cxAndroid::GetCountry()
{
    AConfiguration_getCountry(config, (char *)Country);
    return cxStr::Create()->Init(Country);
}

cxStr *cxAndroid::DocumentPath(cchars file)
{
    CX_ASSERT(activity->internalDataPath != nullptr, "activity internal data path null");
    return cxStr::Create()->AppFmt("%s/%s",activity->internalDataPath,file);
}

cxStr *cxAndroid::AssetsData(cchars file)
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
        case APP_CMD_INPUT_CHANGED:{
            app->mutex.Lock();
            if (app->inputQueue != NULL) {
                AInputQueue_detachLooper(app->inputQueue);
            }
            app->inputQueue = app->pendingInputQueue;
            if (app->inputQueue != NULL) {
                AInputQueue_attachLooper(app->inputQueue,app->looper, LOOPER_ID_INPUT, NULL,&app->input);
            }
            app->cond.Broadcast();
            app->mutex.Unlock();
            break;
        }
        case APP_CMD_INIT_WINDOW:{
            app->mutex.Lock();
            app->window = app->pendingWindow;
            app->cond.Broadcast();
            app->mutex.Unlock();
            break;
        }
        case APP_CMD_TERM_WINDOW:{
            app->cond.Broadcast();
            break;
        }
        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:{
            app->mutex.Lock();
            app->activityState = cmd;
            app->cond.Broadcast();
            app->mutex.Unlock();
            break;
        }
        case APP_CMD_CONFIG_CHANGED:{
            AConfiguration_fromAssetManager(app->config,app->activity->assetManager);
            break;
        }
        case APP_CMD_DESTROY:{
            app->destroyReq = true;
            break;
        }
    }
}

void cxAndroid::cxAndroidPostExec(cxAndroid *app, int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:{
            app->mutex.Lock();
            app->window = NULL;
            app->cond.Broadcast();
            app->mutex.Unlock();
            break;
        }
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
            animating = true;
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
            animating = false;
            break;
        }
        case APP_CMD_CONFIG_CHANGED:{
            break;
        }
        case APP_CMD_LOW_MEMORY:{
            cxEngine::Instance()->Warning();
            break;
        }
        case APP_CMD_START:{
            break;
        }
        case APP_CMD_RESUME:{
            cxEngine::Instance()->Resume();
            animating = true;
            break;
        }
        case APP_CMD_SAVE_STATE:{
            break;
        }
        case APP_CMD_PAUSE:{
            animating = false;
            cxEngine::Instance()->Pause();
            break;
        }
        case APP_CMD_STOP:{
            animating = false;
            break;
        }
        case APP_CMD_DESTROY:{
            animating = false;
            destroyReq = true;
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

int32_t cxAndroid::HandleMotionInput(AInputEvent* event)
{
    cxInt action = AMotionEvent_getAction(event);
    cxInt atype =  action & AMOTION_EVENT_ACTION_MASK;
    switch (atype) {
        case AMOTION_EVENT_ACTION_POINTER_DOWN:{
            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            cxTouchId id = AMotionEvent_getPointerId(event, idx);
            cxFloat x = AMotionEvent_getX(event, idx);
            cxFloat y = AMotionEvent_getY(event, idx);
            cxEngine::Instance()->Dispatch(id, cxTouchPoint::Began, x, y);
            break;
        }
        case AMOTION_EVENT_ACTION_POINTER_UP:{
            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            cxTouchId id = AMotionEvent_getPointerId(event, idx);
            cxFloat x = AMotionEvent_getX(event, idx);
            cxFloat y = AMotionEvent_getY(event, idx);
            cxEngine::Instance()->Dispatch(id, cxTouchPoint::Ended, x, y);
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE:{
            cxInt count = (cxInt)AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchId id = AMotionEvent_getPointerId(event, i);
                cxFloat x = AMotionEvent_getX(event, i);
                cxFloat y =AMotionEvent_getY(event, i);
                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Moved, x, y);
            }
            break;
        }
        case AMOTION_EVENT_ACTION_DOWN:{
            cxInt count = (cxInt)AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchId id = AMotionEvent_getPointerId(event, i);
                cxFloat x = AMotionEvent_getX(event, i);
                cxFloat y =AMotionEvent_getY(event, i);
                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Began, x, y);
            }
            break;
        }
        case AMOTION_EVENT_ACTION_UP:{
            cxInt count = (cxInt)AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchId id = AMotionEvent_getPointerId(event, i);
                cxFloat x = AMotionEvent_getX(event, i);
                cxFloat y =AMotionEvent_getY(event, i);
                cxEngine::Instance()->Dispatch(id, cxTouchPoint::Ended, x, y);
            }
            break;
        }
    }
    return 0;
}

int32_t cxAndroid::HandleKeyInput(AInputEvent* event)
{
    cxKeyType type = cxKeyTypeDown;
    cxInt action = AKeyEvent_getAction(event);
    if(action == AKEY_EVENT_ACTION_DOWN){
        type = cxKeyTypeDown;
    }else if(action == AKEY_EVENT_ACTION_UP){
        type = cxKeyTypeUp;
    }else if(action == AKEY_EVENT_ACTION_MULTIPLE){
        type = cxKeyTypeMultiple;
    }
    return cxEngine::Instance()->Dispatch(type, AKeyEvent_getKeyCode(event));
}

int32_t cxAndroid::HandleInput(AInputEvent* event)
{
    int type = AInputEvent_getType(event);
    if(type == AINPUT_EVENT_TYPE_MOTION){
        return HandleMotionInput(event);
    }
    if(type == AINPUT_EVENT_TYPE_KEY){
        return HandleKeyInput(event);
    }
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
        if(destroyReq) {
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

void cxAndroid::AndroidEntry(void *data)
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
    
    app->mutex.Lock();
    app->running = true;
    app->cond.Broadcast();
    app->mutex.Unlock();
    
    app->cxAndroidMain();
    
    app->mutex.Lock();
    if (app->inputQueue != NULL) {
        AInputQueue_detachLooper(app->inputQueue);
        app->inputQueue = NULL;
    }
    AConfiguration_delete(app->config);
    app->config = NULL;
    app->destroyed = true;
    app->cond.Broadcast();
    app->mutex.Unlock();
}

void cxAndroid::SetWindow(ANativeWindow* window)
{
    mutex.Lock();
    if (pendingWindow != NULL) {
        writecmd(APP_CMD_TERM_WINDOW);
    }
    pendingWindow = window;
    if (window != NULL) {
        writecmd(APP_CMD_INIT_WINDOW);
    }
    while (window != pendingWindow) {
        cond.Wait(mutex);
    }
    mutex.Unlock();
}

void cxAndroid::SetInput(AInputQueue* inputQueue)
{
    mutex.Lock();
    pendingInputQueue = inputQueue;
    writecmd(APP_CMD_INPUT_CHANGED);
    while (inputQueue != pendingInputQueue) {
        cond.Wait(mutex);
    }
    mutex.Unlock();
}

void cxAndroid::cxAndroidMain()
{
    vm = activity->vm;
    vm->AttachCurrentThread(&env, nullptr);
    CX_ASSERT(env != nullptr, "attach current thread java env error");
    //startup engine
    cxEngine::Startup(false);
    while(!destroyReq){
        if(ProcessInput()){
            break;
        }
        if(animating) {
            DrawFrame();
        }
    }
    //destroy engine
    cxEngine::Destroy();
    //clear core cache
    cxCore::Instance()->Clear();
    DestroyDisplay();
    if(env != nullptr){
        vm->DetachCurrentThread();
        env = NULL;
    }
}

void cxAndroid::onDestroy(ANativeActivity* activity)
{
    cxAndroid *app = (cxAndroid *)activity->instance;
    app->mutex.Lock();
    app->writecmd(APP_CMD_DESTROY);
    while (!app->destroyed) {
        app->cond.Wait(app->mutex);
    }
    app->mutex.Unlock();
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
    uv_thread_create(&thread, cxAndroid::AndroidEntry, this);
    mutex.Lock();
    while (!running) {
        cond.Wait(mutex);
    }
    mutex.Unlock();
}

CX_CPP_END
