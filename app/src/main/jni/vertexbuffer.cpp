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
    mModelMatrix = nullptr;
}

VertexBuffer::~VertexBuffer()
{
    if(nullptr != mVertexes)
    {
        delete[] mVertexes;
    }

    if(nullptr != mModelMatrix)
    {
        delete[] mModelMatrix;
    }
}

void VertexBuffer::SetSize(int vertexCount)
{
    mVertexCount = vertexCount;

    mVertexes = new VertexData[mVertexCount];
    memset(mVertexes, 0, sizeof(VertexData) * mVertexCount);

    mBoneInfo = new BoneInfo[mVertexCount];

    mModelMatrix = new glm::mat4[mVertexCount];

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
void VertexBuffer::BlendVertex(int vertexIndex)
{
    //do the vertex blending,get the vertex's pos in world space
    mModelMatrix[vertexIndex] = glm::mat4();

    for(int i = 0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
    {
        glm::mat4 mat;

        mat = glm::mul(mBoneInfo[vertexIndex].m_bones[i]->mModelMatrix, mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);

        mat = mat * mBoneInfo[vertexIndex].m_boneWeights[i];

        mModelMatrix[vertexIndex] = glm::mul(mat, mModelMatrix[vertexIndex]);
    }
}

#if 0
void Vertex::ComputeWorldPosByBone(Bone* pBone, float& outX, float& outY, float& outZ)
{

    //step1: transform vertex from mesh space to bone space

    outX = pBone->m_boneOffset.m_offx+m_x;

    outY =  pBone->m_boneOffset.m_offy+m_y;

    outZ = pBone->m_boneOffset.m_offz+m_z;



    //step2: transform vertex from bone space to world sapce

    outX += pBone->m_wx;

    outY += pBone->m_wy;

    outZ += pBone->m_wz;

}


void Vertex::BlendVertex()

{
    //do the vertex blending,get the vertex's pos in world space
    m_wX = 0;
    m_wY = 0;
    m_wZ = 0;

    for(int i=0; i<m_boneNum; ++i)
    {

        float tx, ty, tz;

        ComputeWorldPosByBone(m_bones[i], tx, ty, tz);

        tx*= m_boneWeights[i];

        ty*= m_boneWeights[i];

        tz*= m_boneWeights[i];



        m_wX += tx;

        m_wY += ty;

        m_wZ += tz;

    }

}
#endif