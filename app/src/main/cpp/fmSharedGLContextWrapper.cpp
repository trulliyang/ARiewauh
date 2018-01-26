//
// Created by menghai on 16/7/14.
//

#include "fmSharedGLContextWrapper.h"
#include "fmEngine.h"


extern "C" {

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeGetCameraData(JNIEnv *env, jobject instance, jlong address) {
    fmEngine *engine= (fmEngine *) address;
//    __android_log_print(ANDROID_LOG_ERROR, "shiyang", "send data addr=%d, engine=%p", address, engine);
    engine->getCameraData();
}

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeSendCameraData(JNIEnv *env, jobject instance, jobject data, jlong address) {
    fmEngine *engine= (fmEngine *) address;
//    __android_log_print(ANDROID_LOG_ERROR, "shiyang", "send data addr=%d, engine=%p", address, engine);
    unsigned char *t_imageDate = (unsigned char *)env->GetDirectBufferAddress(data);
    engine->sendData(t_imageDate);
}

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeDraw(JNIEnv *env, jobject instance, jint textId,jlong address) {
    fmEngine *engine= (fmEngine *) address;
//    __android_log_print(ANDROID_LOG_ERROR, "shiyang", "native draw addr=%d, engine=%p", address, engine);
    engine->drawTex(textId);
}

JNIEXPORT jlong JNICALL
Java_com_appmagics_demo_Engine_nativeCreateEngine(JNIEnv *env, jobject instance) {
    fmEngine *engine=new fmEngine();
    engine->create();
    
    return (jlong)engine;
}


JNIEXPORT jlong JNICALL
Java_com_appmagics_demo_Engine_nativeCreateShareEGLContext(JNIEnv *env, jclass type, jint width,
                                                                        jint height,
                                                                        jobject surface) {
    fmSharedGLContext *m_fmSharedContext=fmSharedGLContext::create(width,height,env,surface);
    return (jlong)m_fmSharedContext;
}


JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeRelease(JNIEnv *env, jclass type,
                                                          jlong nativeAddress) {
    fmSharedGLContext *m_fmSharedContext=(fmSharedGLContext *)nativeAddress;
    delete  m_fmSharedContext;
}

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeMakeCurrent(JNIEnv *env, jclass type,
                                                              jlong nativeAddress) {
    fmSharedGLContext *m_fmSharedContext=(fmSharedGLContext *)nativeAddress;
    m_fmSharedContext->makecurrent();
}

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeWrapBuffer(JNIEnv *env, jobject instance,
                                                             jlong nativeAddress) {
    fmSharedGLContext *m_fmSharedContext=(fmSharedGLContext *)nativeAddress;
    m_fmSharedContext->swapbuffers();

    // TODO

}



}