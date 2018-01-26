//
// Created by menghai on 16/7/13.
//

#ifndef MY_APPLICATION_FMSHAREDGLCONTEXT_H
#define MY_APPLICATION_FMSHAREDGLCONTEXT_H

#include <EGL/egl.h>
#include "fmPlatform_ANDROID.h"



#ifndef EGL_CONTEXT_CLIENT_VERSION
#define EGL_CONTEXT_CLIENT_VERSION 0x3098
#endif

#ifndef EGL_RECORDABLE_ANDROID
#define EGL_RECORDABLE_ANDROID 0x3142
#endif


class fmSharedGLContext {
    fmSharedGLContext() { }

public:
    enum ContextType {
        PBUFFER,
        RECORDABLE_ANDROID

    };



    static fmSharedGLContext *create(int width, int height,JNIEnv *env,jobject surface);

    static fmSharedGLContext *create(EGLContext sharedContext, int width, int height,
                                     ContextType type,JNIEnv *env,jobject surface);

    ~fmSharedGLContext();

    void makecurrent();

    bool swapbuffers();

    static void setContextColorBits(int r, int g, int b, int a);

private:

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLContext m_context;

    static EGLint s_bitR, s_bitG, s_bitB, s_bitA;

    bool init(EGLContext sharedContext, int width, int height, ContextType type,JNIEnv *env,jobject surface);
    void destroy();

};


#endif //MY_APPLICATION_FMSHAREDGLCONTEXT_H
