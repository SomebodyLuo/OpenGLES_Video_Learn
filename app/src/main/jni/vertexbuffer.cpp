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
    mVertexMoveMatrix = nullptr;
}

VertexBuffer::~VertexBuffer()
{
    if(nullptr != mVertexes)
    {
        delete[] mVertexes;
    }

    if(nullptr != mVertexMoveMatrix)
    {
        delete[] mVertexMoveMatrix;
    }
}

void VertexBuffer::SetSize(int vertexCount)
{
    mVertexCount = vertexCount;

    mVertexes = new VertexData[mVertexCount];
    memset(mVertexes, 0, sizeof(VertexData) * mVertexCount);

    mBoneInfo = new BoneInfo[mVertexCount];

    mVertexMoveMatrix = new glm::mat4[mVertexCount];

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
    for(int i=0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
    {
#if 0
        print_mat(mBoneInfo[vertexIndex].m_bones[i]->mWorldMatrix);
        memcpy(&(mVertexes->BoneWorldMatrix[i]), glm::value_ptr(mBoneInfo[vertexIndex].m_bones[i]->mWorldMatrix), sizeof(float) * 16);
        print_array(mVertexes->BoneWorldMatrix[i]);

#else
        mVertexes->BoneWorldMatrix[i][0]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][1]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][2]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][3]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][4]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][5]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][6]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][7]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][8]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][9]    = 0.4f;
        mVertexes->BoneWorldMatrix[i][10]   = 0.4f;
        mVertexes->BoneWorldMatrix[i][11]   = 0.4f;
        mVertexes->BoneWorldMatrix[i][12]   = 0.4f;
        mVertexes->BoneWorldMatrix[i][13]   = 0.4f;
        mVertexes->BoneWorldMatrix[i][14]   = 0.4f;
        mVertexes->BoneWorldMatrix[i][15]   = 0.4f;
#endif

        LOGE("-----------------------------\n");
        print_mat(mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);
        memcpy(&(mVertexes->BoneOffsetMatrix[i]), glm::value_ptr(mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix), sizeof(float) * 16);
        print_array(mVertexes->BoneOffsetMatrix[i]);
    }
#if 0
    //do the vertex blending,get the vertex's pos in world space
    glm::vec4 vAfterVc = glm::vec4();
    for(int i=0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
    {
        //
        glm::mat4 mat =  glm::mul(mBoneInfo[vertexIndex].m_bones[i]->mWorldMatrix, mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);
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
