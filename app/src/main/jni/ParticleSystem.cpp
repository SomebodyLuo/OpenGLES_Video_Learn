//
// Created by pc on 2017/12/5.
//

#include "ParticleSystem.h"
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
        mVertexBuffer->SetPosition(i, 5.0f * cosf(float(i) * 2.0f * 3.14f / 180.0f), 0.0f, 5.0f * sinf(float(i) * 2.0f * 3.14f / 180.0f));
        mVertexBuffer->SetColor(i, 0.1f, 0.4f, 0.6f);
    }

    // Shader
    mShader = new Shader;
    mShader->Init(assetManager, "Res/particle.vs", "Res/particle.fs");
    mShader->SetTexture("U_Texture", CreateProcedureTexture(128));
}

void ParticleSystem::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    glDisable(GL_DEPTH_TEST);   // 一般粒子系统需要关闭深度测试
    glEnable(GL_BLEND);         // 开启混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // 混合算法

    mVertexBuffer->Bind();

    mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_POINTS, 0, mVertexBuffer->mVertexCount);

    mVertexBuffer->Unbind();

    glDisable(GL_BLEND);        // 关闭混合
}
