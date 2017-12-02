//
// Created by pc on 2017/12/1.
//

#ifndef GLDEMO_MODEL_H
#define GLDEMO_MODEL_H


#include "ggl.h"
#include "vertexbuffer.h"
#include "shader.h"

class Model {

public:
    Model();

    VertexBuffer *mVertexBuffer;

    Shader *mShader;

    glm::mat4 mModelMatrix;


    void Init(AAssetManager *assetManager, const char *modelPath);

    void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

    void SetPosition(float x, float y, float z);
};


#endif //GLDEMO_MODEL_H
