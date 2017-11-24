//
// Created by pc on 2017/11/24.
//

#include "scene.h"
#include "ggl.h"

//------------------- GPU(显卡) ------------------
// 理解显卡的原理
// 实现数据从CPU端传到GPU端的方法
// 顶点数据只在CPU，处理效率很低。数据量大，就很难搞
// 由于数据已经到了显卡上，无法使用C语言来操作数据，因此就要依赖VBO--Vertex Buffer Object。

GLuint vbo;

void InitGL()
{
    float data[] = {
            -0.2f, -0.2f, -0.6f, 1.0f,
            0.2f, -0.2f, -0.6f, 1.0f,
            0.0f, 0.2f, -0.6f, 1.0f,
    };

    //向显卡申请一块VBO内存
    glGenBuffers(1, &vbo);

    // 把VBO内存设置成当前（OpenGL状态机）的VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 将数据传入GPU内存，然后CPU内的数据就可以删除了
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, data, GL_STATIC_DRAW);

    // 解除当前的VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SetViewportSize(float width, float height)
{

}

void DrawGL()
{

}


