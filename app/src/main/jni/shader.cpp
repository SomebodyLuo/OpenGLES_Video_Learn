//
// Created by pc on 2017/11/30.
//

#include "shader.h"
#include "utils.h"
#include "vertexbuffer.h"

Shader::Shader()
{
    mAssetManager = nullptr;
    mProgram = 0;

    mPositionLocation = -1;
    mColorLocation = -1;
    mNormalLocation = -1;
    mTexcoordLocation = -1;
    mModelMatrixLocation = -1;
    mViewMatrixLocation = -1;
    mProjectionMatrixLocation = -1;
}

bool Shader::Init(AAssetManager *assetManager, const char *vs, const char *fs)
{
    if ((nullptr == assetManager) || (nullptr == vs) || (nullptr == fs))
    {
        return false;
    }
    mAssetManager = assetManager;

    int fileSize = 0;
    char *vsCode = LoadFileContent(mAssetManager, vs, fileSize);
    if (nullptr == vsCode)
    {
        return false;
    }

    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
    delete vsCode;
    if (0 == vsShader)
    {
        return false;
    }

    char *fsCode = LoadFileContent(mAssetManager, fs, fileSize);
    if (nullptr == fsCode)
    {
        glDeleteShader(vsShader);
        return false;
    }

    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
    delete fsCode;
    if (0 == fsShader)
    {
        glDeleteShader(vsShader);
        return 0;
    }

    mProgram = CreateProgram(vsShader, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    if (0 == mProgram)
    {
        return false;
    }

    // 可能我们的shader代码中，不一定4个Attribute都有，但是OpenGL能够容错。
    // 不存在的Attribute，相应的Location就是-1。
    mPositionLocation = glGetAttribLocation(mProgram, "position");
    mColorLocation = glGetAttribLocation(mProgram, "color");
    mTexcoordLocation = glGetAttribLocation(mProgram, "texcoord");
    mNormalLocation = glGetAttribLocation(mProgram, "normal");

    mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
    mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
    mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");

    return true;
}

void Shader::Bind(float *M, float *V, float *P)
{
    if (0 == mProgram)
    {
        return ;
    }

    glUseProgram(mProgram);

    glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, M);
    glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, V);
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, P);

    LOGI("mUniformTextures.size = %d", mUniformTextures.size());
    // 启用多重纹理
    int index = 0;
    for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter)
    {
        // 每次绑定一个纹理，都要先激活一个插槽里面的纹理单元与之对应
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
        glUniform1i(iter->second->mLocation, index);

        index++;
    }

    for (auto iter = mUniformVec4s.begin(); iter != mUniformVec4s.end(); ++iter)
    {
        glUniform4fv(iter->second->mLocation, 1, iter->second->v);
    }

    // 可能我们的shader代码中，不一定4个Attribute都有，但是OpenGL能够容错。
    // 不存在的Attribute，相应的Location就是-1。
    glEnableVertexAttribArray(mPositionLocation);
    glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(mColorLocation);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 4));

    glEnableVertexAttribArray(mTexcoordLocation);
    glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 8));

    glEnableVertexAttribArray(mNormalLocation);
    glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 12));

}

void Shader::SetTexture(const char *name, const char *imagePath)
{
    if ((nullptr == mAssetManager) || (nullptr == name) || (nullptr == imagePath))
    {
        return ;
    }

    // 设置纹理(支持多重)
    auto iter = mUniformTextures.find(name);
    if(iter == mUniformTextures.end())
    {
        // 如果纹理列表中不存在目标纹理，则创建
        GLint location = glGetUniformLocation(mProgram, name);
        if (-1 != location)
        {
            UniformTexture *t = new UniformTexture;
            t->mLocation = location;
            t->mTexture = CreateTexture2DFromBMP(mAssetManager, imagePath);
            mUniformTextures.insert(std::pair<std::string, UniformTexture *>(name, t));
        }

    } else {
        // 如果纹理列表中，存在目标纹理，则先删除，然后再重新生成
        glDeleteTextures(1, &iter->second->mTexture);
        iter->second->mTexture = CreateTexture2DFromBMP(mAssetManager, imagePath);
    }
}

void Shader::SetVec4(const char *name, float x, float y, float z, float w)
{
    auto iter = mUniformVec4s.find(name);

    if (mUniformVec4s.end() == iter)
    {
        GLint location = glGetUniformLocation(mProgram, name);
        if (-1 != location)
        {
            UniformVector4f *v = new UniformVector4f;
            v->v[0] = x;
            v->v[1] = y;
            v->v[2] = z;
            v->v[3] = w;
            v->mLocation = location;
            mUniformVec4s.insert(std::pair<std::string, UniformVector4f *>(name, v));
        }
    } else {
        iter->second->v[0] = x;
        iter->second->v[1] = y;
        iter->second->v[2] = z;
        iter->second->v[3] = w;
    }
}


