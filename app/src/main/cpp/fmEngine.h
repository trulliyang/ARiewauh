//
// Created by 梦海 on 2017/4/1.
//

#ifndef APPMAGICSDEMO_FMENGINE_H
#define APPMAGICSDEMO_FMENGINE_H


#include "fmDrawer.h"
#include "fmDrawerGray.h"
#include "fmDrawerPoints.h"
#include "fmDrawerYUV.h"

class fmEngine {


public:
    fmDrawer *draw;
    fmDrawerYUV *drawYUV;
    fmDrawerGray *drawGray;
    fmDrawerPoints *drawPoints;

    void create();

    void getCameraData();
    
    void sendData(unsigned char *_data);

    void drawTex(int id);

    void testYUV();

protected:
    bool mIsDeviceColorOK;
    bool initDeviceColor();
    
    bool mIsDeviceDepthOK;
    bool initDeviceDepth();
};


#endif //APPMAGICSDEMO_FMENGINE_H
