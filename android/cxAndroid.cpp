//
//  cxMain.c
//  cxEngine2D
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAndroid.h"

CX_CPP_BEGIN

cxStr *cxEngine::TextImage(const cxStr *txt,const cxTextAttr &attr,cxSize2F &size)
{
    return nullptr;
}


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

struct androidEngine androidEngine = {0};

/*java native method*/
JNIEXPORT void JNICALL Java_com_cxengine_EngineActivity_SendJson(JNIEnv *env,jobject this,jstring jstr)
{
    struct android_app *android_app = androidEngine.app;
    CX_ASSERT(android_app != NULL, "app null,send json invoke failed");
    jboolean copy = JNI_FALSE;
    const char *chars = (*env)->GetStringUTFChars(env,jstr,&copy);
    pthread_mutex_lock(&android_app->mutex);
    android_app->pendingJson = chars;
    int8_t cmd = APP_CMD_RECV_JSON;
    write(android_app->msgwrite, &cmd, sizeof(cmd));
    while (android_app->recvJson != android_app->pendingJson) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
    (*env)->ReleaseStringUTFChars(env,jstr,chars);
}

JavaVM *JavaVMInstance()
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    return app->activity->vm;
}

JNIEnv *JNIEnvInstance()
{
    return androidEngine.env;;
}

cxStr jstringTocxStr(JNIEnv *env,jstring jstr)
{
    if (jstr == NULL) {
        return NULL;
    }
    jboolean copy = JNI_FALSE;
    cxConstChars chars = (*env)->GetStringUTFChars(env,jstr,&copy);
    cxStr rv = cxStrConstChars(chars);
    (*env)->ReleaseStringUTFChars(env,jstr,chars);
    return rv;
}

jstring cxStrTojstring(JNIEnv *env,cxStr str)
{
    if(!cxStrOK(str)){
        return NULL;
    }
    return (*env)->NewStringUTF(env,cxStrBody(str));
}

//获取默认语言文件目录
cxStr cxDefaultLocalized(cxConstChars country,cxConstChars lang)
{
    cxStr ret = NULL;
    JNIMethodInfo info = {0};
    if(JNIAllocMethodInfo(&info,"EngineDefaultLocalized","()Ljava/lang/String;")){
        jstring path = (*info.env)->CallObjectMethod(info.env,info.object,info.methodID);
        if(path != NULL){
            ret = jstringTocxStr(info.env,path);
            (*info.env)->DeleteLocalRef(info.env,path);
        }
    }
    JNIFreeMethodInfo(&info);
    return ret;
}

//创建UUID
cxStr cxUUID()
{
    cxStr ret = NULL;
    JNIMethodInfo info = {0};
    if(JNIAllocMethodInfo(&info,"EngineUUID","()Ljava/lang/String;")){
        jstring uuid = (*info.env)->CallObjectMethod(info.env,info.object,info.methodID);
        if(uuid != NULL){
            ret = jstringTocxStr(info.env,uuid);
            (*info.env)->DeleteLocalRef(info.env,uuid);
        }
    }
    JNIFreeMethodInfo(&info);
    if(ret == NULL){
        return NULL;
    }
    return cxMD5(ret);
}

