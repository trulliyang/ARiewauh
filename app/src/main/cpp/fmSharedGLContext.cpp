//
// Created by menghai on 16/7/13.
//


#include "fmSharedGLContext.h"
#include <android/native_window_jni.h>


EGLint fmSharedGLContext::s_bitR = 8;
EGLint fmSharedGLContext::s_bitG = 8;
EGLint fmSharedGLContext::s_bitB = 8;
EGLint fmSharedGLContext::s_bitA = 8;


void fmSharedGLContext::setContextColorBits(int r, int g, int b, int a) {

    s_bitR = r;
    s_bitG = g;
    s_bitB = b;
    s_bitA = a;


}


fmSharedGLContext *fmSharedGLContext::create(int width, int height, JNIEnv *env, jobject surface) {

    fmSharedGLContext *context = new fmSharedGLContext();
    if (!context->init(EGL_NO_CONTEXT, width, height, PBUFFER, env, surface)) {
        delete context;
        context = nullptr;


    }
    return context;


}


fmSharedGLContext *fmSharedGLContext::create(EGLContext sharedContext, int width, int height,
                                             ContextType type, JNIEnv *env, jobject surface) {

    fmSharedGLContext *context = new fmSharedGLContext();

    if (!context->init(sharedContext, width, height, type, env, surface)) {
        delete context;
        context = nullptr;


    }
    return context;

}


fmSharedGLContext::~fmSharedGLContext() {


    destroy();


}

bool fmSharedGLContext::init(EGLContext sharedContext, int width, int height, ContextType type,
                             JNIEnv *env, jobject surface) {


    if ((m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {

        FM_LOG_ERROR("eglGetDisplay() returned error 0x%x", eglGetError());
        return false;
    }

    EGLint major, minor;

    if (!eglInitialize(m_display, &major, &minor)) {
        FM_LOG_ERROR("eglInitialize() returned error 0x%x", eglGetError());
        return false;
    }


    EGLint attribList[]{

            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE

    };

//



   // EGLint *configSpec;


   // if(surface!=NULL) {

        EGLint  configSpec[] = {
                EGL_BUFFER_SIZE, 32,
                EGL_ALPHA_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE


        };
      //  configSpec=configSpec1;

//    }else {
//
//        EGLint configSpec1[] = {
//                EGL_RED_SIZE, 8,
//                EGL_GREEN_SIZE, 8,
//                EGL_BLUE_SIZE, 8,
//                EGL_ALPHA_SIZE, 8,
//                EGL_RENDERABLE_TYPE,
//                EGL_OPENGL_ES2_BIT,
//                EGL_NONE, 0,
//                EGL_NONE
//
//        };
//        configSpec=configSpec1;
//    }


    EGLConfig config;
    EGLint numConfigs;
    EGLint format;
//
//    const int configSize = sizeof(configSpec) / sizeof(*configSpec);
//
//    switch (type) {
//        case RECORDABLE_ANDROID:
//            configSpec[configSize - 3] = EGL_RECORDABLE_ANDROID;
//            configSpec[configSize - 2] = 1;
//
//
//            break;
//
//        case PBUFFER:
//            configSpec[configSize - 3] = EGL_SURFACE_TYPE;
//            configSpec[configSize - 2] = EGL_PBUFFER_BIT;
//        default:;
//
//    }

    const EGLint pBufferAttrib[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };



    FM_LOG_INFO("eglInitialize: major: %d, minor: %d", major, minor);

    if (!eglChooseConfig(m_display, configSpec, &config, 1, &numConfigs)) {
        FM_LOG_ERROR("eglChooseConfig() returned error 0x%x", eglGetError());
        destroy();
        return false;
    }

    FM_LOG_INFO("Config num: %d, sharedContext id: %p", numConfigs, sharedContext);

    m_context = eglCreateContext(m_display, config, sharedContext, attribList);
    if (m_context == EGL_NO_CONTEXT) {
        FM_LOG_ERROR("eglCreateContext Failed:  0x%x", eglGetError());
        return false;
    }

        ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
        m_surface = eglCreateWindowSurface(m_display, config, window, EGL_FALSE);


    if (m_surface == EGL_NO_SURFACE) {
        FM_LOG_ERROR("eglCreatePbufferSurface Failed:  0x%x", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        FM_LOG_ERROR("eglMakeCurrent failed: 0x%x", eglGetError());
        return false;
    }


    int clientVersion;
    eglQueryContext(m_display, m_context, EGL_CONTEXT_CLIENT_VERSION, &clientVersion);
    FM_LOG_INFO("EGLContext created, client version %d\n", clientVersion);


    return true;
}

void fmSharedGLContext::destroy() {
    FM_LOG_INFO("####FMSharedGLContext Destroying context... ####");

    if (m_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_display, m_context);
        }
        eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
    }

    m_display = EGL_NO_DISPLAY;
    m_surface = EGL_NO_SURFACE;
    m_context = EGL_NO_CONTEXT;
}

void fmSharedGLContext::makecurrent() {


    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        FM_LOG_ERROR("eglMakeCurrent failed: 0x%x", eglGetError());
    }
}

bool fmSharedGLContext::swapbuffers() {
    bool bo = false;
    if(m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE){
        bo = (bool) eglSwapBuffers(m_display, m_surface);
    }

    return bo;
}