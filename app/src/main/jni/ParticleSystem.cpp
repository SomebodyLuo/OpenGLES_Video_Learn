//
// Created by pc on 2017/12/5.
//

#include "ParticleSystem.h"
#include "ggl.h"
#include "utils.h"

ParticleSystem::ParticleSystem() {

    mVertexBuffer = nullptr;
    mShader = nullptr;
}

void ParticleSystem::Init(AAssetManager *assetManager, float x, float y, float z)
{
    // 设置粒子的位置
    mModelMatrix = glm::translate(x, y, z);

    mVertexBuffer = new VertexBuffer;
    int particleCounts = 180;
    mVertexBuffer->SetSize(particleCounts);
    for (int i = 0; i < particleCounts; ++i) {
        mVertexBuffer->SetPosition(i, 8.0f * cosf(float(i) * 8.0f * 3.14f / 180.0f), 0.0f, 8.0f * sinf(float(i) * 8.0f * 3.14f / 180.0f));
        mVertexBuffer->SetColor(i, 0.1f, 0.4f, 0.6f);
    }

    // Shader
    mShader = new Shader;
    mShader->Init(assetManager, "Res/particle.vs", "Res/particle.fs");
    mTexture = CreateProcedureTexture(256);
    mShader->SetTexture("U_Texture", mTexture);
}

void ParticleSystem::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    glDisable(GL_DEPTH_TEST);   // 一般粒子系统需要关闭深度测试
    glEnable(GL_BLEND);         // 开启混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // 混合算法

    mVertexBuffer->Bind();

    mShader->BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix),
                     glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_POINTS, 0, mVertexBuffer->mVertexCount);

    mVertexBuffer->Unbind();

    glDisable(GL_BLEND);        // 关闭混合
}

void ParticleSystem::Update(float deltaTime) {

    static float angle = 0.0f;

    // 每次刷新时，旋转一定角度，相当于有了动画
    angle += deltaTime * 10.f;
    mModelMatrix = glm::rotate(angle, 0.0f, 1.0f, 0.0f);

    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
//        Normal[0] = x; Normal[1] = y; 让粒子上下运动
        mVertexBuffer->GetVertex(i).Normal[1] = 0.1f * i;
        if (i > 90)
        {
            break;
        }
    }
}
