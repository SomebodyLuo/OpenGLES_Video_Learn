//
// Created by pc on 2017/12/5.
//

#ifndef GLDEMO_PARTICLESYSTEM_H
#define GLDEMO_PARTICLESYSTEM_H

#include "vertexbuffer.h"
#include "shader.h"

class ParticleSystem {
    VertexBuffer *mVertexBuffer;
    glm::mat4 mModelMatrix;
    Shader *mShader;

public:
    ParticleSystem();

    void Init(AAssetManager *assetManager, float x, float y, float z);
    void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
    void Update(float deltaTime);
};


#endif //GLDEMO_PARTICLESYSTEM_H
