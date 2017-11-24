//
// Created by pc on 2017/11/24.
//

#include "utils.h"

// JNI-C++访问Android APP资源的方法
char *LoadFileContent(AAssetManager *assetManager, const char *path, int &filesize)
{
    char *fileContent = nullptr;
    filesize = 0;

    if (nullptr == assetManager)
    {
        return nullptr;
    }

    // 打开
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (nullptr == asset)
    {
        return nullptr;
    }

    filesize = AAsset_getLength(asset);
    fileContent = new char[filesize + 1];

    // 读文件
    AAsset_read(asset, fileContent, filesize);
    fileContent[filesize] = '\0';

    // 关闭文件
    AAsset_close(asset);

    return fileContent;
}

GLuint CompileShader(GLenum shaderType, const char *shaderCode)
{
    // 根据shader类型创建一个Shader对象（标识符）
    GLuint shader = glCreateShader(shaderType);

    // 加载Shader源码内容
    glShaderSource(shader, 1, &shaderCode, nullptr);

    // 编译Shader源码
    glCompileShader(shader);

    // 获取编译结果
    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (GL_FALSE == compileResult)
    {
        char szLog[1024] = {0};
        GLsizei logLen = 0;
        // 获取编译结果Log日志
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        printf("Compile Shader fail error log : %s \nshader code :\n%s\n", szLog, shaderCode);

        // 既然编译失败了就释放吧！
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

// vs--Vertex Shader; fs--Fragment Shader
// 注意这个程序是在GPU上面跑，而不是在CPU上面跑！！！！
GLuint CreateProgram(GLuint vsShader, GLuint fsShader)
{
    // 创建一个程序
    GLuint program = glCreateProgram();

    // 将vs/fs Shader绑定到程序上
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);

    // Link程序
    glLinkProgram(program);

    // 解绑定
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);

    // 获取Link结果
    GLint nResult;
    glGetProgramiv(program, GL_LINK_STATUS, &nResult);
    if (GL_FALSE == nResult)
    {
        char log[1024] = {0};
        GLsizei writed = 0;
        // 获取Link结果Log日志
        glGetProgramInfoLog(program, 1024, &writed, log);

        printf("create GPU program failed, link error : %s \n", log);

        // 有问题，删除
        glDeleteProgram(program);

        program = 0;
    }

    return program;
}