//创建文字纹理数据
cxStr cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,const cxTextAttr *attr)
{
    JNIMethodInfo info = {0};
    if(!JNIAllocMethodInfo(&info,"createTextBitmap", "(Ljava/lang/String;Ljava/lang/String;ZIIIFFFFFFFFFFFF)[B")){
        return NULL;
    }
    cxStr rv = NULL;
    jstring strText = (*info.env)->NewStringUTF(info.env,txt);
    jstring fontName = NULL;
    if(font != NULL){
        fontName = (*info.env)->NewStringUTF(info.env,font);
    }
    jint size = attr->size;
    jint align = attr->align;
    jint format = attr->format;
    //fixwidth
    jfloat fw = attr->fixWidth;
    //font color
    jfloat tr = attr->color.r;
    jfloat tg = attr->color.g;
    jfloat tb = attr->color.b;
    jfloat ta = attr->color.a;
    //stroke width
    jfloat sw = attr->strokeWidth;
    //stroke color
    jfloat sr = attr->strokeColor.r;
    jfloat sg = attr->strokeColor.g;
    jfloat sb = attr->strokeColor.b;
    jfloat sa = attr->strokeColor.a;
    //stroke offset
    jfloat sx = attr->strokeOffset.x;
    jfloat sy = attr->strokeOffset.y;
    //bold
    jboolean bold = attr->boldFont;
    jbyteArray bytes = (*info.env)->CallObjectMethod(info.env,info.object,info.methodID,
                                                     strText,fontName,
                                                     bold,
                                                     size,align,format,
                                                     fw,
                                                     tr,tg,tb,ta,
                                                     sw,sr,sg,sb,sa,sx,sy);
    jsize length = 0;
    if(bytes != NULL && (length = (*info.env)->GetArrayLength(info.env,bytes)) > 0){
        jbyte *buffer = allocator->malloc(length);
        (*info.env)->GetByteArrayRegion(info.env,bytes,0,length,buffer);
        rv = cxStrAttachMem((cxChars)buffer, length);
    }
    if(bytes != NULL){
        (*info.env)->DeleteLocalRef(info.env,bytes);
    }
    if(fontName != NULL){
        (*info.env)->DeleteLocalRef(info.env,fontName);
    }
    if(strText != NULL){
        (*info.env)->DeleteLocalRef(info.env,strText);
    }
    JNIFreeMethodInfo(&info);
    return rv;
}
//退出程序
void cxEngineTerminate()
{
    JNIMethodInfo info = {0};
    if(JNIAllocMethodInfo(&info,"EngineTerminate", "()V")){
        (*info.env)->CallVoidMethod(info.env,info.object,info.methodID);
    }
    JNIFreeMethodInfo(&info);
}

//获取存储路径
cxStr cxDocumentPath(cxConstChars file)
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    CX_ASSERT(app->activity->internalDataPath != NULL, "internalDataPath error");
    return cxStrCreate("%s/%s",app->activity->internalDataPath,file);
}

//获取国家 CN
cxStr cxLocalizedCountry()
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    AConfiguration_getCountry(app->config, androidEngine.Country);
    return cxStrConstChars(androidEngine.Country);
}

//获取语言 zh
cxStr cxLocalizedLang()
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    AConfiguration_getLanguage(app->config, androidEngine.Language);
    return cxStrConstChars(androidEngine.Language);
}
//日志打印
void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    if(cxConstCharsEqu(type, "ERROR") || cxConstCharsEqu(type, "ASSERT")){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else if(cxConstCharsEqu(type, "WARN")){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }
}
//获取资源路径
cxStr cxAssetsPath(cxConstChars file)
{
    CX_RETURN(file == NULL, NULL);
    return cxStrConstChars(file);
}
//获取资源文件fd
cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxStr path = cxAssetsPath(file);
    AAssetManager *assetMgr = cxEngineGetAssetManager();
    AAsset *asset = AAssetManager_open(assetMgr, cxStrBody(path), AASSET_MODE_UNKNOWN);
    if(asset == NULL){
        return -1;
    }
    off_t outStart = 0;
    off_t outLength = 0;
    cxInt fd = AAsset_openFileDescriptor(asset,&outStart,&outLength);
    *off = (cxInt)outStart;
    *length = (cxInt)outLength;
    AAsset_close(asset);
    return fd;
}
//资源是否存在
cxBool cxAssetsExists(cxConstChars file)
{
    cxBool ret = false;
    cxStr path = cxAssetsPath(file);
    AAssetManager *assetMgr = cxEngineGetAssetManager();
    AAsset *asset = AAssetManager_open(assetMgr, cxStrBody(path), AASSET_MODE_UNKNOWN);
    if(asset != NULL){
        ret = AAsset_getLength(asset) > 0;
        AAsset_close(asset);
    }
    return ret;
}
//获取资源管理器
AAssetManager *cxEngineGetAssetManager()
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    return app->activity->assetManager;
}

