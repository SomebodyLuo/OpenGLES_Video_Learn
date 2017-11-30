//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_GROUND_H
#define GLDEMO_GROUND_H

#include "utils.h"
#include "vertexbuffer.h"
#include "shader.h"

class Ground
{
public:
    Ground();

private:
    VertexBuffer *mVertexBuffer;
    GLuint mVBO;

    Shader *mShader;

public:
    void Init(AAssetManager *assetManager);

};


#endif //GLDEMO_GROUND_H
