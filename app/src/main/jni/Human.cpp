//
// Created by pc on 2018/5/15.
//

#include "Human.h"
#include "utils.h"

Human::Human() {
    mBoneRoot = nullptr;
}

Human::~Human()
{
    delete mBoneRoot;
}

void Human::Init(AAssetManager *assetManager, const char *modelPath)
{
    struct FloatData
    {
        float v[3];
    };
    struct VertexDefine
    {
        int posIndex;
        int texcoordIndex;
        int normalIndex;
    };

    if ((nullptr == assetManager) || (nullptr == modelPath))
    {
        return;
    }

    // 解析OBJ文件
    int fileSize = 0;
    const char *fileContent = LoadFileContent(assetManager, modelPath, fileSize);
    if (nullptr == fileContent)
    {
        return;
    }

    std::vector<FloatData> positions, texcoords, normals;
    std::vector<VertexDefine> vertexes;
    std::stringstream ssFileContent(fileContent);
    std::string temp;
    char lineStr[256];
    while (!ssFileContent.eof())
    {
        memset(lineStr, 0, 256);

        ssFileContent.getline(lineStr, 256);
        if (strlen(lineStr) > 0)
        {
            if (lineStr[0] == 'v')
            {
                std::stringstream ssLine(lineStr);
                // 注意以下的流输出
                if (lineStr[1] == 't')
                {
                    //纹理坐标
                    ssLine >> temp;
                    FloatData floatData;
                    ssLine >> floatData.v[0];
                    ssLine >> floatData.v[1];
                    texcoords.push_back(floatData);
                } else if (lineStr[1] == 'n') {
                    //法线坐标
                    ssLine >> temp;
                    FloatData floatData;
                    ssLine >> floatData.v[0];
                    ssLine >> floatData.v[1];
                    ssLine >> floatData.v[2];
                    normals.push_back(floatData);
                } else {
                    //顶点坐标
                    ssLine >> temp;
                    FloatData floatData;
                    ssLine >> floatData.v[0];
                    ssLine >> floatData.v[1];
                    ssLine >> floatData.v[2];
                    positions.push_back(floatData);
                }
            } else if (lineStr[0] == 'f') {
                //三角形面的点构成
                std::stringstream ssLine(lineStr);
                ssLine >> temp;

                std::string vertexStr;
                for (int i = 0; i < 3; ++i) {
                    ssLine >> vertexStr;
                    size_t pos = vertexStr.find_first_of('/');
                    std::string posIndexStr = vertexStr.substr(0, pos);
                    size_t pos2 = vertexStr.find_first_of('/', pos + 1);
                    std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - 1 - pos);
                    std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);

                    VertexDefine vd;
                    vd.posIndex = atoi(posIndexStr.c_str());
                    vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
                    vd.normalIndex = atoi(normalIndexStr.c_str());
                    vertexes.push_back(vd);
                }

            }
        }
    }
    delete fileContent;

    // 将模型数据转化成OpenGL顶点数据
    // 注意：模型真正要绘制的点，与其实际含有的点是不一样的。
    // 因为绘制是以三角形面为基础，很多点会被复用，
    // 所以我们在统计总共要绘制的点的数量时，就应以三角形面数 x 3，也就是vertexes
    int vertexCounts = vertexes.size();
    LOGI("vertexCounts = %d", vertexCounts);
    mVertexBuffer = new VertexBuffer();
    mVertexBuffer->SetSize(vertexCounts);
    for (int i = 0; i < vertexCounts; ++i)
    {
        //注意obj文件中三角形面信息里面的索引号是从1开始，而数组索引是从0开始，所以posIndex - 1
        float *temp = positions[vertexes[i].posIndex - 1].v;
        mVertexBuffer->SetPosition(i, temp[0], temp[1], temp[2]);

        temp = texcoords[vertexes[i].texcoordIndex - 1].v;
        mVertexBuffer->SetTexcoord(i, temp[0], temp[1]);

        temp = normals[vertexes[i].normalIndex - 1].v;
        mVertexBuffer->SetNormal(i, temp[0], temp[1], temp[2]);

//        mVertexBuffer->SetMeshInfoId(i);
    }
    LOGI("Human::Init: mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    // 加载Shader
    mShader = new Shader;
    mShader->Init(assetManager, "Res/human.vs", "Res/human.fs");

    // 光照
    // 环境光
    mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    SetAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);

    // 漫反射光
    mShader->SetVec4("U_LightPos", 1.0f, 1.0f, 0.0f, 0.0f);     //方向光！！！
    mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    SetDiffuseMaterial(0.6f, 0.6f, 0.6f, 1.0f);

    // 镜面反射光
    mShader->SetVec4("U_LightSpecular", 1.0f, 1.0f, 1.0f, 1.0f);
    SetSpecularMaterial(1.0f, 1.0f, 1.0f, 1.0f);

    mShader->SetVec4("U_CameraPos", 0.0f, 0.0f, 0.0f, 1.0f);
    mShader->SetVec4("U_LightOpt", 32.0f, 0.0f, 0.0f, 0.0f);

    mShader->SetVec4("thresholdPoint", 0.0f, 0.0f, 0.0f, 0.0f);

    mKeyPoints.Init(6, assetManager, "");
    mKeyPoints.mModelMatrix = glm::translate(3.0f, -8.0f, 2.5f) * glm::scale(0.5f, 0.5f, 0.5f) * glm::rotate(-20.0f, 0.0f, 1.0f, 0.0f);

}