void JNIFreeMethodInfo(JNIMethodInfo *info)
{
    if(info->classID != NULL){
        (*info->env)->DeleteLocalRef(info->env,info->classID);
        info->classID = NULL;
    }
    info->methodID = NULL;
}

cxBool JNIAllocMethodInfo(JNIMethodInfo *info,cxConstChars methodName,cxConstChars paramCode)
{
    struct android_app *app = androidEngine.app;
    CX_ASSERT(app != NULL, "app null");
    JNIEnv *env = JNIEnvInstance();
    info->env = env;
    info->object = app->activity->clazz;
    jclass class = (*env)->GetObjectClass(env,info->object);
    if(class == NULL){
        CX_ERROR("get object nativeclass error");
        return false;
    }
    info->classID = class;
    jmethodID methodID = (*env)->GetMethodID(env,class,methodName,paramCode);
    if(methodID == NULL){
        CX_ERROR("get methodId %s(%s) error",methodName,paramCode);
        return false;
    }
    info->methodID = methodID;
    return true;
}

//发送json到主线程
void cxEngineSendJson(cxStr json)
{
    JNIMethodInfo info = {0};
    if(JNIAllocMethodInfo(&info,"RecvJson", "(Ljava/lang/String;)V")){
        jstring str = cxStrTojstring(info.env,json);
        (*info.env)->CallVoidMethod(info.env,info.object,info.methodID,str);
        (*info.env)->DeleteLocalRef(info.env,str);
    }
    JNIFreeMethodInfo(&info);
}

static int engine_init_display(struct androidEngine* androidEngine) {
    if(androidEngine->display == NULL){
        androidEngine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(androidEngine->display == NULL){
            CX_ERROR("eglGetDisplay error");
            return -1;
        }
        EGLint major = 0;
        EGLint minor = 0;
        if(!eglInitialize(androidEngine->display, &major, &minor)){
            CX_ERROR("eglInitialize error");
            return -1;
        }
        CX_LOGGER("EGL Version:%d.%d",major,minor);
        eglChooseConfig(androidEngine->display, display_attribs24, &androidEngine->config, 1, &androidEngine->numConfigs);
        if(!androidEngine->numConfigs){
            eglChooseConfig(androidEngine->display, display_attribs16, &androidEngine->config, 1, &androidEngine->numConfigs);
        }
        if(!androidEngine->numConfigs){
            CX_ERROR("get egl config error");
            return -1;
        }
        CX_LOGGER("EGL Config Number:%d",androidEngine->numConfigs);
        if(!eglGetConfigAttrib(androidEngine->display, androidEngine->config, EGL_NATIVE_VISUAL_ID, &androidEngine->format)){
            CX_ERROR("eglGetConfigAttrib error");
            return -1;
        }
        androidEngine->context = eglCreateContext(androidEngine->display, androidEngine->config, NULL, context_attribs);
        if(androidEngine->context == NULL){
            CX_ERROR("eglCreateContext error");
            return -1;
        }
    }
    return 0;
}

