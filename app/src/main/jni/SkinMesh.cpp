#include "SkinMesh.h"


SkinMesh::SkinMesh(void)
{
	m_vertexNum = 0;
    mShader = nullptr;
    mShaderDynamic = nullptr;

    dir	= -1;
    dir2 = -1;
}

SkinMesh::~SkinMesh(void)
{
    delete g_boneRoot;

    delete g_bone1;
    delete g_bone2;
    delete g_bone31;
    delete g_bone32;

    delete[] mVertexBuffer;
    delete mShader;
    delete mShaderDynamic;

}
 




void SkinMesh::DrawStaticMesh(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{

    // 因为模型的specularLight跟camera的位置有关，所以必须更新
//    mShader->SetVec4("U_CameraPos", cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

    mShader->SetVec4("U_PointColor", 0.2f, 0.9f, 0.2f, 1.0f);
//    mShader->SetVec4("U_Bool", 1.0f, 0.0f, 0.0f, 0.0f);

    glm::mat4 identityMat = glm::translate(-0.1f, -0.1f, 0.0f);
    mShader->BindMVP(glm::value_ptr(identityMat), glm::value_ptr(viewMatrix),
                     glm::value_ptr(projectionMatrix));

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
        glDrawArrays(GL_POINTS, i, 1);
    }

    // draw lines
//    glLineWidth(2.0f);
//    for (int i = 0; i < mVertexBuffer->mVertexCount - 1; ++i) {
//        glDrawArrays(GL_LINES, i, 2);
//    }

    mVertexBuffer->Unbind();
}

void SkinMesh::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{

    // 因为模型的specularLight跟camera的位置有关，所以必须更新
//    mShader->SetVec4("U_CameraPos", cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

    mShaderDynamic->SetVec4("U_PointColor", 0.9f, 0.2f, 0.2f, 1.0f);

    glm::mat4 identityMat = glm::mat4();
    mShaderDynamic->Bind(glm::value_ptr(identityMat), glm::value_ptr(identityMat), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), mVertexBuffer);

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
        glDrawArrays(GL_POINTS, i, 1);
    }

//    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);

    // draw lines
//    glLineWidth(2.0f);
//    for (int i = 0; i < mVertexBuffer->mVertexCount - 1; ++i) {
//        glDrawArrays(GL_LINES, i, 2);
//    }

    mVertexBuffer->Unbind();

}

float time1 = 0.0f;
float angle = 0.0f;
void SkinMesh::animateBones()
{

    //animate bones manually
    time1 += 0.1f;
    if(time1 > 3.14*2){
        time1 = 0;
    }
    if(glm::sin(time1) > 0){
//        g_bone1->mLocalTranslateMatrix = glm::mul(glm::translate(0.0f,0.01f,0.0f), g_bone1->mLocalTranslateMatrix);
        //g_boneRoot->mLocalTranslateMatrix = glm::mul(glm::translate(0.01f,0.0f,0.0f), g_boneRoot->mLocalTranslateMatrix);
    }else{
//        g_bone1->mLocalTranslateMatrix = glm::mul(glm::translate(0.0f,-0.01f,0.0f), g_bone1->mLocalTranslateMatrix);
        //g_boneRoot->mLocalTranslateMatrix = glm::mul(glm::translate(-0.01f,0.0f,0.0f), g_boneRoot->mLocalTranslateMatrix);
    }

    angle += 0.001f;
    if(angle == 360.0f)
    {
        angle = 0.0f;
    }
    g_boneRoot->mLocalRotationMatrix = glm::mul(glm::rotate(angle, 0.0f, 0.0f, 1.0f), g_boneRoot->mLocalRotationMatrix);

}


void SkinMesh::ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix)
{
    g_boneRoot->ComputeWorldModelMatrix(fatherModelMatrix);
}

