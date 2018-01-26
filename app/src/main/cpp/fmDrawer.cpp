//
// Created by menghai on 16/7/14.
//


#include "fmDrawer.h"
#include "fmPlatform_ANDROID.h"


const char *m_fsh = ""
        "#extension GL_OES_EGL_image_external : require \n "
        "precision mediump float;\n"
        "varying vec2 texCoord; \n "
        "uniform samplerExternalOES inputImageTexture;\n"
        "void main() {\n "
        "    gl_FragColor = texture2D(inputImageTexture, texCoord);\n"
//        "    vec4 temp = texture2D(inputImageTexture, texCoord);\n"
//        "    float gray = temp.r*0.299+temp.g*0.587+temp.b*0.114;\n"
//        "    gl_FragColor = vec4(gray, gray, gray, temp.a);\n"
        "}";
const char *m_fsh_2d =
        "#extension GL_OES_EGL_image_external : require \n "
        "precision mediump float;\nvarying vec2 texCoord; \n "
        "uniform sampler2D inputImageTexture;\nvoid main() {\n "
        "    gl_FragColor = texture2D(inputImageTexture, texCoord);\n"
        "}";


const char *m_vsh = FM_SHADER_STRING (
        attribute vec2 vPosition;
        varying vec2 texCoord;
        uniform mat2 rotation;
        uniform vec2 flipScale;
        void main() {
            gl_Position = vec4(vPosition, 0.0, 1.0);
            texCoord = flipScale * (vPosition / 2.0 * rotation) + 0.5;
            gl_PointSize = 20.0;
        });

fmDrawer *fmDrawer::create() {
    fmDrawer *draw = new fmDrawer();
    if (!draw->init((const char **) &m_vsh, (const char **) &m_fsh)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        draw->release();
        draw = nullptr;
    }

    return draw;

}

void fmDrawer::release() {
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


bool fmDrawer::initShader(const char **vsh, const char **fsh) {

    if (!init((const char **) vsh, (const char **) fsh)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        release();
        return false;
    }
    return true;
}

bool fmDrawer::loaderShader() {


    return initShader((const char **) &m_vsh, (const char **) &m_fsh);

}


bool fmDrawer::init(const char **vsh, const char **fsh) {
    fmProgram *program = new fmProgram();
    fmDrawer::program = program;
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
    program2d->init(vsh, &m_fsh_2d);
    program2d->bind();


    return true;

}

void fmDrawer::loaderVerBuffer() {
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    verBufferId = vertBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glBufferData(GL_ARRAY_BUFFER, 32, verticeArray, GL_STATIC_DRAW);
}

void fmDrawer::loaderTexBuffer() {
}


void fmDrawer::attuibuteProgram() {
}

void fmDrawer::drawPoints(int texID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void fmDrawer::drawTextrue(int texID) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    glDrawArrays(GL_POINTS, 0, 4);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void fmDrawer::drawTextrue2D(int texID) {
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

void fmDrawer::setFilpScale(GLfloat x, GLfloat y) {
    program->bind();
    glUniform2f(mFlipScaleLoc, x, y);

}

void fmDrawer::setRotation(GLfloat rad) {
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

void fmDrawer::loaderProgram() {


}

void fmDrawer::loaderTextre() {


}



