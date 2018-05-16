//
// Created by pc on 2018/5/15.
//

#include "ggl.h"
#include "vertexbuffer.h"
#include "shader.h"

#ifndef GLDEMO_BOX_H
#define GLDEMO_BOX_H


class Box {

public:
    Box();
    ~Box();

    int m_vertexNum;

    VertexBuffer *mVertexBuffer;

    Shader *mShader;

    glm::mat4 mModelMatrix;

    virtual void Init(AAssetManager *assetManager, const char *modelPath);

    virtual void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

    void animateBones();

    void ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix);

    void retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb);

    void UpdateMatrices();

    void SetPosition(float x, float y, float z);

    void SetAmbientMaterial(float r, float g, float b, float a);

    void SetDiffuseMaterial(float r, float g, float b, float a);

    void SetSpecularMaterial(float r, float g, float b, float a);

    void SetTexure(const char *imagePath);

    void SetVec4(const char *name, float x, float y, float z, float w);

    Bone* g_boneRoot, *g_boneVertex;
};


#endif //GLDEMO_BOX_H
