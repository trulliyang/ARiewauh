//
// Created by menghai on 16/7/14.
//


#include "fmDrawerPoints.h"
#include "fmPlatform_ANDROID.h"


const char *m_fsh_points = ""
        "#extension GL_OES_EGL_image_external : require \n "
        "void main() {\n "
        "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}";
const char *m_fsh_2d_points = "#extension GL_OES_EGL_image_external : require \n precision mediump float;\nvarying vec2 texCoord; \n uniform sampler2D inputImageTexture;\nvoid main() {\n gl_FragColor = texture2D(inputImageTexture, texCoord);\n}";


const char *m_vsh_points = FM_SHADER_STRING (
        attribute vec2 vPosition;
        void main() {
            gl_PointSize = 10.0;
            gl_Position = vec4(vPosition, 0.0, 1.0);
        });

fmDrawerPoints *fmDrawerPoints::create() {
    fmDrawerPoints *draw = new fmDrawerPoints();
    if (!draw->init((const char **) &m_vsh_points, (const char **) &m_fsh_points)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        draw->release();
        draw = nullptr;
    } else {
        FM_LOG_INFO("FMDrawer create succeed!");
    }

    return draw;

}

void fmDrawerPoints::release() {
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


bool fmDrawerPoints::initShader(const char **vsh, const char **fsh) {

    if (!init((const char **) vsh, (const char **) fsh)) {
        FM_LOG_INFO("FMDrawer createfailed!");
        release();
        return false;
    } else {
        FM_LOG_INFO("FMDrawer create succeed!");
    }
    return true;
}

bool fmDrawerPoints::loaderShader() {


    return initShader((const char **) &m_vsh_points, (const char **) &m_fsh_points);

}


bool fmDrawerPoints::init(const char **vsh, const char **fsh) {
    fmProgram *program = new fmProgram();
    fmDrawerPoints::program = program;
    attuibuteProgram();
    program->init(vsh, fsh);
    program->bind();

//    char *name = (char *) "rotation";
//    mRotLoc = program->getUniformloc(name);
//
//    name = (char *) "flipScale";
//    mFlipScaleLoc = program->getUniformloc(name);
    loaderProgram();
    loaderTextre();
    loaderVerBuffer();
    loaderTexBuffer();
//    setFilpScale(1.0, 1.0);
    setRotation(0);

    //
    program2d = new fmProgram();
    program2d->init(vsh, &m_fsh_2d_points);
    program2d->bind();

    return true;
}

void fmDrawerPoints::loaderVerBuffer() {
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    verBufferId = vertBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    memset(verticeFacePointsArray, 1, sizeof(verticeFacePointsArray));
//    glBufferData(GL_ARRAY_BUFFER, 32, verticeArray, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, 64, verticePointsArray, GL_STATIC_DRAW);
    mHasFacePoints = false;
    verticeFacePointsArray[0] = 0.5f;
    verticeFacePointsArray[1] = 0.5f;
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticeFacePointsArray), verticeFacePointsArray, GL_DYNAMIC_DRAW);
}

void fmDrawerPoints::loaderTexBuffer() {
}


void fmDrawerPoints::attuibuteProgram() {
}

void fmDrawerPoints::sendFacePoints(float *_points) {
    if (nullptr == _points) {
        mHasFacePoints = false;
        return;
    }
    memcpy(verticeFacePointsArray, _points, sizeof(verticeFacePointsArray));
    mHasFacePoints = true;
}


void fmDrawerPoints::drawPoints(int texID) {
    if (!mHasFacePoints) {
        return;
    }
    glEnable(GL_POINTS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticeFacePointsArray), verticeFacePointsArray, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
//    glDrawArrays(GL_POINTS, 0, 8);
    glDrawArrays(GL_POINTS, 0, NUM_FACE_POINTS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void fmDrawerPoints::drawTextrue(int texID) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texID);
    glBindBuffer(GL_ARRAY_BUFFER, verBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
    program->bind();
    glDrawArrays(GL_POINTS, 0, 8);

}

void fmDrawerPoints::drawTextrue2D(int texID) {
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

void fmDrawerPoints::setFilpScale(GLfloat x, GLfloat y) {
    program->bind();
    glUniform2f(mFlipScaleLoc, x, y);

}

void fmDrawerPoints::setRotation(GLfloat rad) {
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

void fmDrawerPoints::loaderProgram() {


}

void fmDrawerPoints::loaderTextre() {


}



