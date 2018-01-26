//
// Created by menghai on 16/7/14.
//


#include "fmDrawerYUV.h"
#include "fmPlatform_ANDROID.h"


const char *m_fsh_yuv = ""
//        "#extension GL_OES_EGL_image_external : require \n "
        "precision highp float;\n"
        "varying vec2 texCoord; \n"
        "uniform sampler2D inputImageTextureY;\n"
        "uniform sampler2D inputImageTextureUV;\n"
        "void main() {\n "
        "float y = texture2D(inputImageTextureY, texCoord).r;\n"
        "float u = texture2D(inputImageTextureUV, texCoord).r - 0.5;\n"
        "float v = texture2D(inputImageTextureUV, texCoord).a - 0.5;\n"
//        "u = 0.0;\n"
//        "v = 0.0;\n"
        "float r = y             + 1.402 * v;\n"
        "float g = y - 0.344 * u - 0.714 * v;\n"
        "float b = y + 1.772 * u;\n"
        "\n"
        "\n"
        "gl_FragColor = vec4(r, g, b, 1.0);\n"
        "}";

const char *m_fsh_2d_yuv = ""
        "#extension GL_OES_EGL_image_external : require \n "
        "precision mediump float;\n"
        "varying vec2 texCoord; \n "
        "uniform sampler2D inputImageTexture;\n"
        "void main() {\n "
        "gl_FragColor = texture2D(inputImageTexture, texCoord);\n"
        "}";

const char *m_vsh_yuv = FM_SHADER_STRING (
        attribute vec2 vPosition;
        varying vec2 texCoord;
        uniform mat2 rotation;
        uniform vec2 flipScale;
        void main() {
            gl_Position = vec4(vPosition, 0.0, 1.0);

//            texCoord = flipScale * (vPosition / 2.0 * rotation) + 0.5;
            texCoord = vPosition;
//            texCoord = flipScale * (vPosition * rotation * 2.0) - 1.0;

//            gl_PointSize = 20.0;
        });

fmDrawerYUV *fmDrawerYUV::create() {
    fmDrawerYUV *draw = new fmDrawerYUV();
    if (!draw->init((const char **) &m_vsh_yuv, (const char **) &m_fsh_yuv)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        draw->release();
        draw = nullptr;
    }

    return draw;

}

void fmDrawerYUV::release() {
    if (nullptr != program) {
        delete program;
        program = nullptr;
    }

    if (verBufferId != 0) {
        glDeleteBuffers(1, &verBufferId);
        verBufferId = 0;
    }
    if (texBufferId != 0) {
        glDeleteBuffers(1, &texBufferId);
        texBufferId = 0;
    }

    for (int i = 0; i < texIdArray.size(); ++i) {
        glDeleteTextures(1, &texIdArray[i]);

    }

    texIdArray.clear();


}


bool fmDrawerYUV::initShader(const char **vsh, const char **fsh) {

    if (!init((const char **) vsh, (const char **) fsh)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        release();
        return false;
    }
    return true;
}

bool fmDrawerYUV::loaderShader() {


    return initShader((const char **) &m_vsh_yuv, (const char **) &m_fsh_yuv);

}


bool fmDrawerYUV::init(const char **vsh, const char **fsh) {
    fmProgram *program = new fmProgram();
    fmDrawerYUV::program = program;
    attuibuteProgram();
    program->init(vsh, fsh);
    program->bind();

    char *name = (char *) "rotation";
    mRotLoc = program->getUniformloc(name);

    name = (char *) "flipScale";
    mFlipScaleLoc = program->getUniformloc(name);
    loaderProgram();
    loaderTextre();
    loaderVerBuffer();
    loaderTexBuffer();
    setFilpScale(1.0, 1.0);
    setRotation(0);

    //
    program2d = new fmProgram();
    program2d->init(vsh, &m_fsh_2d_yuv);
    program2d->bind();


    return true;

}

void fmDrawerYUV::loaderVerBuffer() {
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    verBufferId = vertBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glBufferData(GL_ARRAY_BUFFER, 32, verticeArray, GL_STATIC_DRAW);
}

void fmDrawerYUV::loaderTexBuffer() {
}


void fmDrawerYUV::attuibuteProgram() {
}


void fmDrawerYUV::drawPoints(int texID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fmDrawerYUV::drawTextrue(int texID) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void fmDrawerYUV::drawTextrue2D(int texID) {
    //shader update
    program2d->bind();
    //
    int tFlipScaleLoc = program->getUniformloc((char *) "flipScale");
    glUniform2f(tFlipScaleLoc, 1.0f, 1.0f);
    //
    float rad = 180.0f;
    rad = rad * M_PI / 180;
    float cosRad = cos(rad);
    float sinRad = sin(rad);
    float rotation[] = {
            cosRad, sinRad,
            -sinRad, cosRad
    };
    int tRotLoc = program->getUniformloc( (char *) "rotation");
    glUniformMatrix2fv(tRotLoc, 1, false, rotation);

    //render
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fmDrawerYUV::drawTextureYUV(GLuint _showTexId, GLuint _yTexId, GLuint _uvTexId)
{
    program->bind();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, _showTexId);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _yTexId);
    glUniform1i(glGetUniformLocation(program->getProgramID(), "inputImageTextureY"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _uvTexId);
    glUniform1i(glGetUniformLocation(program->getProgramID(), "inputImageTextureUV"), 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fmDrawerYUV::setFilpScale(GLfloat x, GLfloat y) {
    program->bind();
    glUniform2f(mFlipScaleLoc, x, y);

}

void fmDrawerYUV::setRotation(GLfloat rad) {
    rad = rad * M_PI / 180;
    float cosRad = cos(rad);
    float sinRad = sin(rad);

    float rotation[] = {
            cosRad, sinRad,
            -sinRad, cosRad
    };
    program->bind();

    glUniformMatrix2fv(mRotLoc, 1, false, rotation);

}

void fmDrawerYUV::loaderProgram() {


}

void fmDrawerYUV::loaderTextre() {


}



