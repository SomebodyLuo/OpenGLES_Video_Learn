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
    delete[] mAfterVertexBuffer;
    delete mShader;
    delete mShaderDynamic;

}
 

void SkinMesh::UpdateVertices()
{

    for(int i=0; i<m_vertexNum; ++i)
    {
        mVertexBuffer->BlendVertex(i, mAfterVertexBuffer);
    }
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
    mShader->Bind(glm::value_ptr(identityMat), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

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
//    mShaderDynamic->SetVec4("U_Bool", 0.0f, 0.0f, 0.0f, 0.0f);

    glm::mat4 identityMat = glm::mat4();
    mShaderDynamic->Bind(glm::value_ptr(identityMat), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
        glDrawArrays(GL_POINTS, i, 1);
    }

//    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);

    // draw lines
    glLineWidth(2.0f);
    for (int i = 0; i < mVertexBuffer->mVertexCount - 1; ++i) {
        glDrawArrays(GL_LINES, i, 2);
    }

    mVertexBuffer->Unbind();

}

float time1 = 0;
void SkinMesh::animateBones()
{

    //animate bones manually
    time1 += 0.1f;
    if(time1 > 3.14*2){
        time1 = 0;
    }
    if(glm::sin(time1) > 0){
        g_bone1->mLocalMatrix = glm::mul(glm::translate(0.0f,0.01f,0.0f), g_bone1->mLocalMatrix);
        g_boneRoot->mLocalMatrix = glm::mul(glm::translate(0.01f,0.0f,0.0f), g_boneRoot->mLocalMatrix);
    }else{
        g_bone1->mLocalMatrix = glm::mul(glm::translate(0.0f,-0.01f,0.0f), g_bone1->mLocalMatrix);
        g_boneRoot->mLocalMatrix = glm::mul(glm::translate(-0.01f,0.0f,0.0f), g_boneRoot->mLocalMatrix);
    }
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
    }

    mAfterVertexBuffer = new VertexBuffer();
    mAfterVertexBuffer->SetSize(m_vertexNum);
    for (int i = 0; i < m_vertexNum; ++i) {
        mAfterVertexBuffer->SetPosition(i, _meshData[i*3], _meshData[i*3 + 1], _meshData[i*3 + 2]);
    }
    LOGI("mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    //=======================2===================
    // 加载Shader
    mShader = new Shader;
    mShader->Init(assetManager, "Res/skeleton.vs", "Res/skeleton.fs");

    // 光照
    mShader->SetVec4("U_LightPos", 0.0f, 20.0f, 0.0f, 0.0f);
    mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    mShader->SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
    mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    mShader->SetVec4("U_DiffuseMaterial", 0.6f, 0.6f, 0.6f, 1.0f);
    mShader->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);
    mShader->SetVec4("U_Bool", 1.0f, 0.0f, 0.0f, 0.0f);

    mShaderDynamic = new Shader;
    mShaderDynamic->Init(assetManager, "Res/skeleton_dynamic.vs", "Res/skeleton_dynamic.fs");

    // 光照
    mShaderDynamic->SetVec4("U_LightPos", 0.0f, 20.0f, 0.0f, 0.0f);
    mShaderDynamic->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    mShaderDynamic->SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
    mShaderDynamic->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    mShaderDynamic->SetVec4("U_DiffuseMaterial", 0.6f, 0.6f, 0.6f, 1.0f);
    mShaderDynamic->SetVec4("U_PointColor", 0.6f, 0.6f, 0.6f, 1.0f);
    mShaderDynamic->SetVec4("U_Bool", 1.0f, 0.0f, 0.0f, 0.0f);

    //=======================3===================
    g_boneRoot = new Bone(0, 0, 0);

    g_bone1 = new Bone(0.2, 0, 0);
    g_bone2 = new Bone(0.2, 0, 0);
    g_bone31 = new Bone(0.2, 0.1, 0);
    g_bone32 = new Bone(0.2, -0.1, 0);

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

    for (int i = 0; i < m_vertexNum; ++i) {
        mVertexBuffer->mVertexes[i].BoneNumber[0] = _skinInfo[i*9];
        mVertexBuffer->mVertexes[i].BoneIdArray[0] = _skinInfo[i * 9 + 1];
        mVertexBuffer->mVertexes[i].BoneIdArray[1] = _skinInfo[i * 9 + 2];
        mVertexBuffer->mVertexes[i].BoneIdArray[2] = _skinInfo[i * 9 + 3];
        mVertexBuffer->mVertexes[i].BoneIdArray[3] = _skinInfo[i * 9 + 4];
        mVertexBuffer->mVertexes[i].BoneWeightArray[0] = _skinInfo[i * 9 + 5];
        mVertexBuffer->mVertexes[i].BoneWeightArray[1] = _skinInfo[i * 9 + 6];
        mVertexBuffer->mVertexes[i].BoneWeightArray[2] = _skinInfo[i * 9 + 7];
        mVertexBuffer->mVertexes[i].BoneWeightArray[3] = _skinInfo[i * 9 + 8];
    }

    //set skin info
    for(int i=0; i<m_vertexNum; ++i)
    {
        mVertexBuffer->mBoneInfo[i].m_boneNum = _skinInfo[i*9];

        for(int j=0; j < mVertexBuffer->mBoneInfo[i].m_boneNum; ++j)
        {
            Bone* pBone = g_boneRoot;

            if(_skinInfo[ i*9 + 1 + j] == 1)
                pBone = g_bone1;

            else if(_skinInfo[ i*9 + 1 + j] == 2)
                pBone = g_bone2;

            else if(_skinInfo[i*9+1+j]==31)
                pBone = g_bone31;

            else if(_skinInfo[i*9+1+j]==32)
                pBone = g_bone32;

            mVertexBuffer->mBoneInfo[i].SetBoneAndWeight(j, pBone, _skinInfo[i*9 + 5 + j]);
        }
    }

    //compute bone offset

    g_boneRoot->ComputeWorldPos(glm::mat4());

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