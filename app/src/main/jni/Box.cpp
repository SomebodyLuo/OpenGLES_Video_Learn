//
// Created by pc on 2018/5/15.
//

#include "Box.h"
#include "utils.h"

Box::Box()
{
    m_vertexNum = 0;
    mShader = nullptr;
}

Box::~Box()
{
    delete g_boneRoot;

    delete[] mVertexBuffer;
    delete mShader;
}


void Box::Init(AAssetManager *assetManager, const char *modelPath)
{

    if ((nullptr == assetManager) || (nullptr == modelPath))
    {
        return;
    }

    //======================1====================
    float boxData[]=
            {
                    //x,y,z
                    // 正方体
//                    -0.5f, 0.5f, 0.5f,
//                    0.5f, 0.5f, 0.5f,
//                    0.5f, 0.5f, -0.5f,
//                    -0.5f, 0.5f, -0.5f,
//                    -0.5f, -0.5f, 0.5f,
//                    0.5f, -0.5f, 0.5f,
//                    0.5f, -0.5f, -0.5f,
//                    -0.5f, -0.5f, -0.5f,

                    //2(3)-----4
                    //|        |
                    //|        |
                    //0--------1(5)
//                    -0.5f, -0.5f, 0.5f,
//                    0.5f, -0.5f, 0.5f,
//                    -0.5f, 0.5f, -0.5f,
//
//                    -0.5f, 0.5f, -0.5f,
//                    0.5f, 0.5f, -0.5f,
//                    0.5f, -0.5f, 0.5f,

                    //2(3)-----4
                    //|        |
                    //|        |
                    //0--------1(5)
                    -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f,

                    -0.5f, 0.5f, 0.0f,
                    0.5f, 0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,

            };

    m_vertexNum = sizeof(boxData)/(sizeof(float)*3);

    LOGI("m_vertexNum = %d", m_vertexNum);
    mVertexBuffer = new VertexBuffer();
    mVertexBuffer->SetSize(m_vertexNum);
    for (int i = 0; i < m_vertexNum; ++i) {
        mVertexBuffer->SetPosition(i, boxData[i*3 + 0], boxData[i*3 + 1], boxData[i*3 + 2]);
        mVertexBuffer->SetColor(i, 0.9f, 0.6f, 0.1f);
    }

    LOGI("Box::Init: mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    //=======================2===================
    // 加载Shader
    mShader = new Shader();
    mShader->Init(assetManager, "Res/box.vs", "Res/box.fs");

    // 光照
    mShader->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);

    //=======================3===================
    g_boneRoot = new Bone();
    g_boneRoot->setPosition(0, 0, 0);
    g_boneRoot->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_boneRoot->setScale(1.0f, 1.0f, 1.0f);

    for(int i=0; i<m_vertexNum; ++i)
    {
        mVertexBuffer->mBoneInfo[i].m_boneNum = 1;

        mVertexBuffer->mVertexes[i].boneCounts = 1;

        for(int j=0; j < mVertexBuffer->mBoneInfo[i].m_boneNum; ++j)
        {
            Bone* pBone = g_boneRoot;
            pBone->mBoneIndex = 78;

            mVertexBuffer->mBoneInfo[i].SetBoneAndWeight(j, pBone, 1.0f);

            mVertexBuffer->mVertexes[i].boneIds[0] = 78;
            mVertexBuffer->mVertexes[i].boneWeights[0] = 1.0f;
        }
    }


    mVertexBuffer->mBoneIndexArray.resize(5);
    mVertexBuffer->mBoneIndexArray[0] = 78;

    mVertexBuffer->mBoneWorldModelMatrixArray.resize(5);
    mVertexBuffer->mBoneOffsetMatrixArray.resize(5);

    LOGI("mVertexBuffer->mBoneIndexArray.size = %d\n", (int)mVertexBuffer->mBoneIndexArray.size());
    LOGI("mVertexBuffer->mBoneIndexArray.capacity = %d\n", (int)mVertexBuffer->mBoneIndexArray.capacity());
    LOGI("mVertexBuffer->mBoneIndexArray[0] = %d\n", mVertexBuffer->mBoneIndexArray[0]);
    LOGI("mVertexBuffer->mBoneIndexArray[1] = %d\n", mVertexBuffer->mBoneIndexArray[1]);
    LOGI("mVertexBuffer->mBoneIndexArray[2] = %d\n", mVertexBuffer->mBoneIndexArray[2]);
    LOGI("mVertexBuffer->mBoneIndexArray[3] = %d\n", mVertexBuffer->mBoneIndexArray[3]);
    LOGI("mVertexBuffer->mBoneIndexArray[4] = %d\n", mVertexBuffer->mBoneIndexArray[4]);

    //compute bone offset

    ComputeWorldModelMatrix(glm::mat4());

    g_boneRoot->ComputeBoneOffset();

}

