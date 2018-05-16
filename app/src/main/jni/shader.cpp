//
// Created by pc on 2017/11/30.
//

#include "shader.h"
#include "utils.h"

Shader::Shader()
{
    mAssetManager = nullptr;
    mProgram = 0;

    mPositionLocation = -1;
    mColorLocation = -1;
    mNormalLocation = -1;
    mTexcoordLocation = -1;

    mBoneCountsLocation = -1;
    mBoneIdsArrayLocation = -1;
    mBoneWeightArrayLocation = -1;

    mInvModelMatrixLocation = -1;
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

    LOGI("Shader::Init mProgram = %d\n", mProgram);

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
    mBoneCountsLocation = glGetAttribLocation(mProgram, "boneCounts");
    LOGI("mBoneCountsLocation = %d\n", mBoneCountsLocation);
    mBoneIdsArrayLocation = glGetAttribLocation(mProgram, "boneIdsArray");
    LOGI("mBoneIdsArrayLocation = %d\n", mBoneIdsArrayLocation);
    mBoneWeightArrayLocation = glGetAttribLocation(mProgram, "boneWeightArray");
    LOGI("mBoneWeightArrayLocation = %d\n", mBoneWeightArrayLocation);

    mInvModelMatrixLocation = glGetUniformLocation(mProgram, "invModelMatrix");
    mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
    mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
    mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");


    mBoneIndexArrayLocation = glGetUniformLocation(mProgram, "boneIndexArray");
    mBoneWorldTranslateMatrixArrayLocation = glGetUniformLocation(mProgram, "boneWorldTranslateMatrixArray");
    mBoneWorldRotationMatrixArrayLocation = glGetUniformLocation(mProgram, "boneWorldRotationMatrixArray");
    mBoneWorldModelMatrixArrayLocation = glGetUniformLocation(mProgram, "boneWorldModelMatrixArray");
    mBoneOffsetMatrixArrayLocation = glGetUniformLocation(mProgram, "boneOffsetMatrixArray");
    LOGI("mBoneIndexArrayLocation = %d\n", mBoneIndexArrayLocation);
    LOGI("mBoneWorldTranslateMatrixArrayLocation = %d\n", mBoneWorldModelMatrixArrayLocation);
    LOGI("mBoneOffsetMatrixArrayLocation = %d\n", mBoneOffsetMatrixArrayLocation);


    return true;
}

void Shader::Bind(float *InvM, float *M, float *V, float *P, VertexBuffer *vb)
{
    if (0 == mProgram)
    {
        return ;
    }

    glUseProgram(mProgram);

    glUniformMatrix4fv(mInvModelMatrixLocation, 1, GL_FALSE, InvM);
    glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, M);
    glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, V);
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, P);

#if 0
    //----------------------------------------------
    // 每个顶点拥有的骨骼数量、骨骼ID、骨骼权重
    glUniform1iv(mBoneCountsLocation,
            // How many matrices to pass
                 vb->mBoneCountsArray.size(),
            // Pointer to the first element of the matrix
                 &(vb->mBoneCountsArray[0]));

    glUniform4iv(mBoneIdsArrayLocation,
            // How many matrices to pass
                 vb->mBoneIdsArray.size(),
            // Pointer to the first element of the matrix
                 &(vb->mBoneIdsArray[0][0]));

    glUniform4fv(mBoneWeightArrayLocation,
            // How many matrices to pass
                 vb->mBoneWeightArray.size(),
            // Pointer to the first element of the matrix
                 &(vb->mBoneWeightArray[0][0]));

    //----------------------------------------------
    // 所有骨骼的信息：ID、worldMatrix、OffsetMatrix
    LOGI("mBoneWeightArrayLocation = %d\n", mBoneWeightArrayLocation);
    LOGI("vb->mBoneWeightArray[0][0] = %f\n", vb->mBoneWeightArray[0][0]);
    LOGI("vb->mBoneWeightArray[1][0] = %f\n", vb->mBoneWeightArray[1][0]);
    LOGI("vb->mBoneWeightArray[2][0] = %f\n", vb->mBoneWeightArray[2][0]);
    LOGI("vb->mBoneWeightArray[3][0] = %f\n", vb->mBoneWeightArray[3][0]);
    LOGI("vb->mBoneWeightArray[4][0] = %f\n", vb->mBoneWeightArray[4][0]);
#endif

    glUniform1iv(mBoneIndexArrayLocation,
            // How many matrices to pass
                 vb->mBoneIndexArray.size(),
            // Pointer to the first element of the matrix
                 &(vb->mBoneIndexArray[0]));

    glUniformMatrix4fv(mBoneWorldTranslateMatrixArrayLocation,
            // How many matrices to pass
                       vb->mBoneWorldTranslateMatrixArray.size(),
            // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
            // Pointer to the first element of the matrix
                       &(vb->mBoneWorldTranslateMatrixArray[0][0][0]));

    glUniformMatrix4fv(mBoneWorldRotationMatrixArrayLocation,
            // How many matrices to pass
                       vb->mBoneWorldRotationMatrixArray.size(),
            // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
            // Pointer to the first element of the matrix
                       &(vb->mBoneWorldRotationMatrixArray[0][0][0]));

    glUniformMatrix4fv(mBoneWorldModelMatrixArrayLocation,
            // How many matrices to pass
                       vb->mBoneWorldModelMatrixArray.size(),
            // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
            // Pointer to the first element of the matrix
                       &(vb->mBoneWorldModelMatrixArray[0][0][0]));

    glUniformMatrix4fv(mBoneOffsetMatrixArrayLocation,
            // How many matrices to pass
                       vb->mBoneOffsetMatrixArray.size(),
            // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
            // Pointer to the first element of the matrix
                       &(vb->mBoneOffsetMatrixArray[0][0][0]));


//    LOGI("mUniformTextures.size = %d", mUniformTextures.size());
    // 启用多重纹理
    int index = 0;
    for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter)
    {
        // 每次绑定一个纹理，都要先激活一个插槽里面的纹理单元与之对应
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
        glUniform1i(iter->second->mLocation, index);
//        LOGI("BindMVP: t->mLocation = %d; t->mTexture = %u", iter->second->mLocation, iter->second->mTexture);
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

    glEnableVertexAttribArray(mBoneCountsLocation);
    glVertexAttribPointer(mBoneCountsLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 16));

    glEnableVertexAttribArray(mBoneIdsArrayLocation);
    glVertexAttribPointer(mBoneIdsArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 17));

    glEnableVertexAttribArray(mBoneWeightArrayLocation);
    glVertexAttribPointer(mBoneWeightArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 21));
}

void Shader::BindMVP(float *M, float *V, float *P)
{
    if (0 == mProgram)
    {
        return ;
    }

    glUseProgram(mProgram);

    glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, M);
    glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, V);
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, P);


//    LOGI("mUniformTextures.size = %d", mUniformTextures.size());
    // 启用多重纹理
    int index = 0;
    for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter)
    {
        // 每次绑定一个纹理，都要先激活一个插槽里面的纹理单元与之对应
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
        glUniform1i(iter->second->mLocation, index);
//        LOGI("BindMVP: t->mLocation = %d; t->mTexture = %u", iter->second->mLocation, iter->second->mTexture);
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

    glEnableVertexAttribArray(mBoneIdsArrayLocation);
    glVertexAttribPointer(mBoneIdsArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 16));

    glEnableVertexAttribArray(mBoneWeightArrayLocation);
    glVertexAttribPointer(mBoneWeightArrayLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 20));
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