static int engine_init_surface(struct androidEngine* androidEngine) {
    
    EGLint w, h;
    EGLSurface surface;
    if(engine_init_display(androidEngine) != 0){
        return -1;
    }
    if(ANativeWindow_setBuffersGeometry(androidEngine->app->window, 0, 0, androidEngine->format) != 0){
        CX_ERROR("ANativeWindow_setBuffersGeometry error");
        return -1;
    }
    surface = eglCreateWindowSurface(androidEngine->display, androidEngine->config, androidEngine->app->window, NULL);
    if(surface == NULL){
        CX_ERROR("eglCreateWindowSurface error");
        return -1;
    }
    if (eglMakeCurrent(androidEngine->display, surface, surface, androidEngine->context) == EGL_FALSE) {
        CX_ERROR("Unable to eglMakeCurrent");
        return -1;
    }
    if(!eglQuerySurface(androidEngine->display, surface, EGL_WIDTH, &w)){
        CX_ERROR("eglQuerySurface error");
        return -1;
    }
    if(!eglQuerySurface(androidEngine->display, surface, EGL_HEIGHT, &h)){
        CX_ERROR("eglQuerySurface error");
        return -1;
    }
    androidEngine->surface = surface;
    androidEngine->width = w;
    androidEngine->height = h;
    return 0;
}

static void engine_draw_frame(struct androidEngine* androidEngine)
{
    cxEngine engine = cxEngineInstance();
    if(engine == NULL){
        return;
    }
    if(androidEngine->display == EGL_NO_DISPLAY) {
        return;
    }
    if(androidEngine->surface == EGL_NO_SURFACE){
        return;
    }
    cxEngine::Instance()->Run();
    eglSwapBuffers(androidEngine->display, androidEngine->surface);
}

static void engine_term_surface(struct androidEngine* androidEngine)
{
    if(androidEngine->surface != EGL_NO_SURFACE){
        eglDestroySurface(androidEngine->display, androidEngine->surface);
        androidEngine->surface = EGL_NO_SURFACE;
    }
}

static void engine_destroy_display(struct androidEngine* androidEngine)
{
    engine_term_surface(androidEngine);
    eglMakeCurrent(androidEngine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if(androidEngine->context != EGL_NO_CONTEXT){
        eglDestroyContext(androidEngine->display, androidEngine->context);
        androidEngine->context = EGL_NO_CONTEXT;
    }
    if(androidEngine->display != EGL_NO_DISPLAY){
        eglTerminate(androidEngine->display);
        androidEngine->display = EGL_NO_DISPLAY;
    }
}

static cxBool android_process_input(struct android_app* app)
{
    int ident = 0;
    int events = 0;
    struct android_poll_source* source = NULL;
    while((ident = ALooper_pollAll(androidEngine.animating?0:-1, NULL, &events,(void**)&source)) >= 0){
        if(source != NULL){
            source->process(app, source);
        }
        if(app->destroyRequested != 0) {
            return true;
        }
    }
    return false;
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    int type = AInputEvent_getType(event);
    //touch
    if (type == AINPUT_EVENT_TYPE_MOTION) {
        cxTouchType type = cxTouchTypeNone;
        cxTouchPoint points[10];
        cxInt num = 0;
        cxInt action = AMotionEvent_getAction(event);
        cxInt atype =  action & AMOTION_EVENT_ACTION_MASK;
        if(atype == AMOTION_EVENT_ACTION_POINTER_DOWN){
            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            cxTouchPoint *p = &points[num++];
            p->id = AMotionEvent_getPointerId(event, idx);
            p->xy.x = AMotionEvent_getX(event, idx);
            p->xy.y = AMotionEvent_getY(event, idx);
            type = cxTouchTypeDown;
        }else if(atype == AMOTION_EVENT_ACTION_POINTER_UP){
            cxInt idx = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            cxTouchPoint *p = &points[num++];
            p->id = AMotionEvent_getPointerId(event, idx);
            p->xy.x = AMotionEvent_getX(event, idx);
            p->xy.y = AMotionEvent_getY(event, idx);
            type = cxTouchTypeUp;
        }else if(atype == AMOTION_EVENT_ACTION_MOVE){
            cxInt count = AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchPoint *p = &points[num++];
                p->id = AMotionEvent_getPointerId(event, i);
                p->xy.x = AMotionEvent_getX(event, i);
                p->xy.y =AMotionEvent_getY(event, i);
            }
            type = cxTouchTypeMove;
        }else if(atype == AMOTION_EVENT_ACTION_CANCEL){
            cxInt count = AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchPoint *p = &points[num++];
                p->id = AMotionEvent_getPointerId(event, i);
                p->xy.x = AMotionEvent_getX(event, i);
                p->xy.y =AMotionEvent_getY(event, i);
            }
            type = cxTouchTypeCancel;
        }else if(atype == AMOTION_EVENT_ACTION_DOWN){
            cxInt count = AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchPoint *p = &points[num++];
                p->id = AMotionEvent_getPointerId(event, i);
                p->xy.x = AMotionEvent_getX(event, i);
                p->xy.y =AMotionEvent_getY(event, i);
            }
            type = cxTouchTypeDown;
        }else if(atype == AMOTION_EVENT_ACTION_UP){
            cxInt count = AMotionEvent_getPointerCount(event);
            for(cxInt i=0; i < count; i++){
                cxTouchPoint *p = &points[num++];
                p->id = AMotionEvent_getPointerId(event, i);
                p->xy.x = AMotionEvent_getX(event, i);
                p->xy.y =AMotionEvent_getY(event, i);
            }
            type = cxTouchTypeUp;
        }
        return cxEngineFireTouch(type, num, points);
    }
    //key
    if(type == AINPUT_EVENT_TYPE_KEY){
        cxKeyType type = cxKeyTypeDown;
        cxInt action = AKeyEvent_getAction(event);
        if(action == AKEY_EVENT_ACTION_DOWN){
            type = cxKeyTypeDown;
        }else if(action == AKEY_EVENT_ACTION_UP){
            type = cxKeyTypeUp;
        }else if(action == AKEY_EVENT_ACTION_MULTIPLE){
            type = cxKeyTypeMultiple;
        }
        return cxEngineFireKey(type, AKeyEvent_getKeyCode(event));
    }
    return 0;
}

