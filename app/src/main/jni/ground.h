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

    //注意模型矩阵肯定是模型自己的，需要放在模型类里面
    glm::mat4 mModelMatrix;

public:
    void Init(AAssetManager *assetManager);
    void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

};


#endif //GLDEMO_GROUND_H
