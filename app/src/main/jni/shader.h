//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_SHADER_H
#define GLDEMO_SHADER_H

#include "ggl.h"

class Shader {
public:
    AAssetManager *mAssetManager;

    GLuint mProgram;
    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;

    Shader();

    bool Init(AAssetManager *assetManager, const char*vs, const char *fs);
    void Bind(float *M, float *V, float *P);
};


#endif //GLDEMO_SHADER_H