void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct androidEngine* androidEngine = (struct androidEngine*)app->userData;
    switch (cmd) {
        case APP_CMD_RECV_JSON:{
            cxStr json = cxStrAttachMem(app->recvJson, (cxInt)strlen(app->recvJson));
            cxEngineRecvJson(json);
            app->recvJson = NULL;
            break;
        }
        case APP_CMD_INIT_WINDOW:{
            engine_init_surface(androidEngine);
            break;
        }
        case APP_CMD_TERM_WINDOW:{
            androidEngine->animating = 0;
            engine_term_surface(androidEngine);
            break;
        }
        case APP_CMD_INPUT_CHANGED:{
            break;
        }
        case APP_CMD_WINDOW_RESIZED:{
            cxEngineLayout(androidEngine->width, androidEngine->height);
            androidEngine->animating = 1;
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
            androidEngine->animating = 0;
            break;
        }
        case APP_CMD_CONFIG_CHANGED:{
            break;
        }
        case APP_CMD_LOW_MEMORY:{
            cxEngineMemory();
            break;
        }
        case APP_CMD_START:{
            break;
        }
        case APP_CMD_RESUME:{
            cxEngineResume();
            break;
        }
        case APP_CMD_SAVE_STATE:{
            break;
        }
        case APP_CMD_PAUSE:{
            androidEngine->animating = 0;
            cxEnginePause();
            break;
        }
        case APP_CMD_STOP:{
            androidEngine->animating = 0;
            break;
        }
        case APP_CMD_DESTROY:{
            androidEngine->animating = 0;
            app->destroyRequested = 1;
            break;
        }
        default:{
            break;
        }
    }
}

void android_main(struct android_app* app)
{
    memset(&androidEngine, 0, sizeof(struct androidEngine));
    
    app->userData = &androidEngine;
    app->onAppCmd = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;
    androidEngine.app = app;
    JavaVM *vm = JavaVMInstance();
    (*vm)->AttachCurrentThread(vm,&androidEngine.env,NULL);
    CX_ASSERT(androidEngine.env != NULL, "attach current thread jnienv error");
    CX_LOGGER("cxEngine android startup");
    while(!app->destroyRequested){
        if(android_process_input(app)){
            break;
        }
        if(androidEngine.animating) {
            engine_draw_frame(&androidEngine);
        }
    }
    CX_LOGGER("cxEngine android exit");
    cxEngine::Destroy();
    engine_destroy_display(&androidEngine);
    if(androidEngine.env != NULL){
        (*vm)->DetachCurrentThread(vm);
    }
}

