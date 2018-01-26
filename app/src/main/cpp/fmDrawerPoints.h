//
// Created by menghai on 16/7/14.
//

#ifndef MY_APPLICATION_FMDRAWERPOINTS_H
#define MY_APPLICATION_FMDRAWERPOINTS_H


#include "fmPlatform_ANDROID.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "fmProgram.h"
#include <string.h>
#include <vector>
#include <math.h>




#define NUM_FACE_POINTS 68

    extern const char *m_fsh_points;
    extern const char *m_vsh_points;



    class fmDrawerPoints {




    private:

        GLuint mRotLoc, mFlipScaleLoc;


    public:
          ~fmDrawerPoints(){
              release();

          }
         fmDrawerPoints(){


         }

        static fmDrawerPoints *create();
        bool init(const char **vsh, const char **fsh);

        virtual bool initShader(const char **vsh, const char **fsh);
        virtual void setRotation(GLfloat rad);
        virtual void drawPoints(int texID);
        virtual void drawTextrue(int texID);
        virtual void drawTextrue2D(int texID);
        virtual void release();

        virtual void sendFacePoints(float *_points);

        void setFilpScale(GLfloat x, GLfloat y);



         const float verticeArray[8]={
                -1.0f, -1.0f,
                1.0, -1.0f,
                1.0, 1.0,
                -1.0f, 1.0

        };

        const float verticePointsArray[16]={
              0.125f*0.0f, -0.125f*0.0f,
              0.125f*1.0f, -0.125f*1.0f,
              0.125f*2.0f, -0.125f*2.0f,
              0.125f*3.0f, -0.125f*3.0f,
              0.125f*4.0f, -0.125f*4.0f,
              0.125f*5.0f, -0.125f*5.0f,
              0.125f*6.0f, -0.125f*6.0f,
              0.125f*7.0f, -0.125f*7.0f,
        };

        float verticeFacePointsArray[NUM_FACE_POINTS*2];

        virtual bool  loaderShader();

    protected:
        virtual void attuibuteProgram();
        virtual void loaderProgram();
        virtual void loaderTextre();
        virtual void loaderVerBuffer();
        virtual  void loaderTexBuffer();

        std::vector<GLuint > texIdArray;

        bool mHasFacePoints;
        fmProgram *program;
        fmProgram *program2d;
         GLuint verBufferId=0;
         GLuint texBufferId=0;

    };


#endif //MY_APPLICATION_FMDRAWERPOINTS_H
