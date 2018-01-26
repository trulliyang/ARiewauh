//
// Created by 梦海 on 2017/4/1.
//

#include "fmEngine.h"
//#include "opencv2/opencv.hpp"
//#include "FaceTracker.h"
#include "pch.h"
#include "time.h"
extern int clock_gettime(int, struct timespec *);
struct timespec now;
char *data = new char[720*1280*3/2];
static unsigned int showTex = 0;
static unsigned int yTex = 0;
static unsigned int uvTex = 0;
unsigned int yuvWidth = 800;
unsigned int yuvHeight = 1280;

//static unsigned int grayTex = 0;
//static unsigned char *grayData = new unsigned char[720*1280];
//static unsigned char *rgbaData = new unsigned char[720*1280*4];
//FaceTracker* fm = nullptr;
#include <DeviceInterface.h>
#include <unistd.h>
VideoStream * streamDepth = nullptr;
VideoStream * streamColor = nullptr;
Device deviceDepth;
Device deviceColor;


void fmEngine::create() {
    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::create()");
    draw = new fmDrawer();
    draw->loaderShader();

    drawYUV = new fmDrawerYUV();
    drawYUV->loaderShader();
    
//    drawGray = new fmDrawerGray();
//    drawGray->loaderShader();
//
//    drawPoints = new fmDrawerPoints();
//    drawPoints->loaderShader();
    
    mIsDeviceColorOK = initDeviceColor();
    if (mIsDeviceColorOK) {
        __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar init Device Color succeed");
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar init Device Color failed");
    }
    
    glGenTextures(1,&showTex);
    glBindTexture(GL_TEXTURE_2D,showTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,yuvWidth,yuvHeight,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1,&yTex);
    glBindTexture(GL_TEXTURE_2D,yTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,yuvWidth,yuvHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1,&uvTex);
    glBindTexture(GL_TEXTURE_2D,uvTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,yuvWidth/2,yuvHeight/2,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

//    glGenTextures(1,&grayTex);
//    glBindTexture(GL_TEXTURE_2D,grayTex);
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,720,1280,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
//    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

bool fmEngine::initDeviceColor()
{
    int result = 0;
    int count = 0;
    do {
        result = deviceDepth.open("123");
        __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar device.open result = %d", result);
        if (0 == result) {
            __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar device.open succeed");
            break;
        }
        count++;
        usleep(500*1000);
    } while (count<=20);
    
    if (result) {
        return false;
    }
    
    if (nullptr == streamColor) {
        streamColor = new VideoStream();
    }
    
    result = streamColor->create(deviceDepth, SENSOR_COLOR);
    __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar create result = %d", result);
    
    if (0 == result) {
        __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar create succeed.");
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "AR", "shiyang ar create failed. return!!!");
        return false;
    }
    
    int mode = streamColor->getVideoMode();
    streamColor->setVideoMode(mode);
    return true;
}

bool fmEngine::initDeviceDepth()
{
    return false;
}

void fmEngine::getCameraData() {
    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::getCameraData()");
    if (!mIsDeviceColorOK) {
        __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar no device color");
        return;
    }
    int result = streamColor->start();
    if (result) {
        __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar device color start failed");
        return;
    }
    VideoFrame colorFrame;
    int irWidth = 800;;
    int irHeight = 1280;;
    
    colorFrame.data = malloc(irWidth*irHeight*3/2);
    
    result = streamColor->readFrame(&colorFrame);
    if (result) {

    } else {
        sendData((unsigned char*)colorFrame.data);
    }
    
    free(colorFrame.data);
    
    
}

void fmEngine::sendData(unsigned char *_data) {
    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::sendData()");
    if (nullptr == _data) {
        NSLog("shiyang _data is nullptr");
        return;
    }

    unsigned char *yData = new unsigned char[yuvWidth*yuvHeight];
    unsigned char *uvData = new unsigned char[yuvWidth*yuvHeight/2];
    
    memcpy(yData, _data, yuvWidth*yuvHeight);
    memcpy(uvData, _data+yuvWidth*yuvHeight, yuvWidth*yuvHeight/2);
    
//    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::y0=%u<-%u", yData[0], _data[0]);
//    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::y100=%u<-%u", yData[100], _data[0]);
//    __android_log_print(ANDROID_LOG_ERROR, "ar", "shiyang ar mEngine::y1000=%u<-%u", yData[1000], _data[0]);
    
    glBindTexture(GL_TEXTURE_2D, yTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yuvWidth, yuvHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, yData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindTexture(GL_TEXTURE_2D,uvTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,yuvWidth/2,yuvHeight/2,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE, uvData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
//    delete[] fakeData;
    delete[] yData;
    delete[] uvData;
    
//    memcpy(grayData, _data, 720*1280);
//    drawGray->sendGrayData(grayData);

//    memcpy(grayData, _data, 1280*720);
//    memcpy(rgbaData, grayData, 1280*720);
//    memcpy(grayData+1280*720, grayData, 1280*720);
//    memcpy(grayData+1280*720*2, grayData, 1280*720);

//    int len = 1280*720*3/2;

//    memcpy(data, _data, len);

//    clock_gettime(CLOCK_MONOTONIC, &now);
//    long long int time_begin = now.tv_sec * 1000000000LL + now.tv_nsec;

//    std::vector<FacialInfo> result = fm->FacialLandmarkDetectInVideo((char *)_data, 1280, 720, 1280*sizeof(char), 1, faceOritation(3));
//    std::vector<FacialInfo> result = fm->FacialLandmarkDetectInVideo(data, 720, 1280, 720*sizeof(char), 1, faceOritation(0));

//    clock_gettime(CLOCK_MONOTONIC, &now);//yang
//    long long int time_end = now.tv_sec * 1000000000LL + now.tv_nsec;//yang
//    float time_delta = (time_end - time_begin)/1000000.0f;
//    int m_face_count = result.size();
//    if (m_face_count) {
//        NSLog("time_delta %f", time_delta);
//    } else {
//        NSLog("time_delta -1");
//    }


//    float facePoints[68*2];
//    if (m_face_count) {
//        for (int j = 0; j < result.at(0).facialLandMarklist.size(); ++j) {
//            PtInfo ptInfo = result.at(0).facialLandMarklist.at(j);
//
//            facePoints[j*2+0] = -(ptInfo.y/720.0f-0.5f)*2.0f;
//            facePoints[j*2+1] = (ptInfo.x/1280.0f-0.5f)*2.0f;
//        }
//        drawPoints->sendFacePoints(facePoints);
//    } else {
//        drawPoints->sendFacePoints(nullptr);
//    }

}

void fmEngine::drawTex(int id) {
//    if (useSDYUV) {
//        testYUV();
//        useSDYUV = false;
//        return;
//    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 720, 1280);
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
//    drawYUV->setRotation(90);
    drawYUV->drawTextureYUV(showTex, yTex, uvTex);
    
//    draw->setRotation(90);
//    draw->drawTextrue(id);
//
////    glBindTexture(GL_TEXTURE_2D,grayTex);
////    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 720,1280,GL_RGBA,GL_UNSIGNED_BYTE,rgbaData);
//
//    drawGray->setRotation(90);
//    drawGray->drawTextrue(id);
//
//    drawPoints->setRotation(90);
//    drawPoints->drawPoints(id);
}


