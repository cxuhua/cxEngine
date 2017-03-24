//
//  OpenGL.h
//  cxEngineCore
//
//  Created by xuhua on 6/12/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineCore_OpenGL_h
#define cxEngineCore_OpenGL_h

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
static PFNGLGENVERTEXARRAYSOESPROC      glGenVertexArrays;
static PFNGLBINDVERTEXARRAYOESPROC      glBindVertexArray;
static PFNGLDELETEVERTEXARRAYSOESPROC   glDeleteVertexArrays;
static PFNGLISVERTEXARRAYOESPROC        glIsVertexArray;
static PFNGLMAPBUFFEROESPROC            glMapBuffer;
static PFNGLUNMAPBUFFEROESPROC          glUnmapBuffer;
static PFNGLGETBUFFERPOINTERVOESPROC    glGetBufferPointerv;
#define GL_DEPTH24_STENCIL8             GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY                   GL_WRITE_ONLY_OES

#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define glDeleteVertexArrays            glDeleteVertexArraysOES
#define glGenVertexArrays               glGenVertexArraysOES
#define glBindVertexArray               glBindVertexArrayOES
#define glIsVertexArray                 glIsVertexArrayOES
#define glMapBuffer                     glMapBufferOES
#define glUnmapBuffer                   glUnmapBufferOES
#define glGetBufferPointerv             glGetBufferPointervOES
#define GL_DEPTH24_STENCIL8             GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY                   GL_WRITE_ONLY_OES
#elif (CX_TARGET_PLATFORM == CX_PLATFORM_MAC)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#define glDeleteVertexArrays            glDeleteVertexArraysAPPLE
#define glGenVertexArrays               glGenVertexArraysAPPLE
#define glBindVertexArray               glBindVertexArrayAPPLE
#define glClearDepthf                   glClearDepth
#define glDepthRangef                   glDepthRange

#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG GL_COMPRESSED_RGBA
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG GL_COMPRESSED_RGBA
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG  GL_COMPRESSED_RGB
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG  GL_COMPRESSED_RGB
#else
#error "not define CX_TARGET_PLATFORM"
#endif

#endif
