//
// Created by pc on 2017/11/24.
//

#include "scene.h"
#include "ggl.h"
#include "utils.h"

//------------------- GPU(显卡) ------------------
// 理解显卡的原理
// 实现数据从CPU端传到GPU端的方法
// 顶点数据只在CPU，处理效率很低。数据量大，就很难搞
// 由于数据已经到了显卡上，无法使用C语言来操作数据，因此就要依赖VBO--Vertex Buffer Object。

GLuint vbo = 0;     //vertex buffer object, 顶点数据缓存！！！
GLuint program = 0;

//指导组织VBO的数据去绘制图元
GLuint ebo = 0;     //element buffer object, 顶点数据索引缓存！！！


// 申明插槽C++变量
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation, colorLocation;

GLint texcoordLocation, textureLocation;

GLuint texture;

// 矩阵默认是单位矩阵
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

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
    //------------------------- VBO ----------------------------
    vbo = 0;
    //向显卡申请一块VBO内存
    glGenBuffers(1, &vbo);
    LOGI("vbo = %u", vbo);      //1

    // 把VBO内存设置成当前（OpenGL状态机）的VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 将数据传入GPU内存，然后CPU内的数据就可以删除了
    glBufferData(GL_ARRAY_BUFFER, sizeof(trianglecoordinates), trianglecoordinates, GL_STATIC_DRAW);

    // 解除当前的VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //------------------------- EBO ----------------------------
    ebo = 0;
    unsigned short indices[] = {0, 1, 2};
    glGenBuffers(1, &ebo);
    LOGI("ebo = %u", ebo);      //2
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // 将索引缓存数据传入GPU内存，然后CPU内的数据就可以删除了
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //------------------------- shader program -------------------------
    int filesize = 0;
    // vsShader
    char *shaderCode = LoadFileContent(assetManager, "Res/test.vs", filesize);
    if (nullptr == shaderCode)
    {
        LOGD("1 nullptr == vs shaderCode");
        return;
    }
    GLuint  vsShader = CompileShader(GL_VERTEX_SHADER, shaderCode);
    delete shaderCode;
    if (0 == vsShader)
    {
        LOGD("0 == vsShader");
        return;
    }

    // fsShader
    shaderCode = LoadFileContent(assetManager, "Res/test.fs", filesize);
    if (nullptr == shaderCode)
    {
        LOGD("2 nullptr == fs shaderCode");
        glDeleteShader(vsShader);
        return;
    }
    GLuint  fsShader = CompileShader(GL_FRAGMENT_SHADER, shaderCode);
    delete shaderCode;
    if (0 == fsShader)
    {
        LOGD("0 == fsShader");
        glDeleteShader(vsShader);
        return;
    }

    // 把vsShader和fsShader合成program
    // 类似于C++中所有obj文件链接到一个exe执行文件里面
    program = 0;
    program = CreateProgram(vsShader, fsShader);
    if (0 == program)
    {
        LOGD("0 == program");
        glDeleteShader(vsShader);
        glDeleteShader(fsShader);
        return;
    }

    // vsShader / fsShader 相当于C++编译时的obj文件
    // 当编译结束后，obj文件就没有用了。
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    //-------------------------------------------------------------------

    // 如何把C++中的顶点数据和MVP矩阵，设置到Shader里面去？
    // shader中有槽的概念，shader源码中的变量就对应到每一个插槽
    // 那么我们只要告诉GPU，哪个插槽应该放哪个数据即可。
    // 获取到shader变量对应的插槽：
    positionLocation = glGetAttribLocation(program, "position");
    colorLocation = glGetAttribLocation(program, "color");
    modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
    viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
    projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
    LOGI("------program: positionLocation = %d; modelMatrixLocation = %d; viewMatrixLocation = %d; projectionMatrixLocation = %d", positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation);

    // 加载纹理，准备工作
    texcoordLocation = glGetAttribLocation(program, "texcoord");
    textureLocation = glGetUniformLocation(program, "U_Texture");
    texture = CreateTexture2DFromBMP(assetManager, "Res/test.bmp");
    LOGI("------program: texcoordLocation = %d; textureLocation = %d; texture = %d;", texcoordLocation, textureLocation, texture);

    // 给model一个偏移矩阵，也就是 modelMatrix
//    modelMatrix = glm::translate(modelMatrix,  glm::vec3(0.0f, 0.0f, -0.6f));

    // 给camera一个偏移矩阵，也就是 viewMatrix
//    viewMatrix = glm::translate(viewMatrix,  glm::vec3(0.0f, 0.0f, -1.0f));
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

    if(0 != program) {

        LOGI("------ DrawGL shader program ------- ");
        // 选择一个程序来执行绘制指令
        glUseProgram(program);

        //将矩阵传入插槽
        //param1: 插槽位置（序号）
        //param2: 传入设置几个这样的矩阵
        //param3: 矩阵从CPU传入GPU是否需要转制
        //param4: 要传入的矩阵
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // 启用纹理
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureLocation, 0);

        // 设置当前的数据集，vbo实际是指代GPU的内存数据
        // 程序绘制的时候，shader绘制图形的时候从哪里获取顶点数据
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // 启用顶点插槽
        glEnableVertexAttribArray(positionLocation);

        //param1: 插槽位置（序号）
        //param2: 插槽中每个顶点数据有几个分量，我们这里的数据有x/y/z/w 4个分量
        //param3: 每个分量是什么类型
        //param4: 是否要进行归一化。如果传入的是RGB数据（0-255），就要映射到0.0-1.0
        //param5: 紧挨着的2个点，地址相距的间隔，也就是数据数组的行大小
        //param6: 顶点信息从VBO中哪里开始取值
        // 告诉GPU如何去遍历VBO内存块上面的数据
        glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void *)0);

        //param1: 绘制三角形
        //param2: 第几个顶点开始绘制
        //param3: 总共绘制几个点
//        glDrawArrays(GL_TRIANGLES, 0, vertexCounts);



        // 启用颜色插槽
        glEnableVertexAttribArray(colorLocation);
        glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void *)(sizeof(float) * 4));    //注意最后一个参数，代表颜色数据的起始位置偏移量

        // 启用纹理坐标插槽
        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void *)(sizeof(float) * 8));

        // 启用顶点索引
        //注意EBO代码的位置，必须在glDisableVertexAttribArray / glBindBuffer之前，也就是VBO使能期间
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // 解除顶点插槽
//        glDisableVertexAttribArray(positionLocation);

        // 解除VBO设置
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 解除shader程序
        glUseProgram(0);
    }
}


