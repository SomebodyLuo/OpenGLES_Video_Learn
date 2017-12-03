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

struct UniformVector4f
{
    GLint mLocation;
    float v[4];

    UniformVector4f()
    {
        mLocation = -1;
        memset(v, 0, sizeof(float) * 4);
    }
};


class Shader {
public:
    Shader();

    AAssetManager *mAssetManager;

    GLuint mProgram;
    std::map<std::string, UniformTexture *> mUniformTextures;
    std::map<std::string, UniformVector4f *> mUniformVec4s;

    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;

    bool Init(AAssetManager *assetManager, const char*vs, const char *fs);
    void Bind(float *M, float *V, float *P);

    void SetTexture(const char *name, const char *imagePath);

    void SetVec4(const char *name, float x, float y, float z, float w);
};


#endif //GLDEMO_SHADER_H