void SkinMesh::retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb)
{
    for (int i = 0; i < vb->mBoneIndexArray.size(); ++i) {
        if(pBone->mBoneIndex == vb->mBoneIndexArray[i])
        {
            vb->mBoneWorldTranslateMatrixArray[i] = pBone->mLocalTranslateMatrix;
            vb->mBoneWorldRotationMatrixArray[i] = pBone->mLocalRotationMatrix;
            vb->mBoneWorldModelMatrixArray[i] = pBone->mWorldModelMatrix;
            vb->mBoneOffsetMatrixArray[i] = pBone->m_boneOffset.mOffsetMatrix;
        }
    }

    if(pBone->m_pSibling != NULL)
        retrieveBoneMatrices(pBone->m_pSibling, vb);

    if(pBone->m_pFirstChild != NULL)
        retrieveBoneMatrices(pBone->m_pFirstChild, vb);
}

void SkinMesh::UpdateMatrices()
{
    retrieveBoneMatrices(g_boneRoot, mVertexBuffer);
}


//==================================================================================================
void SkinMesh::Init(AAssetManager *assetManager, const char *modelPath)
{


    if ((nullptr == assetManager) || (nullptr == modelPath))
    {
        return;
    }

    //======================1====================
    float _meshData[]=
    {
            //x,y,z

            -0.1,0.05,0,

            0.1,0.05,0,

            0.3,0.05,0,

            0.45,0.06,0,

            0.6,0.15,0,

            0.65,0.1,0,



            0.5,0,0,



            0.65,-0.1,0,

            0.6,-0.15,0,

            0.45,-0.06,0,

            0.3,-0.05,0,

            0.1,-0.05,0,

            -0.1,-0.05,0,

    };

    m_vertexNum = sizeof(_meshData)/(sizeof(float)*3);

    LOGI("m_vertexNum = %d", m_vertexNum);
    mVertexBuffer = new VertexBuffer();
    mVertexBuffer->SetSize(m_vertexNum);
    for (int i = 0; i < m_vertexNum; ++i) {
        mVertexBuffer->SetPosition(i, _meshData[i*3], _meshData[i*3 + 1], _meshData[i*3 + 2]);
        mVertexBuffer->SetColor(i, 0.9f, 0.6f, 0.1f);
//        mVertexBuffer->SetMeshInfoId(i);
    }

    LOGI("SkinMesh::Init: mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    //=======================2===================
    // 加载Shader
    mShader = new Shader();
    mShader->Init(assetManager, "Res/skeleton.vs", "Res/skeleton.fs");

    // 光照
    mShader->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);

    mShaderDynamic = new Shader();
    mShaderDynamic->Init(assetManager, "Res/skeleton_dynamic.vs", "Res/skeleton_dynamic.fs");

    // 光照
    mShaderDynamic->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);

    //=======================3===================
    g_boneRoot = new Bone();
    g_boneRoot->setPosition(0, 0, 0);
    g_boneRoot->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_boneRoot->setScale(1.0f, 1.0f, 1.0f);

    g_bone1 = new Bone();
    g_bone1->setPosition(0.2, 0, 0);
    g_bone1->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_bone1->setScale(1.0f, 1.0f, 1.0f);

    g_bone2 = new Bone();
    g_bone2->setPosition(0.2, 0, 0);
    g_bone2->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_bone2->setScale(1.0f, 1.0f, 1.0f);

    g_bone31 = new Bone();
    g_bone31->setPosition(0.2, 0.1, 0);
    g_bone31->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_bone31->setScale(1.0f, 1.0f, 1.0f);

    g_bone32 = new Bone();
    g_bone32->setPosition(0.2, -0.1, 0);
    g_bone32->setRotation(0.1f, 0.0f, 0.0f, 1.0f);
    g_bone32->setScale(1.0f, 1.0f, 1.0f);

    g_boneRoot->SetFirstChild(g_bone1);
    g_bone1->SetFirstChild(g_bone2);
    g_bone2->SetFirstChild(g_bone31);
    g_bone31->SetSibling(g_bone32);

    float _skinInfo[] =
    {
            //bone_num,bone id(0,1,2,31 or 32), bone weight 1~4,

            1,  		0, -1, -1, -1,    		1.0, 0.0, 0.0, 0.0,

            2,  		0,  1, -1, -1, 			0.5, 0.5, 0.0, 0.0,

            2,  		1,  2, -1, -1,  		0.5, 0.5, 0.0, 0.0,

            2,  		2,  31, -1, -1, 		0.3, 0.7, 0.0, 0.0,

            2,  		2,  31, -1, -1, 		0.2, 0.8, 0.0, 0.0,

            1,  		31, -1, -1, -1, 		1.0, 0.0, 0.0, 0.0,



            2,  		31, 32, -1, -1, 		0.5, 0.5, 0.0, 0.0,



            1,  		32, -1, -1, -1, 		1.0, 0.0, 0.0, 0.0,

            2,  		2,  32, -1, -1, 		0.2, 0.8, 0.0, 0.0,

            2,  		2,  32, -1, -1, 		0.3, 0.7, 0.0, 0.0,

            2,  		1,  2, -1, -1,  		0.5, 0.5, 0.0, 0.0,

            2,  		0,  1, -1, -1, 			0.5, 0.5, 0.0, 0.0,

            1,  		0, -1, -1, -1,    		1.0, 0.0, 0.0, 0.0,

    };

    for(int i=0; i<m_vertexNum; ++i)
    {
        mVertexBuffer->mBoneInfo[i].m_boneNum = _skinInfo[i*9];

        mVertexBuffer->mVertexes[i].boneCounts = _skinInfo[i*9];

        for(int j=0; j < mVertexBuffer->mBoneInfo[i].m_boneNum; ++j)
        {
            Bone* pBone = g_boneRoot;
            pBone->mBoneIndex = 0;

            if(_skinInfo[ i*9 + 1 + j] == 1)
            {
                pBone = g_bone1;
                pBone->mBoneIndex = 1;
            }
            else if(_skinInfo[ i*9 + 1 + j] == 2)
            {
                pBone = g_bone2;
                pBone->mBoneIndex = 2;
            }
            else if(_skinInfo[i*9 + 1 + j] == 31)
            {
                pBone = g_bone31;
                pBone->mBoneIndex = 31;
            }
            else if(_skinInfo[i*9 + 1 + j] == 32)
            {
                pBone = g_bone32;
                pBone->mBoneIndex = 32;
            }

            mVertexBuffer->mBoneInfo[i].SetBoneAndWeight(j, pBone, _skinInfo[i*9 + 5 + j]);

            // 给与骨骼权重
            mVertexBuffer->mVertexes[i].boneIds[j] = _skinInfo[ i*9 + 1 + j];
            mVertexBuffer->mVertexes[i].boneWeights[j] = _skinInfo[i*9 + 5 + j];
        }
    }

    mVertexBuffer->mBoneIndexArray.resize(5);
    mVertexBuffer->mBoneIndexArray[0] = 0;
    mVertexBuffer->mBoneIndexArray[1] = 1;
    mVertexBuffer->mBoneIndexArray[2] = 2;
    mVertexBuffer->mBoneIndexArray[3] = 31;
    mVertexBuffer->mBoneIndexArray[4] = 32;
    mVertexBuffer->mBoneWorldTranslateMatrixArray.resize(5);
    mVertexBuffer->mBoneWorldRotationMatrixArray.resize(5);
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


void SkinMesh::SetAmbientMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_AmbientMaterial", r, g, b, a);
//    mShaderDynamic
}

void SkinMesh::SetDiffuseMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_DiffuseMaterial", r, g, b, a);
//    mShaderDynamic
}

void SkinMesh::SetSpecularMaterial(float r, float g, float b, float a)
{
    mShader->SetVec4("U_SpecularMaterial", r, g, b, a);
//    mShaderDynamic
}