int8_t android_app_read_cmd(struct android_app* android_app)
{
    int8_t cmd;
    read(android_app->msgread, &cmd, sizeof(cmd));
    return cmd;
}

static void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_RECV_JSON:{
            pthread_mutex_lock(&android_app->mutex);
            if(android_app->recvJson != NULL){
                allocator->free(android_app->recvJson);
                android_app->recvJson = NULL;
            }
            if(android_app->pendingJson != NULL){
                android_app->recvJson = allocator->strdup(android_app->pendingJson);
            }
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        }
        case APP_CMD_INPUT_CHANGED:
            pthread_mutex_lock(&android_app->mutex);
            if (android_app->inputQueue != NULL) {
                AInputQueue_detachLooper(android_app->inputQueue);
            }
            android_app->inputQueue = android_app->pendingInputQueue;
            if (android_app->inputQueue != NULL) {
                AInputQueue_attachLooper(android_app->inputQueue,android_app->looper, LOOPER_ID_INPUT, NULL,&android_app->inputPollSource);
            }
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        case APP_CMD_INIT_WINDOW:
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = android_app->pendingWindow;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        case APP_CMD_TERM_WINDOW:
            pthread_cond_broadcast(&android_app->cond);
            break;
        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            pthread_mutex_lock(&android_app->mutex);
            android_app->activityState = cmd;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        case APP_CMD_CONFIG_CHANGED:
            AConfiguration_fromAssetManager(android_app->config,android_app->activity->assetManager);
            break;
        case APP_CMD_DESTROY:
            android_app->destroyRequested = 1;
            break;
    }
}

static void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd)
{
    switch (cmd) {
        case APP_CMD_RECV_JSON:{
            pthread_mutex_lock(&android_app->mutex);
            if(android_app->recvJson != NULL){
                allocator->free(android_app->recvJson);
                android_app->recvJson = NULL;
            }
            android_app->pendingJson = NULL;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        }
        case APP_CMD_TERM_WINDOW:
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = NULL;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
        case APP_CMD_SAVE_STATE:
            pthread_mutex_lock(&android_app->mutex);
            android_app->stateSaved = 1;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;
    }
}

static void android_app_destroy(struct android_app* android_app)
{
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->inputQueue != NULL) {
        AInputQueue_detachLooper(android_app->inputQueue);
    }
    AConfiguration_delete(android_app->config);
    android_app->destroyed = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);
    
    if(android_app->savedState != NULL){
        allocator->free(android_app->savedState);
        android_app->savedState = NULL;
    }
}

static void process_input(struct android_app* app, struct android_poll_source* source)
{
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        if (app->onInputEvent != NULL) {
            handled = app->onInputEvent(app, event);
        }
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

static void process_cmd(struct android_app* app, struct android_poll_source* source)
{
    int8_t cmd = android_app_read_cmd(app);
    android_app_pre_exec_cmd(app, cmd);
    if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
    android_app_post_exec_cmd(app, cmd);
}

static void* android_app_entry(void* param)
{
    struct android_app* android_app = (struct android_app*)param;
    
    android_app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);
    
    android_app->cmdPollSource.id = LOOPER_ID_MAIN;
    android_app->cmdPollSource.app = android_app;
    android_app->cmdPollSource.process = process_cmd;
    
    android_app->inputPollSource.id = LOOPER_ID_INPUT;
    android_app->inputPollSource.app = android_app;
    android_app->inputPollSource.process = process_input;
    
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,&android_app->cmdPollSource);
    android_app->looper = looper;
    
    pthread_mutex_lock(&android_app->mutex);
    android_app->running = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);
    
    android_main(android_app);
    
    android_app_destroy(android_app);
    return NULL;
}

