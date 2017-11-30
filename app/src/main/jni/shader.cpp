//
// Created by pc on 2017/11/30.
//

#include "shader.h"
#include "utils.h"

Shader::Shader()
{
    mAssetManager = nullptr;
    mProgram = 0;
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

    GLuint fsShader = CompileShader(GL_VERTEX_SHADER, fsCode);
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

    mPositionLocation = glGetAttribLocation(mProgram, "position");
    mColorLocation = glGetAttribLocation(mProgram, "color");
    mNormalLocation = glGetAttribLocation(mProgram, "normal");

    mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
    mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
    mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");

    return true;
}


