//
// Created by menghai on 16/7/14.
//

#include "fmShaderLoader.h"


    fmShaderLoader::~fmShaderLoader() {


        release();
    }

    fmShaderLoader::fmShaderLoader() { }

    fmShaderLoader::fmShaderLoader(const GLchar **shaderCode,GLuint shaderType) {


        init(shaderCode, shaderType);
    }

    bool fmShaderLoader::init(const GLchar **shaderCode, GLuint shaderType) {

        shader_type = shaderType;
        shader_id = loadShaser(shaderType, shaderCode);
        if (shader_type == 0) {
            FM_LOG_ERROR("GLCreateShader Failed!...");
            return false;
        }

        return true;


    }


    GLuint fmShaderLoader::loadShaser(GLuint type, const GLchar **code) {
        int shaderID = glCreateShader(type);

        if (shaderID == 0) {
            shaderID = glCreateShader(type);
            FM_LOG_CODE(
                    if (shaderID == 0) {
                        FM_LOG_ERROR("glCreateShader Failed!");
                        return false;
                    })
        }


        glShaderSource(shaderID, 1, code, nullptr);
        glCompileShader(shaderID);
        GLint compiled = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

        if (compiled == GL_TRUE) {
            return shaderID;
        }

        GLint loglen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &loglen);
        if (loglen > 0) {
            char *buf = new char[loglen];
            if (buf != nullptr) {
                glGetShaderInfoLog(shaderID, loglen, &loglen, buf);
                FM_LOG_ERROR("Shader %d compile faild: \n%s\n", shaderID, buf);
                delete[] buf;

            }
        }


        return shaderID;


    }

    void fmShaderLoader::release() {
        if (shader_id == 0) {

            return;
        }

        glDeleteShader(shader_id);


    }

    GLuint fmShaderLoader::shaderID() {


        return shader_id;
    }








