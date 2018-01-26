//
// Created by menghai on 16/7/14.
//

#ifndef MY_APPLICATION_FMPROGRAM_H
#define MY_APPLICATION_FMPROGRAM_H

#include "GLES2/gl2.h"
#include "fmShaderLoader.h"


    class fmProgram {




    public:
        static fmProgram *create(const GLchar **  vsh, const GLchar ** FSH);

        bool init(const GLchar ** vsh, const GLchar ** fsh);

        bool init(const GLchar ** vsh, const GLchar ** fsh, GLuint programID);

        void bind();

        int getUniformloc(const GLchar * name);
        void sendUniformi(const GLchar * name,GLint x);
        void bindAttribLocation(char* name, int index);
        void release();

        ~fmProgram();
        fmProgram();

        int getProgramID();

    private:
        GLuint m_program_id;
        fmShaderLoader *mVertexShader= nullptr, *mFragmentShader= nullptr;


    };


#endif //MY_APPLICATION_FMPROGRAM_H
