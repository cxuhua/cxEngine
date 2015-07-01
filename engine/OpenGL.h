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
PFNGLGENVERTEXARRAYSOESPROC         glGenVertexArrays;
PFNGLBINDVERTEXARRAYOESPROC         glBindVertexArray;
PFNGLDELETEVERTEXARRAYSOESPROC      glDeleteVertexArrays;
PFNGLISVERTEXARRAYOESPROC           glIsVertexArray;
PFNGLMAPBUFFEROESPROC               glMapBuffer;
PFNGLUNMAPBUFFEROESPROC             glUnmapBuffer;
PFNGLGETBUFFERPOINTERVOESPROC       glGetBufferPointerv;
#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES
#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define glDeleteVertexArrays		glDeleteVertexArraysOES
#define glGenVertexArrays			glGenVertexArraysOES
#define glBindVertexArray			glBindVertexArrayOES
#define glIsVertexArray             glIsVertexArrayOES
#define glMapBuffer                 glMapBufferOES
#define glUnmapBuffer               glUnmapBufferOES
#define glGetBufferPointerv         glGetBufferPointervOES
#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES
#else
#error "not define CX_TARGET_PLATFORM"
#endif

#endif
