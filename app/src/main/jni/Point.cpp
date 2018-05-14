//
// Created by pc on 2018/5/15.
//

#include "Point.h"

Point::Point()
{
    m_vertexNum = 0;
    mShader = nullptr;
}

Point::~Point(void)
{
    delete[] mVertexBuffer;
    delete mShader;
}

void Point::Init(int pointCounts, AAssetManager *assetManager, const char *modelPath)
{
    if ((nullptr == assetManager) || (nullptr == modelPath))
    {
        return;
    }

    m_vertexNum = pointCounts;

    mVertexBuffer = new VertexBuffer();
    mVertexBuffer->SetSize(m_vertexNum);
    for (int i = 0; i < m_vertexNum; ++i) {
        mVertexBuffer->SetPosition(i, 0.1f, 0.1f, 0.1f);
        mVertexBuffer->SetColor(i, 0.9f, 0.6f, 0.1f);
    }

    LOGI("mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    //=======================2===================
    // 加载Shader
    mShader = new Shader();
    mShader->Init(assetManager, "Res/point.vs", "Res/point.fs");

    // 光照
    mShader->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);

}

void Point::SetPointPosition(int index, float x, float y, float z)
{
    if(index >= m_vertexNum)
        return;

    mVertexBuffer->SetPosition(index, x, y, z);
}


void Point::DrawStaticMesh(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{

    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

    mShader->SetVec4("U_PointColor", 0.2f, 0.9f, 0.2f, 1.0f);
//    mShader->SetVec4("U_Bool", 1.0f, 0.0f, 0.0f, 0.0f);

    glm::mat4 identityMat = glm::mat4();//glm::translate(-0.1f, -0.1f, 0.0f);
    mShader->BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix),
                     glm::value_ptr(projectionMatrix));

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
        glDrawArrays(GL_POINTS, i, 1);
    }

    // draw lines
//    glLineWidth(2.0f);
//    for (int i = 0; i < mVertexBuffer->mVertexCount - 1; ++i) {
//        glDrawArrays(GL_LINES, i, 2);
//    }

    mVertexBuffer->Unbind();
}
