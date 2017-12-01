//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_SHADER_H
#define GLDEMO_SHADER_H

#include "ggl.h"

struct UniformTexture
{
    UniformTexture()
    {
        mLocation = -1;
        mTexture = 0;
    }

    GLint mLocation;
    GLuint mTexture;

};



class Shader {
public:
    Shader();

    AAssetManager *mAssetManager;

    GLuint mProgram;
    UniformTexture mTexture;
    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;

    bool Init(AAssetManager *assetManager, const char*vs, const char *fs);
    void Bind(float *M, float *V, float *P);

    void SetTexture(const char *name, const char *imagePath);
};


#endif //GLDEMO_SHADER_H
