//
// Created by menghai on 16/7/14.
//
#include "jni.h"
#ifndef MY_APPLICATION_FMSHAREDGLCONTEXTWRAPPER_H
#define MY_APPLICATION_FMSHAREDGLCONTEXTWRAPPER_H


#include "fmSharedGLContext.h"



extern "C" {


JNIEXPORT jlong JNICALL
Java_com_appmagics_demo_Engine_nativeCreateShareEGLContext(JNIEnv *env, jclass type,
                                                                                jint width, jint height,jobject surface);



JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeRelease(JNIEnv *env, jclass type,
                                                                  jlong nativeAddress) ;

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeMakeCurrent(JNIEnv *env, jclass type,
                                                                      jlong nativeAddress) ;

JNIEXPORT void JNICALL
Java_com_appmagics_demo_Engine_nativeWrapBuffer(JNIEnv *env, jobject instance,
                                                                     jlong nativeAddress) ;



}










#endif //MY_APPLICATION_FMSHAREDGLCONTEXTWRAPPER_H
