//
// Created by pc on 2017/12/1.
//

#include "Model.h"
#include "utils.h"

Model::Model()
{
    mShader = nullptr;
}

void Model::Init(AAssetManager *assetManager, const char *modelPath)
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
    mVertexBuffer = new VertexBuffer;
    mVertexBuffer->SetSize(vertexCounts);
    for (int i = 0; i < vertexCounts; ++i) {
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
    mShader->Init(assetManager, "Res/model.vs", "Res/model.fs");

    // 光照
    mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    mShader->SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);

}

void Model::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    glEnable(GL_DEPTH_TEST);
    mVertexBuffer->Bind();

    mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexCount);

    mVertexBuffer->Unbind();
}

void Model::SetPosition(float x, float y, float z)
{
    mModelMatrix = glm::translate(x, y, z);
}