void Human::ParseHumanBody()
{
    LOGI("Human::ParseHumanBody: mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);
    float maxY = 0.0f, minY = 0.0f;
    int maxYIndex = 0, minYIndex = 0;
    float maxX = 0.0f, minX = 0.0f;
    int maxXIndex = 0, minXIndex = 0;

    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {

        if(mVertexBuffer->mVertexes[i].Position[0] > maxX)
        {
            maxX = mVertexBuffer->mVertexes[i].Position[0];
            maxXIndex = i;
        }

        if(mVertexBuffer->mVertexes[i].Position[0] < minX)
        {
            minX = mVertexBuffer->mVertexes[i].Position[0];
            minXIndex = i;
        }

        if(mVertexBuffer->mVertexes[i].Position[1] > maxY)
        {
            maxY = mVertexBuffer->mVertexes[i].Position[1];
            maxYIndex = i;
        }

        if(mVertexBuffer->mVertexes[i].Position[1] < minY)
        {
            minY = mVertexBuffer->mVertexes[i].Position[1];
            minYIndex = i;
        }
    }
    // 头顶 mtopPoint
    mKeyPoints.SetPointPosition(0, mVertexBuffer->mVertexes[maxYIndex].Position[0], mVertexBuffer->mVertexes[maxYIndex].Position[1], mVertexBuffer->mVertexes[maxYIndex].Position[2]);
    LOGI("\n\n---------------------------------\n");
    print_array(mVertexBuffer->mVertexes[maxYIndex].Position, 4);

    // 脚底 mbottomPoint
    mKeyPoints.SetPointPosition(1, mVertexBuffer->mVertexes[minYIndex].Position[0], mVertexBuffer->mVertexes[minYIndex].Position[1], mVertexBuffer->mVertexes[minYIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[minYIndex].Position, 4);

    // 左手 mLeftPoint
    mKeyPoints.SetPointPosition(2, mVertexBuffer->mVertexes[minXIndex].Position[0], mVertexBuffer->mVertexes[minXIndex].Position[1], mVertexBuffer->mVertexes[minXIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[minXIndex].Position, 4);

    // 右手 mRightPoint
    mKeyPoints.SetPointPosition(3, mVertexBuffer->mVertexes[maxXIndex].Position[0], mVertexBuffer->mVertexes[maxXIndex].Position[1], mVertexBuffer->mVertexes[maxXIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[maxXIndex].Position, 4);

    mHeight = maxY - minY;
    mWidth = maxX - minX;

    float headHeight = mHeight * mHeadPercentage;
    float headPartMinY = maxY - headHeight;
    LOGI("thresholdPoint.y = %f\n", headPartMinY);
    mShader->SetVec4("thresholdPoint", 0.0f, headPartMinY, 0.6f, 1.0f);


    float noseZ = -1000.0f;
    int noseZIndex = 0;

    float jawY = 1000.0f, jawZ = -1000.0f;
    int jawIndex = 0;

    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {

        if(mVertexBuffer->mVertexes[i].Position[1] > headPartMinY)
        {
            // 鼻子是头部Z值最大的点
            if(mVertexBuffer->mVertexes[i].Position[2] > noseZ)
            {
                noseZ = mVertexBuffer->mVertexes[i].Position[2];
                noseZIndex = i;
            }

            // 下巴是头部Y值最小或Z值最大的点
            if((mVertexBuffer->mVertexes[i].Position[1] < jawY) || (mVertexBuffer->mVertexes[i].Position[2] > jawZ))
            {
                jawY = mVertexBuffer->mVertexes[i].Position[1];
                jawZ = mVertexBuffer->mVertexes[i].Position[2];
                jawIndex = i;
            }
        }

    }

    // 鼻子 mNosePoint
    mKeyPoints.SetPointPosition(4, mVertexBuffer->mVertexes[noseZIndex].Position[0], mVertexBuffer->mVertexes[noseZIndex].Position[1], mVertexBuffer->mVertexes[noseZIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[noseZIndex].Position, 4);

    // 下巴 mJawPoint
    mKeyPoints.SetPointPosition(5, mVertexBuffer->mVertexes[jawIndex].Position[0], mVertexBuffer->mVertexes[jawIndex].Position[1], mVertexBuffer->mVertexes[jawIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[jawIndex].Position, 4);

    LOGI("---------------------------------\n\n\n");

    // 下面开始计算头部的旋转中心
    // 头部最高点与鼻子的各自垂直延长线的交点
    glm::vec3 headRotateCenter = glm::vec3(mVertexBuffer->mVertexes[maxYIndex].Position[0], mVertexBuffer->mVertexes[noseZIndex].Position[1], mVertexBuffer->mVertexes[maxYIndex].Position[2]);

    // 计算骨骼权重
    mBoneRoot = new Bone();
    mBoneRoot->setPosition(headRotateCenter);
    mBoneRoot->setRotation(0.1f, 0.0f, 1.0f, 0.0f);

    for(int i=0; i < mVertexBuffer->mVertexCount; ++i)
    {
        mVertexBuffer->mBoneInfo[i].m_boneNum = 1;

        for(int j=0; j < mVertexBuffer->mBoneInfo[i].m_boneNum; ++j)
        {
            Bone* pBone = mBoneRoot;
            pBone->mBoneIndex = 99;

            mVertexBuffer->mBoneInfo[i].SetBoneAndWeight(j, pBone, 0.0f);
        }
    }

//    mVertexBuffer->mBoneCountsArray.resize(mVertexBuffer->mVertexCount);
//    mVertexBuffer->mBoneIdsArray.resize(mVertexBuffer->mVertexCount);
//    mVertexBuffer->mBoneWeightArray.resize(mVertexBuffer->mVertexCount);
//    for(int i = 0; i < mVertexBuffer->mVertexCount; ++i)
//    {
//        mVertexBuffer->mBoneCountsArray[i] = 1;
//
//        // 假设每个顶点拥有的骨骼数量不超过4个
//        for(int j = 0; j < mVertexBuffer->mBoneCountsArray[i]; ++j)
//        {
//            mVertexBuffer->mBoneIdsArray[i][j] = 99;
//
//            // 权重的分配
//            // 1. 头部旋转中心点和下巴的连线，构成“厂”形上部区域权重全部为1.0f
//            // 2. headPartMinY以上的其他区域，以Y = Z的函数关系分配权重
//            // 3. 身体其他部分权重为0
//            if(mVertexBuffer->mVertexes[i].Position[1] > headRotateCenter.y)
//            {
//                LOGI("weight=1.0f -1-: point=%d\n", i);
//                mVertexBuffer->mBoneWeightArray[i][j] = 1.0f;
//            }
//            else if((mVertexBuffer->mVertexes[i].Position[1] < headRotateCenter.y) && (mVertexBuffer->mVertexes[i].Position[2] > headRotateCenter.z) && (mVertexBuffer->mVertexes[i].Position[1] > mVertexBuffer->mVertexes[jawIndex].Position[1]))
//            {
//                LOGI("weight=1.0f -2-: point=%d\n", i);
//                mVertexBuffer->mBoneWeightArray[i][j] = 1.0f;
//            }
//            else if(mVertexBuffer->mVertexes[i].Position[1] > headPartMinY)
//            {
//                float dd = (headRotateCenter.y - headPartMinY) / 10.0f;
//                float Y = headRotateCenter.y, Z = headRotateCenter.z;
//                for (int i = 0; i < 10; ++i) {
//                    // 下面需要判断当前的顶点在Y = -Z直线的右边
//                    if(RightOfLine(mVertexBuffer->mVertexes[i].Position[1], mVertexBuffer->mVertexes[i].Position[2], \
//                        Y - dd, Z - dd, Y - dd - dd, Z))
//                    {
//                        LOGI("0.0f<weight<1.0f : point=%d\n", i);
//                        mVertexBuffer->mBoneWeightArray[i][j] = (10 - i) * 0.1f;
//                    }
//                    Y -= dd;
//                    Z -= dd;
//                }
//            }
//            else
//            {
//                LOGI("weight=0.0f : point=%d\n", i);
//                mVertexBuffer->mBoneWeightArray[i][j] = 0.0f;
//            }
//        }
//    }

    mVertexBuffer->mBoneIndexArray.resize(5);
    mVertexBuffer->mBoneIndexArray[0] = 99;

    mVertexBuffer->mBoneWorldModelMatrixArray.resize(5);
    mVertexBuffer->mBoneOffsetMatrixArray.resize(5);

    ComputeWorldModelMatrix(glm::mat4());

    mBoneRoot->ComputeBoneOffset();
}

bool Human::RightOfLine(float targetPointX, float targetPointY, float linePoint1X, float linePoint1Y, float linePoint2X, float linePoint2Y)
{

    float result = ((linePoint2Y - linePoint1Y) * targetPointX + (linePoint1X - linePoint2X) * targetPointY + (linePoint2X * linePoint1Y - linePoint1X * linePoint2Y));

    return (result <= 0);
}

void Human::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{
#if 1
    // 1. 画人
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

    // draw points
    // add vertex shader: gl_PointSize = 8.0;
//    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {
//        glDrawArrays(GL_POINTS, i, 1);
//    }

    mVertexBuffer->Unbind();
#endif
    // 2. 画点
    mKeyPoints.DrawStaticMesh(viewMatrix, projectionMatrix, cameraPos);
}

void Human::ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix)
{
    mBoneRoot->ComputeWorldModelMatrix(fatherModelMatrix);
}

void Human::retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb)
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

void Human::UpdateMatrices()
{
    retrieveBoneMatrices(mBoneRoot, mVertexBuffer);
}


void Human::animateBones()
{
    angle += df;
    if(angle > 20.0f)
    {
        df = -0.001f;
    }
    if(angle < -20.0f)
    {
        df = 0.001f;
    }
    mBoneRoot->mLocalRotationMatrix = glm::mul(glm::rotate(angle, 0.0f, 1.0f, 0.0f), mBoneRoot->mLocalRotationMatrix);

}