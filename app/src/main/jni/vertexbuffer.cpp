//
// Created by pc on 2017/11/30.
//

#include "vertexbuffer.h"
#include "utils.h"

VertexBuffer::VertexBuffer()
{
    mVBO = 0;
    mVertexes = nullptr;
    mVertexCount = 0;
}

void VertexBuffer::SetSize(int vertexCount)
{
    mVertexCount = vertexCount;
    mVertexes = new Vertex[mVertexCount];
    memset(mVertexes, 0, sizeof(Vertex) * mVertexCount);

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
    return (sizeof(Vertex) * mVertexCount);
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

Vertex & VertexBuffer::GetVertex(int index)
{
    return mVertexes[index];
}


