//
// Created by menghai on 16/7/14.
//

#ifndef MY_APPLICATION_FMDRAWER_H
#define MY_APPLICATION_FMDRAWER_H


#include "fmPlatform_ANDROID.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "fmProgram.h"
#include <string.h>
#include <vector>
#include <math.h>





    extern const char *m_fsh;
    extern const char *m_vsh;



    class fmDrawer {




    private:

        GLuint mRotLoc, mFlipScaleLoc;


    public:
          ~fmDrawer(){
              release();

          }
         fmDrawer(){


         }

        static fmDrawer *create();
        bool init(const char **vsh, const char **fsh);

        virtual bool initShader(const char **vsh, const char **fsh);
        virtual void setRotation(GLfloat rad);
        virtual void drawPoints(int texID);
        virtual void drawTextrue(int texID);
        virtual void drawTextrue2D(int texID);
        virtual void release();



        void setFilpScale(GLfloat x, GLfloat y);



         const float verticeArray[8]={
                -1.0, -1.0,
                1.0, -1.0,
                1.0, 1.0,
                -1.0f, 1.0

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


#endif //MY_APPLICATION_FMDRAWER_H
