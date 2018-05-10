//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_VERTEXBUFFER_H
#define GLDEMO_VERTEXBUFFER_H

#include "ggl.h"
#include "Bone.h"
#include "BoneInfo.h"

class VertexData
{
public:
    float Position[4];
    float Color[4];
    float Texcoord[4];
    float Normal[4];
};

class VertexBuffer
{
public:
    int mVertexCount;
    VertexData *mVertexes;
    BoneInfo *mBoneInfo;
    glm::mat4 *mModelMatrix;

    GLuint mVBO;

    VertexBuffer();
    ~VertexBuffer();

    void SetSize(int vertexCount);
    void SetPosition(int index, float x, float y, float z, float w = 1.0f);
    void SetColor(int index, float r, float g, float b, float a = 1.0f);
    void SetTexcoord(int index, float x, float y);
    void SetNormal(int index, float x, float y, float z);

    int GetByteSize();

    void Bind();
    void Unbind();
    VertexData &GetVertex(int index);

    void BlendVertex(int vertexIndex);

};

#endif //GLDEMO_VERTEXBUFFER_H
