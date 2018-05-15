//
// Created by pc on 2017/11/30.
//

#include "vertexbuffer.h"
#include "utils.h"

VertexBuffer::VertexBuffer()
{
    mVBO = 0;
    mVertexCount = 0;
    mVertexes = nullptr;

    mBoneCountsArray.clear();
    mBoneCountsArray.reserve(100000);
    mBoneIdsArray.clear();
    mBoneIdsArray.reserve(100000);
    mBoneWeightArray.clear();
    mBoneWeightArray.reserve(100000);

    mBoneIndexArray.clear();
    mBoneIndexArray.reserve(10);
    mBoneWorldModelMatrixArray.clear();
    mBoneWorldModelMatrixArray.reserve(10);
    mBoneOffsetMatrixArray.clear();
    mBoneOffsetMatrixArray.reserve(10);

}

VertexBuffer::~VertexBuffer()
{
    if(nullptr != mVertexes)
    {
        delete[] mVertexes;
    }

}

void VertexBuffer::SetSize(int vertexCount)
{
    mVertexCount = vertexCount;

    mVertexes = new VertexData[mVertexCount];
    memset(mVertexes, 0, sizeof(VertexData) * mVertexCount);

    mBoneInfo = new BoneInfo[mVertexCount];


    // 初始化VBO, 可以先不指定数据
    mVBO = CreateBufferObject(GL_ARRAY_BUFFER, GetByteSize(), GL_STATIC_DRAW, nullptr);
}

void VertexBuffer::SetPosition(int index, float x, float y, float z, float w) {
    if (index < 0 || index >= mVertexCount)
    {
        return;
    }

    mVertexes[index].Position[0] = x;
    mVertexes[index].Position[1] = y;
    mVertexes[index].Position[2] = z;
    mVertexes[index].Position[3] = w;
}

void VertexBuffer::SetColor(int index, float r, float g, float b, float a) {
    if (index < 0 || index >= mVertexCount)
    {
        return;
    }

    mVertexes[index].Color[0] = r;
    mVertexes[index].Color[1] = g;
    mVertexes[index].Color[2] = b;
    mVertexes[index].Color[3] = a;
}

void VertexBuffer::SetTexcoord(int index, float x, float y) {
    if (index < 0 || index >= mVertexCount)
    {
        return;
    }

    mVertexes[index].Texcoord[0] = x;
    mVertexes[index].Texcoord[1] = y;
}

void VertexBuffer::SetNormal(int index, float x, float y, float z)
{
    if (index < 0 || index >= mVertexCount)
    {
        return;
    }

    mVertexes[index].Normal[0] = x;
    mVertexes[index].Normal[1] = y;
    mVertexes[index].Normal[2] = z;
}

void VertexBuffer::SetMeshInfoId(int index)
{
    if (index < 0 || index >= mVertexCount)
    {
        return;
    }

    mVertexes[index].meshId = index;

}

int VertexBuffer::GetByteSize()
{
    return (sizeof(VertexData) * mVertexCount);
}

void VertexBuffer::Bind()
{
    if (0 == mVBO)
    {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, GetByteSize(), mVertexes);
}

void VertexBuffer::Unbind()
{
    if (0 == mVBO)
    {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexData & VertexBuffer::GetVertex(int index)
{
    return mVertexes[index];
}


// skin info
void VertexBuffer::BlendVertex(int vertexIndex,VertexBuffer* afterVertexBuffer)
{
#if 0
    for(int i=0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
    {
        print_mat(mBoneInfo[vertexIndex].m_bones[i]->mWorldTranslateMatrix);
        memcpy(&(mVertexes->BoneWorldMatrix[i]), glm::value_ptr(mBoneInfo[vertexIndex].m_bones[i]->mWorldTranslateMatrix), sizeof(float) * 16);
        print_array(mVertexes->BoneWorldMatrix[i]);


        LOGE("-----------------------------\n");
        print_mat(mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);
        memcpy(mVertexes->BoneOffsetMatrix[i], glm::value_ptr(mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix), sizeof(float) * 16);
        print_array(mVertexes->BoneOffsetMatrix[i]);
    }

    //do the vertex blending,get the vertex's pos in world space
    glm::vec4 vAfterVc = glm::vec4();
    for(int i=0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
    {
        //
        glm::mat4 mat =  glm::mul(mBoneInfo[vertexIndex].m_bones[i]->mWorldTranslateMatrix, mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);
        LOGE("luoyouren1----------\n");
        print_mat(mat);
        LOGE("luoyouren2----------\n");
        VertexData vdIndex = mVertexes[vertexIndex];
        glm::vec4 vec41= glm::vec4(vdIndex.Position[0],vdIndex.Position[1],vdIndex.Position[2],vdIndex.Position[3]);
        glm::vec4 newVector  = glm::mul(mat,vec41);

        newVector = newVector * mBoneInfo[vertexIndex].m_boneWeights[i];
        newVector.w = 1;

        vAfterVc += newVector ;
    }
    afterVertexBuffer->SetPosition(vertexIndex, vAfterVc.x, vAfterVc.y, vAfterVc.z );
#endif
}
