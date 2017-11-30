//
// Created by pc on 2017/11/30.
//

#include "ground.h"

void Ground::Init()
{
    Vertex vertex[1600];

    for (int i = 0; i < 20; ++i) {
        float zStart = 100.f - i * 10.0f;
        for (int j = 0; j < 20; ++j) {
            int offset = (j + i * 20) * 4;
            float xStart = i * 10.0f - 100.0f;

            vertex[offset + 0].Position[0] = xStart;
            vertex[offset + 0].Position[1] = -1.0f;
            vertex[offset + 0].Position[2] = zStart;
            vertex[offset + 0].Position[3] = 1.0f;

            vertex[offset + 1].Position[0] = xStart + 10.0f;
            vertex[offset + 1].Position[1] = -1.0f;
            vertex[offset + 1].Position[2] = zStart;
            vertex[offset + 1].Position[3] = 1.0f;

            vertex[offset + 2].Position[0] = xStart;
            vertex[offset + 2].Position[1] = -1.0f;
            vertex[offset + 2].Position[2] = zStart - 10.0f;
            vertex[offset + 2].Position[3] = 1.0f;

            vertex[offset + 3].Position[0] = xStart + 10.0f;
            vertex[offset + 3].Position[1] = -1.0f;
            vertex[offset + 3].Position[2] = zStart - 10.0f;
            vertex[offset + 3].Position[3] = 1.0f;
        }

    }

    mVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * 1600, GL_STATIC_DRAW, vertex);
}
