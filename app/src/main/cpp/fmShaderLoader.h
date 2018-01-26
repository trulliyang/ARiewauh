//
// Created by menghai on 16/7/14.
//

#ifndef MY_APPLICATION_FMSHADERLOADER_H
#define MY_APPLICATION_FMSHADERLOADER_H

#include <GLES2/gl2.h>
#include "fmPlatform_ANDROID.h"

    class fmShaderLoader {



    private:
        GLuint shader_type;
        GLuint shader_id;
    public:
        bool  init(const GLchar ** shaderCode,GLuint shaderType);
        void release();
        GLuint loadShaser(GLuint type,const GLchar ** code);
        GLuint shaderID();
        fmShaderLoader(const GLchar **shaderCode,GLuint shaderType);
        fmShaderLoader();
        ~fmShaderLoader();



    };


#endif //MY_APPLICATION_FMSHADERLOADER_H
