//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_VERTEXBUFFER_H
#define GLDEMO_VERTEXBUFFER_H

#include "ggl.h"

struct Vertex
{
    float Position[4];
    float Color[4];
    float Texcoord[4];
    float Normal[4];
};

class VertexBuffer
{
public:
    Vertex *mVertexes;
    int mVertexCount;

    GLuint mVBO;

    VertexBuffer();

    void SetSize(int vertexCount);
    void SetPosition(int index, float x, float y, float z, float w = 1.0f);
    void SetColor(int index, float r, float g, float b, float a = 1.0f);
    void SetTexcoord(int index, float x, float y);
    void SetNormal(int index, float x, float y, float z);

    int GetByteSize();

    void Bind();
    void Unbind();
    Vertex &GetVertex(int index);

};

#endif //GLDEMO_VERTEXBUFFER_H
