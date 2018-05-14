//
// Created by pc on 2017/11/30.
//

#include "ground.h"

Ground::Ground()
{
    mVertexBuffer = nullptr;
    mShader = nullptr;
}

void Ground::Init(AAssetManager *assetManager)
{
    if(nullptr == assetManager)
    {
        return;
    }

    mVertexBuffer = new VertexBuffer;
    mVertexBuffer->SetSize(1600);

    for (int z = 0; z < 20; ++z) {
        float zStart = 100.0f - z * 10.0f;
        for (int x = 0; x < 20; ++x) {
            int offset = (x + z * 20) * 4;
            float xStart = x * 10.0f - 100.0f;

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
            } else {
                mVertexBuffer->SetColor(offset + 0, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 1, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 2, 0.8f, 0.8f, 0.8f);
                mVertexBuffer->SetColor(offset + 3, 0.8f, 0.8f, 0.8f);
            }
        }
    }

    mShader = new Shader;
    mShader->Init(assetManager, "Res/ground.vs", "Res/ground.fs");

    // 光照
    mShader->SetVec4("U_LightPos", 0.0f, 20.0f, 0.0f, 0.0f);
    mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    mShader->SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
    mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    mShader->SetVec4("U_DiffuseMaterial", 0.6f, 0.6f, 0.6f, 1.0f);
}

void Ground::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    glEnable(GL_DEPTH_TEST);

    // 注意代码顺序, VBO的Bind必须在Shader的Bind之前，否则Shader无数据可用！
    mVertexBuffer->Bind();

    mShader->BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix),
                     glm::value_ptr(projectionMatrix));

    // 绘制400个小方块
    for (int i = 0; i < 400; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);  //注意GL_TRIANGLE_STRIP
    }

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
//    glDrawArrays(GL_POINTS, 0, mVertexBuffer->mVertexCount);

    // draw lines
//    glLineWidth(2.0f);
//    for (int i = 0; i < 800; ++i) {
//        glDrawArrays(GL_LINES, i * 2, 2);  //注意GL_TRIANGLE_STRIP
//    }

    mVertexBuffer->Unbind();

//    glDisable(GL_DEPTH_TEST);

}



void Ground::SetPosition(float x, float y, float z)
{
    mModelMatrix = glm::translate(x, y, z);
}
