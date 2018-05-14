//
// Created by pc on 2018/5/15.
//

#include "Human.h"
#include "utils.h"

Human::Human() {

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
    }
    LOGI("mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);

    // 加载Shader
    mShader = new Shader;
    mShader->Init(assetManager, "Res/human.vs", "Res/human.fs");

    // 光照
    // 环境光
    mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 5.0f, 1.0f);
    SetAmbientMaterial(0.3f, 0.3f, 0.3f, 1.0f);

    // 漫反射光
    mShader->SetVec4("U_LightPos", 1.0f, 1.0f, 3.0f, 0.0f);     //方向光！！！
    mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    SetDiffuseMaterial(0.6f, 0.6f, 0.6f, 1.0f);

    // 镜面反射光
    mShader->SetVec4("U_LightSpecular", 1.0f, 1.0f, 1.0f, 1.0f);
    SetSpecularMaterial(1.0f, 1.0f, 1.0f, 1.0f);

    mShader->SetVec4("U_CameraPos", 0.0f, 0.0f, 0.0f, 1.0f);
    mShader->SetVec4("U_LightOpt", 32.0f, 0.0f, 0.0f, 0.0f);

    mKeyPoints.Init(5, assetManager, "");
    mKeyPoints.mModelMatrix = glm::translate(3.0f, -8.0f, 2.5f) * glm::scale(0.5f, 0.5f, 0.5f) * glm::rotate(-20.0f, 0.0f, 1.0f, 0.0f);

}

void Human::ParseHumanBody()
{
    LOGI("mVertexBuffer->mVertexCount = %d", mVertexBuffer->mVertexCount);
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
    // mtopPoint
    mKeyPoints.SetPointPosition(0, mVertexBuffer->mVertexes[maxYIndex].Position[0], mVertexBuffer->mVertexes[maxYIndex].Position[1], mVertexBuffer->mVertexes[maxYIndex].Position[2]);
    LOGI("\n\n---------------------------------\n");
    print_array(mVertexBuffer->mVertexes[maxYIndex].Position, 4);

    // mbottomPoint
    mKeyPoints.SetPointPosition(1, mVertexBuffer->mVertexes[minYIndex].Position[0], mVertexBuffer->mVertexes[minYIndex].Position[1], mVertexBuffer->mVertexes[minYIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[minYIndex].Position, 4);

    // mLeftPoint
    mKeyPoints.SetPointPosition(2, mVertexBuffer->mVertexes[minXIndex].Position[0], mVertexBuffer->mVertexes[minXIndex].Position[1], mVertexBuffer->mVertexes[minXIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[minXIndex].Position, 4);

    // mRightPoint
    mKeyPoints.SetPointPosition(3, mVertexBuffer->mVertexes[maxXIndex].Position[0], mVertexBuffer->mVertexes[maxXIndex].Position[1], mVertexBuffer->mVertexes[maxXIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[maxXIndex].Position, 4);

    mHeight = maxY - minY;
    mWidth = maxX - minX;

    float headHeight = mHeight * mHeadPercentage;
    float jawY = maxY - headHeight;
    float noseZ = -1000.0f;
    int noseZIndex = 0;

    for (int i = 0; i < mVertexBuffer->mVertexCount; ++i) {

        if(mVertexBuffer->mVertexes[i].Position[1] > jawY)
        {
            if(mVertexBuffer->mVertexes[i].Position[2] > noseZ)
            {
                noseZ = mVertexBuffer->mVertexes[i].Position[2];
                noseZIndex = i;
            }
        }
    }

    // nose
    mKeyPoints.SetPointPosition(4, mVertexBuffer->mVertexes[noseZIndex].Position[0], mVertexBuffer->mVertexes[noseZIndex].Position[1], mVertexBuffer->mVertexes[noseZIndex].Position[2]);
    print_array(mVertexBuffer->mVertexes[noseZIndex].Position, 4);

    LOGI("---------------------------------\n\n\n");
}

void Human::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos)
{
#if 1
    // 因为模型的specularLight跟camera的位置有关，所以必须更新
    mShader->SetVec4("U_CameraPos", cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

    // https://www.cnblogs.com/bigdudu/articles/4191042.html
    // 解决缩放不一致，导致法线不垂直的问题
    glm::mat4 it = glm::inverse(mModelMatrix);

    mShader->BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix),
                     glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(mShader->mProgram, "IT_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(it));

    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);

    mVertexBuffer->Unbind();
#endif

    mKeyPoints.DrawStaticMesh(viewMatrix, projectionMatrix, cameraPos);
}