float time2 = 0.0f;
float angle2 = 0.0f;
void Box::animateBones()
{

    //animate bones manually
    time2 += 0.1f;
    if(time2 > 3.14*2){
        time2 = 0;
    }
    if(glm::sin(time2) > 0){
//        g_bone1->mLocalTranslateMatrix = glm::mul(glm::translate(0.0f,0.01f,0.0f), g_bone1->mLocalTranslateMatrix);
        //g_boneRoot->mLocalTranslateMatrix = glm::mul(glm::translate(0.01f,0.0f,0.0f), g_boneRoot->mLocalTranslateMatrix);
    }else{
//        g_bone1->mLocalTranslateMatrix = glm::mul(glm::translate(0.0f,-0.01f,0.0f), g_bone1->mLocalTranslateMatrix);
        //g_boneRoot->mLocalTranslateMatrix = glm::mul(glm::translate(-0.01f,0.0f,0.0f), g_boneRoot->mLocalTranslateMatrix);
    }

    angle2 += 0.002f;
    if(angle2 == 360.0f)
    {
        angle2 = 0.0f;
    }
    g_boneRoot->mLocalRotationMatrix = glm::mul(glm::rotate(angle2, 0.0f, 0.0f, 1.0f), g_boneRoot->mLocalRotationMatrix);

}


void Box::ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix)
{
    g_boneRoot->ComputeWorldModelMatrix(fatherModelMatrix);
}

void Box::retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb)
{
    for (int i = 0; i < vb->mBoneIndexArray.size(); ++i) {
        if(pBone->mBoneIndex == vb->mBoneIndexArray[i])
        {
            vb->mBoneWorldModelMatrixArray[i] = pBone->mWorldModelMatrix;
            vb->mBoneOffsetMatrixArray[i] = pBone->m_boneOffset.mOffsetMatrix;
        }
    }

    if(pBone->m_pSibling != NULL)
        retrieveBoneMatrices(pBone->m_pSibling, vb);

    if(pBone->m_pFirstChild != NULL)
        retrieveBoneMatrices(pBone->m_pFirstChild, vb);
}

void Box::UpdateMatrices()
{
    retrieveBoneMatrices(g_boneRoot, mVertexBuffer);
}

void Box::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{
    // 因为模型的specularLight跟camera的位置有关，所以必须更新
    mShader->SetVec4("U_CameraPos", cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

//    mShader->BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
    mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), mVertexBuffer);

    // https://www.cnblogs.com/bigdudu/articles/4191042.html
    // 解决缩放不一致，导致法线不垂直的问题
    glm::mat4 it = glm::inverse(mModelMatrix);
    glUniformMatrix4fv(glGetUniformLocation(mShader->mProgram, "IT_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(it));

    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);

    mVertexBuffer->Unbind();
}

void Box::SetPosition(float x, float y, float z)
{
    mModelMatrix = glm::translate(x, y, z);
}

void Box::SetAmbientMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_AmbientMaterial", r, g, b, a);
}

void Box::SetDiffuseMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_DiffuseMaterial", r, g, b, a);
}

void Box::SetSpecularMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_SpecularMaterial", r, g, b, a);
}

void Box::SetTexure(const char *imagePath)
{
    mShader->SetTexture("U_Texture", imagePath);
}

void Box::SetVec4(const char *name, float x, float y, float z, float w)
{
    mShader->SetVec4(name, x, y, z, w);
}