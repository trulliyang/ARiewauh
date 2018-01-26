//
// Created by menghai on 16/7/14.
//


#include "fmProgram.h"
#include "fmPlatform_ANDROID.h"



    fmProgram::fmProgram() : m_program_id(glCreateProgram()), mVertexShader(nullptr),
                             mFragmentShader(
                                     nullptr) {
    }

    fmProgram::~fmProgram() {
        release();
    }

    fmProgram *fmProgram::create(const GLchar **vsh, const GLchar **fsh) {

        fmProgram *program = new fmProgram();

        if (!program->init(vsh, fsh)) {
            program->release();
            program = nullptr;
        }
        return program;


    }










    void fmProgram::bind() {

        glGetError();
        glUseProgram(m_program_id);


    }


    bool fmProgram::init(const GLchar **vsh, const GLchar **fsh) {



        return init(vsh, fsh, m_program_id);


    }

    bool fmProgram::init(const GLchar **vsh, const GLchar **fsh, GLuint programID) {

        if (m_program_id == 0) {
            m_program_id = glCreateProgram();

        }
        if (m_program_id == 0) {
            FM_LOG_ERROR("Invalid Program ID! Check if the context is binded!");

        }
        if (mVertexShader != nullptr) {
            delete mVertexShader;
            mVertexShader = nullptr;
        }

        if (mFragmentShader != nullptr) {

            delete mFragmentShader;
            mFragmentShader = nullptr;
        }

        mVertexShader = new fmShaderLoader(vsh, GL_VERTEX_SHADER);
        mFragmentShader = new fmShaderLoader(fsh, GL_FRAGMENT_SHADER);

        glAttachShader(programID, mVertexShader->shaderID());
        glAttachShader(programID, mFragmentShader->shaderID());
        glLinkProgram(programID);

        GLint compiled = 0;
        glGetProgramiv(programID, GL_COMPILE_STATUS, &compiled);

        if (compiled != GL_TRUE) {


            GLint loglen;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &loglen);
            if (loglen > 0) {
                char *buf = new char[loglen+1];
                if (buf != nullptr) {
                    glGetProgramInfoLog(programID, loglen, 0, buf);
                    FM_LOG_ERROR("program %d compile faild: \n%s\n", 1, buf);
                    delete[] buf;

                }
            }
        }
        if (programID != m_program_id && programID != 0) {

            glDeleteProgram(m_program_id);

        }

        if (m_program_id == programID) {
            return true;
        }
        m_program_id = programID;
        return true;
    }

    int fmProgram::getUniformloc(const GLchar *name) {
        GLint uniform = glGetUniformLocation(m_program_id, name);
        if (uniform < 0) {
            FM_LOG_INFO("uniform name %s does not exist", name);


        }
        return uniform;

    }


    void fmProgram::sendUniformi(const GLchar *name, GLint x) {
        glUniform1i(getUniformloc(name), x);


    }


    void fmProgram::bindAttribLocation(char *name, int index) {
        glBindAttribLocation(m_program_id, index, name);


    }


    void fmProgram::release() {

        if (m_program_id != 0) {

            glDeleteProgram(m_program_id);
            m_program_id=0;

            if (nullptr != mVertexShader) {
                delete mVertexShader;
                mVertexShader = nullptr;
            }
            if (nullptr != mFragmentShader) {
                delete mFragmentShader;
                mFragmentShader = nullptr;
            }
        }


    }

    int fmProgram::getProgramID() {
        return m_program_id;
    }

