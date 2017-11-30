//
// Created by pc on 2017/11/30.
//

#include "ground.h"

void Ground::Init()
{
    mVertexBuffer = new VertexBuffer;
    mVertexBuffer->SetSize(1600);

    for (int z = 0; z < 20; ++z) {
        float zStart = 100.f - z * 10.0f;
        for (int x = 0; x < 20; ++x) {
            int offset = (x + z * 20) * 4;
            float xStart = z * 10.0f - 100.0f;

            // 棋盘格
            mVertexBuffer->SetPosition(offset + 0, xStart, -1.0f, zStart);
            mVertexBuffer->SetPosition(offset + 1, xStart + 10.0f, -1.0f, zStart);
            mVertexBuffer->SetPosition(offset + 2, xStart, -1.0f, zStart - 10.0f);
            mVertexBuffer->SetPosition(offset + 3, xStart + 10.0f, -1.0f, zStart - 10.0f);

            // 法向全部朝上 y=1.0f
            mVertexBuffer->SetNormal(offset + 0, 0.0f, 1.0f, 0.0f);
            mVertexBuffer->SetNormal(offset + 1, 0.0f, 1.0f, 0.0f);
            mVertexBuffer->SetNormal(offset + 2, 0.0f, 1.0f, 0.0f);
            mVertexBuffer->SetNormal(offset + 3, 0.0f, 1.0f, 0.0f);

            // 棋盘格的颜色
            if((z % 2) ^ (x % 2))
            {
                mVertexBuffer->SetColor(offset + 0, 0.1f, 0.1f, 0.1f);
                mVertexBuffer->SetColor(offset + 1, 0.1f, 0.1f, 0.1f);
                mVertexBuffer->SetColor(offset + 2, 0.1f, 0.1f, 0.1f);
                mVertexBuffer->SetColor(offset + 3, 0.1f, 0.1f, 0.1f);
            } else{
                mVertexBuffer->SetColor(offset + 0, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 1, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 2, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 3, 0.8f, 0.8f, 0.8f);
            }
        }

    }

    mVBO = CreateBufferObject(GL_ARRAY_BUFFER, mVertexBuffer->GetByteSize(), GL_STATIC_DRAW, mVertexBuffer->mVertexes);
}
