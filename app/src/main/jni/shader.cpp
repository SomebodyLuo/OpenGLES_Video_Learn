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

    //----------------------------------------------------------------------
    mBoneNumLocation = -1;
    mBoneIdArrayLocation = -1;
    mBoneWeightArrayLocation = -1;

    mBoneWorldMatrixLocation0 = -1;
    mBoneWorldMatrixLocation1 = -1;
    mBoneWorldMatrixLocation2 = -1;
    mBoneWorldMatrixLocation3 = -1;

    mBoneOffsetMatrixLocation0 = -1;
    mBoneOffsetMatrixLocation1 = -1;
    mBoneOffsetMatrixLocation2 = -1;
    mBoneOffsetMatrixLocation3 = -1;

    //----------------------------------------------------------------------

    mModelMatrixLocation = -1;
    mViewMatrixLocation = -1;
    mProjectionMatrixLocation = -1;

//    mVertexMoveMatrixLocation = -1;
}

bool Shader::Init(AAssetManager *assetManager, const char *vs, const char *fs)
{
    if ((nullptr == assetManager) || (nullptr == vs) || (nullptr == fs))
    {
        return false;
    }
    LOGI("Shader::Init vs=%s; fs=%s\n", vs, fs);
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
        LOGE("------ vs CompileShader failed! -------");
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
        LOGE("------ fs CompileShader failed! -------");
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

    //----------------------------------------------------------------------
    mBoneNumLocation = glGetAttribLocation(mProgram, "boneNum");
    mBoneIdArrayLocation = glGetAttribLocation(mProgram, "boneIdArray");
    mBoneWeightArrayLocation = glGetAttribLocation(mProgram, "boneWeightArray");

    mBoneWorldMatrixLocation0 = glGetAttribLocation(mProgram, "boneWorldMatrix0");
    mBoneWorldMatrixLocation1 = glGetAttribLocation(mProgram, "boneWorldMatrix1");
    mBoneWorldMatrixLocation2 = glGetAttribLocation(mProgram, "boneWorldMatrix2");
    mBoneWorldMatrixLocation3 = glGetAttribLocation(mProgram, "boneWorldMatrix3");

    mBoneOffsetMatrixLocation0 = glGetAttribLocation(mProgram, "boneOffsetMatrix0");
    mBoneOffsetMatrixLocation1 = glGetAttribLocation(mProgram, "boneOffsetMatrix1");
    mBoneOffsetMatrixLocation2 = glGetAttribLocation(mProgram, "boneOffsetMatrix2");
    mBoneOffsetMatrixLocation3 = glGetAttribLocation(mProgram, "boneOffsetMatrix3");

    //----------------------------------------------------------------------

    mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
    mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
    mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");

//    mVertexMoveMatrixLocation = glGetUniformLocation(mProgram, "VertexMoveMatrix");

    return true;
}

//void Shader::Bind(float *M, float *V, float *P, float *VM)
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

//    glUniformMatrix4fv(mVertexMoveMatrixLocation, 1, GL_FALSE, VM);

//    LOGI("mUniformTextures.size = %d", mUniformTextures.size());
    // 启用多重纹理
    int index = 0;
    for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter)
    {
        // 每次绑定一个纹理，都要先激活一个插槽里面的纹理单元与之对应
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
        glUniform1i(iter->second->mLocation, index);
//        LOGI("Bind: t->mLocation = %d; t->mTexture = %u", iter->second->mLocation, iter->second->mTexture);
        index++;
    }

    for (auto iter = mUniformVec4s.begin(); iter != mUniformVec4s.end(); ++iter)
    {
        glUniform4fv(iter->second->mLocation, 1, iter->second->v);
    }

    // 可能我们的shader代码中，不一定4个Attribute都有，但是OpenGL能够容错。
    // 不存在的Attribute，相应的Location就是-1。
    glEnableVertexAttribArray(mPositionLocation);
    glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);

    glEnableVertexAttribArray(mColorLocation);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 4));

    glEnableVertexAttribArray(mTexcoordLocation);
    glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 8));

    glEnableVertexAttribArray(mNormalLocation);
    glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 12));

    //----------------------------------------------------------------------
    glEnableVertexAttribArray(mBoneNumLocation);
    glVertexAttribPointer(mBoneNumLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 16));

    glEnableVertexAttribArray(mBoneIdArrayLocation);
    glVertexAttribPointer(mBoneIdArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 20));

    glEnableVertexAttribArray(mBoneWeightArrayLocation);
    glVertexAttribPointer(mBoneWeightArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 24));

    glEnableVertexAttribArray(mBoneWorldMatrixLocation0);
    glVertexAttribPointer(mBoneWorldMatrixLocation0, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 28));
    glEnableVertexAttribArray(mBoneWorldMatrixLocation1);
    glVertexAttribPointer(mBoneWorldMatrixLocation1, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 44));
    glEnableVertexAttribArray(mBoneWorldMatrixLocation2);
    glVertexAttribPointer(mBoneWorldMatrixLocation2, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 60));
    glEnableVertexAttribArray(mBoneWorldMatrixLocation3);
    glVertexAttribPointer(mBoneWorldMatrixLocation3, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 76));

    glEnableVertexAttribArray(mBoneOffsetMatrixLocation0);
    glVertexAttribPointer(mBoneOffsetMatrixLocation0, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 92));
    glEnableVertexAttribArray(mBoneOffsetMatrixLocation1);
    glVertexAttribPointer(mBoneOffsetMatrixLocation1, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 108));
    glEnableVertexAttribArray(mBoneOffsetMatrixLocation2);
    glVertexAttribPointer(mBoneOffsetMatrixLocation2, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 124));
    glEnableVertexAttribArray(mBoneOffsetMatrixLocation3);
    glVertexAttribPointer(mBoneOffsetMatrixLocation3, 16, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 140));

    //----------------------------------------------------------------------
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

void Shader::SetTexture(const char *name, GLuint texture)
{
    auto iter = mUniformTextures.find(name);

    if(mUniformTextures.end() == iter)
    {
        GLint location = glGetUniformLocation(mProgram, name);
        if (-1 != location)
        {
            UniformTexture *t = new UniformTexture;
            t->mLocation = location;
            t->mTexture = texture;
//            LOGI("SetTexture: name = %s, t->mLocation = %d; t->mTexture = %u", name, t->mLocation, t->mTexture);
            mUniformTextures.insert(std::pair<std::string, UniformTexture *>(name, t));
        }
    } else {
        // 如果纹理列表中，存在目标纹理，则先删除，然后再重新生成
        glDeleteTextures(1, &iter->second->mTexture);
        iter->second->mTexture = texture;
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


