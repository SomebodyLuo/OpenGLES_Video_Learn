//
// Created by pc on 2017/11/24.
//

#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "shader.h"

// 矩阵默认是单位矩阵
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

Ground ground;

Shader *shader;
VertexBuffer *vertexBuffer;


float trianglecoordinates[] = {
        /*顶点数据*/-0.2f, -0.2f, -0.6f, 1.0f, /*颜色数据*/1.0f, 1.0f, 1.0f, 1.0f, /*纹理坐标*/0.0f, 0.0f,
        /*顶点数据*/0.2f, -0.2f, -0.6f, 1.0f, /*颜色数据*/0.0f, 1.0f, 0.0f, 1.0f, /*纹理坐标*/1.0f, 0.0f,
        /*顶点数据*/0.0f, 0.2f, -0.6f, 1.0f, /*颜色数据*/1.0f, 0.0f, 0.0f, 1.0f, /*纹理坐标*/0.5f, 1.0f,
};

void InitGL(AAssetManager *assetManager)
{
    LOGI("------InitGL-------");
    if (nullptr == assetManager)
    {
        return;
    }

    vertexBuffer = new VertexBuffer;
    vertexBuffer->SetSize(3);

    vertexBuffer->SetPosition(0, -0.2f, -0.2f, 0.0f);
    vertexBuffer->SetColor(0, 1.0f, 1.0f, 1.0f);
    vertexBuffer->SetTexcoord(0, 0.0f, 0.0f);

    vertexBuffer->SetPosition(1, 0.2f, -0.2f, 0.0f);
    vertexBuffer->SetColor(1, 1.0f, 0.0f, 0.0f);
    vertexBuffer->SetTexcoord(1, 1.0f, 0.0f);

    vertexBuffer->SetPosition(2, 0.0f, 0.2f, 0.0f);
    vertexBuffer->SetColor(2, 0.0f, 1.0f, 1.0f);
    vertexBuffer->SetTexcoord(2, 0.5f, 1.0f);

    shader = new Shader;
    shader->Init(assetManager, "Res/test.vs", "Res/test.fs");
    shader->SetTexture("U_Texture", "Res/test.bmp");

    // 给model一个偏移矩阵，也就是 modelMatrix
    modelMatrix = glm::translate(modelMatrix,  glm::vec3(0.0f, 0.0f, -0.9f));

    // 给camera一个偏移矩阵，也就是 viewMatrix
//    viewMatrix = glm::translate(viewMatrix,  glm::vec3(0.0f, 0.0f, -1.0f));


    // 棋盘格
    ground.Init(assetManager);
}

void SetViewportSize(float width, float height)
{
    //------SetViewportSize-------width = 720.000000; height = 1132.000000
    LOGI("------SetViewportSize-------width = %f; height = %f", width, height);
    // 设置投影矩阵
    projectionMatrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
}

void DrawGL()
{
    float frameTime = GetFrameTime();

    LOGI("------DrawGL------- %f s", frameTime);
    // 擦除背景颜色
    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);

    //设置颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绘制棋盘格
    ground.Draw(viewMatrix, projectionMatrix);

    // 绘制三角形
    vertexBuffer->Bind();
    shader->Bind(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    vertexBuffer->Unbind();


}


