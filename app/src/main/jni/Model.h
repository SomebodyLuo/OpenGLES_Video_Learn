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


    virtual void Init(AAssetManager *assetManager, const char *modelPath);

    void Update(float deltaTime);

    virtual void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

    void SetPosition(float x, float y, float z);

    void SetAmbientMaterial(float r, float g, float b, float a);

    void SetDiffuseMaterial(float r, float g, float b, float a);

    void SetSpecularMaterial(float r, float g, float b, float a);

    void SetTexure(const char *imagePath);

    void SetVec4(const char *name, float x, float y, float z, float w);
};


#endif //GLDEMO_MODEL_H