static struct android_app* android_app_create(ANativeActivity* activity,void* savedState, size_t savedStateSize)
{
    struct android_app* android_app = allocator->malloc(sizeof(struct android_app));
    android_app->activity = activity;
    pthread_mutex_init(&android_app->mutex, NULL);
    pthread_cond_init(&android_app->cond, NULL);
    if (savedState != NULL) {
        android_app->savedState = allocator->malloc(savedStateSize);
        android_app->savedStateSize = savedStateSize;
        memcpy(android_app->savedState, savedState, savedStateSize);
    }
    int msgpipe[2];
    if (pipe(msgpipe)) {
        return NULL;
    }
    android_app->msgread = msgpipe[0];
    android_app->msgwrite = msgpipe[1];
    
    android_app->recvJson = NULL;
    android_app->pendingJson = NULL;
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&android_app->thread, &attr, android_app_entry, android_app);
    
    pthread_mutex_lock(&android_app->mutex);
    while (!android_app->running) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
    return android_app;
}

void android_app_write_cmd(struct android_app* android_app, int8_t cmd)
{
    write(android_app->msgwrite, &cmd, sizeof(cmd));
}

static void android_app_set_input(struct android_app* android_app, AInputQueue* inputQueue)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app->pendingInputQueue = inputQueue;
    android_app_write_cmd(android_app, APP_CMD_INPUT_CHANGED);
    while (android_app->inputQueue != android_app->pendingInputQueue) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_window(struct android_app* android_app, ANativeWindow* window)
{
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->pendingWindow != NULL) {
        android_app_write_cmd(android_app, APP_CMD_TERM_WINDOW);
    }
    android_app->pendingWindow = window;
    if (window != NULL) {
        android_app_write_cmd(android_app, APP_CMD_INIT_WINDOW);
    }
    while (android_app->window != android_app->pendingWindow) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_activity_state(struct android_app* android_app, int8_t cmd)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, cmd);
    while (android_app->activityState != cmd) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_free(struct android_app* android_app)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, APP_CMD_DESTROY);
    while (!android_app->destroyed) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
    close(android_app->msgread);
    close(android_app->msgwrite);
    pthread_cond_destroy(&android_app->cond);
    pthread_mutex_destroy(&android_app->mutex);
    allocator->free(android_app);
}

static void onDestroy(ANativeActivity* activity)
{
    android_app_free((struct android_app*)activity->instance);
}

static void onStart(ANativeActivity* activity)
{
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity)
{
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    void* savedState = NULL;
    pthread_mutex_lock(&android_app->mutex);
    android_app->stateSaved = 0;
    android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);
    while (!android_app->stateSaved) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    if (android_app->savedState != NULL) {
        savedState = android_app->savedState;
        *outLen = android_app->savedStateSize;
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }
    pthread_mutex_unlock(&android_app->mutex);
    return savedState;
}

static void onPause(ANativeActivity* activity)
{
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity)
{
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    android_app_write_cmd(android_app, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    android_app_write_cmd(android_app, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    android_app_write_cmd(android_app,focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    android_app_set_window((struct android_app*)activity->instance, window);
}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    android_app_write_cmd(android_app, APP_CMD_WINDOW_RESIZED);
}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    android_app_write_cmd(android_app, APP_CMD_CONTENT_RECT_CHANGED);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    android_app_set_window((struct android_app*)activity->instance, NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    android_app_set_input((struct android_app*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    android_app_set_input((struct android_app*)activity->instance, NULL);
}

void ANativeActivity_onCreate(ANativeActivity* activity,void* savedState, size_t savedStateSize)
{
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->instance = android_app_create(activity, savedState, savedStateSize);
}

CX_CPP_END