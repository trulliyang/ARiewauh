//
// Created by menghai on 16/7/14.
//

#ifndef MY_APPLICATION_FMDRAWERGRAY_H
#define MY_APPLICATION_FMDRAWERGRAY_H


#include "fmPlatform_ANDROID.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "fmProgram.h"
#include <string.h>
#include <vector>
#include <math.h>





    extern const char *m_fsh_gray;
    extern const char *m_vsh_gray;



    class fmDrawerGray {




    private:

        GLuint mRotLoc, mFlipScaleLoc;


    public:
          ~fmDrawerGray(){
              release();

          }
        fmDrawerGray(){


         }

        static fmDrawerGray *create();
        bool init(const char **vsh, const char **fsh);

        virtual bool initShader(const char **vsh, const char **fsh);
        virtual void setRotation(GLfloat rad);
        virtual void drawPoints(int texID);
        virtual void drawTextrue(int texID);
        virtual void drawTextrue2D(int texID);
        virtual void release();

        void setFilpScale(GLfloat x, GLfloat y);

//         const float verticeArray[8]={
//                -1.0, -1.0,
//                1.0, -1.0,
//                1.0, 1.0,
//                -1.0f, 1.0
//
//        };

//        const float verticeArray[8]={
//                0.5f, 0.5f,
//                1.0f, 0.5f,
//                1.0f, 1.0f,
//                0.5f, 1.0f
//
//        };


        const float verticeArray[8]={
                0.5f-1.5f, 0.5f,
                1.0f-1.5f, 0.5f,
                1.0f-1.5f, 1.0f,
                0.5f-1.5f, 1.0f

        };

        virtual bool  loaderShader();

    protected:
        virtual void attuibuteProgram();
        virtual void loaderProgram();
        virtual void loaderTextre();
        virtual void loaderVerBuffer();
        virtual  void loaderTexBuffer();

        std::vector<GLuint > texIdArray;


        fmProgram *program;
        fmProgram *program2d;
         GLuint verBufferId=0;
         GLuint texBufferId=0;
    };


#endif //MY_APPLICATION_FMDRAWERGRAY_H
