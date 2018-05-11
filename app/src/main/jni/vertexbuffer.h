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

    //----------------------------------------------------------------------
    float BoneNumber[4];
    float BoneIdArray[4];
    float BoneWeightArray[4];

    float BoneWorldMatrix[4][16];
//    float BoneWorldMatrix1[16];
//    float BoneWorldMatrix2[16];
//    float BoneWorldMatrix3[16];

    float BoneOffsetMatrix[4][16];
//    float BoneOffsetMatrix1[16];
//    float BoneOffsetMatrix2[16];
//    float BoneOffsetMatrix3[16];
    //----------------------------------------------------------------------
};

class VertexBuffer
{
public:
    int mVertexCount;
    VertexData *mVertexes;
    BoneInfo *mBoneInfo;
    glm::mat4 *mVertexMoveMatrix;

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

    void BlendVertex(int vertexIndex ,VertexBuffer * afterBuffer);

};

#endif //GLDEMO_VERTEXBUFFER_H
