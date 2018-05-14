//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_VERTEXBUFFER_H
#define GLDEMO_VERTEXBUFFER_H

#include "ggl.h"
#include "BoneInfo.h"

class VertexData
{
public:
    float Position[4];
    float Color[4];
    float Texcoord[4];
    float Normal[4];
    float meshId;

};

class VertexBuffer
{
public:
    int mVertexCount;
    VertexData *mVertexes;
    BoneInfo *mBoneInfo;

    //----------------------------------------------------------------------------------------------
    // 每个顶点拥有的骨骼数量、骨骼ID、骨骼权重
    std::vector<int> mBoneCountsArray;
    std::vector<glm::ivec4> mBoneIdsArray;
    std::vector<glm::vec4> mBoneWeightArray;

    // 所有骨骼的信息：ID、worldMatrix、OffsetMatrix
    std::vector<int> mBoneIndexArray;
    std::vector<glm::mat4> mBoneWorldModelMatrixArray;
    std::vector<glm::mat4> mBoneOffsetMatrixArray;
    //----------------------------------------------------------------------------------------------

    GLuint mVBO;

    VertexBuffer();
    ~VertexBuffer();

    void SetSize(int vertexCount);
    void SetPosition(int index, float x, float y, float z, float w = 1.0f);
    void SetColor(int index, float r, float g, float b, float a = 1.0f);
    void SetTexcoord(int index, float x, float y);
    void SetNormal(int index, float x, float y, float z);
    void SetMeshInfoId(int index);

    int GetByteSize();

    void Bind();
    void Unbind();
    VertexData &GetVertex(int index);

    void BlendVertex(int vertexIndex ,VertexBuffer * afterBuffer);

};

#endif //GLDEMO_VERTEXBUFFER_H
