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
        LOGD("nullptr == assetManager");
        return nullptr;
    }

    // 打开
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (nullptr == asset)
    {
        LOGD("AAssetManager_open failed!");
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

        // shader既然编译失败了就释放吧！
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

        // program有问题，删除
        glDeleteProgram(program);

        program = 0;
    }

    return program;
}

float GetFrameTime()
{
    static unsigned long long lastTime = 0, currentTime = 0;

    timeval current;
    gettimeofday(&current, nullptr);
    currentTime = current.tv_sec * 1000 + current.tv_usec / 1000;

    unsigned long long frameTime = (lastTime == 0) ? 0 : currentTime - lastTime;
    lastTime = currentTime;

    return float(frameTime) / 1000.0f;

}

unsigned char*DecodeBMP(unsigned char *bmpFileData, int &width, int &height)
{
    if (0x4D42 == *((unsigned short*)bmpFileData))
    {
        int pixelDataOffset = *((int *)(bmpFileData + 10));
        width = *((int *)(bmpFileData + 18));
        height = *((int *)(bmpFileData + 22));
        unsigned char* pixelData = bmpFileData + pixelDataOffset;

        int size = width * height * 3;
        // 位图BMP文件的像素排列是BGR, OpenGL可识别的像素排列是RGB, 所以需要转换：
        for (int i = 0; i < size; i += 3) {
            unsigned char temp = pixelData[i];
            pixelData[i] = pixelData[i + 2];
            pixelData[i + 2] = temp;
        }
        return pixelData;
    }

    return nullptr;
}

GLuint CreateTexture2D(unsigned char *pixelData, int width, int height, GLenum type)
{
    GLuint texture = 0;

    // 申请一个纹理
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //纹理放大时，用什么算法采集像素
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //纹理缩小时，用什么算法采集像素
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //param1: 纹理编号
    //param2: MipMap级别，OpenGL会选择不同的级别去着色(占内存)。比如原始图像是128x128，缩小一倍就是64x64；原始级别就是0，下一级别是1，
    //param3: internal format, 像素在显卡中的格式
    //param4: 宽
    //param5: 高
    //param6: border, 必须写0
    //param7: 纹理数据在内存中是什么格式
    //param8: 像素数据每个分量是什么类型
    //param9: 像素数据地址
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint CreateTexture2DFromBMP(AAssetManager *assetManager, const char*bmpPath)
{
    int nFileSize = 0;

    unsigned char *bmpFileContent = (unsigned char *)LoadFileContent(assetManager, bmpPath, nFileSize);
    if (nullptr == bmpFileContent)
    {
        return 0;
    }

    int bmpWidth = 0, bmpHeight = 0;
    unsigned char *pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
    if (nullptr == pixelData)
    {
        delete bmpFileContent;
        return 0;
    }

    GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
    delete bmpFileContent;

    return texture;
}



