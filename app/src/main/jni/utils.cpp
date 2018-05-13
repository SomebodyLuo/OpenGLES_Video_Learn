//
// Created by pc on 2017/11/24.
//

#include "utils.h"

// JNI-C++访问Android APP资源的方法
char *LoadFileContent(AAssetManager *assetManager, const char *path, int &filesize)
{
    char *fileContent = nullptr;
    filesize = 0;

    if (nullptr == assetManager)
    {
        LOGD("nullptr == assetManager");
        return nullptr;
    }

    // 打开
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (nullptr == asset)
    {
        LOGD("AAssetManager_open failed!");
        return nullptr;
    }

    filesize = AAsset_getLength(asset);
    fileContent = new char[filesize + 1];

    // 读文件
    AAsset_read(asset, fileContent, filesize);
    fileContent[filesize] = '\0';

    // 关闭文件
    AAsset_close(asset);

    return fileContent;
}


char *LoadJsonFile(AAssetManager *assetManager, const char *path, int &filesize)
{
    char *fileContent = nullptr;
    filesize = 0;

    if (nullptr == assetManager)
    {
        LOGD("nullptr == assetManager");
        return nullptr;
    }

    // 打开
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (nullptr == asset)
    {
        LOGD("AAssetManager_open failed!");
        return nullptr;
    }

    filesize = AAsset_getLength(asset);
    fileContent = new char[filesize + 1];

    // 读文件
    AAsset_read(asset, fileContent, filesize);
    fileContent[filesize] = '\0';

    std::vector<std::string>joint_name;
    std::vector<glm::vec3>joint_pos;
    std::vector<glm::quat>joint_rot;
    std::vector<int> joint_parent_ID;
    joint_name.reserve(100);
    joint_pos.reserve(100);
    joint_rot.reserve(100);
    joint_parent_ID.reserve(100);

    // 解析JSON文件
    rapidjson::Document jdoc;
    jdoc.Parse(fileContent);
    LOGI("\nJSON: jdoc.IsObject() %s\n", jdoc.IsObject() ? "true" : "false");
    LOGI("JSON: jdoc[\"root\"].IsString() %s\n", jdoc["root"].IsString() ? "true" : "false");
    LOGI("JSON: jdoc[\"root\"].IsObject() %s\n", jdoc["root"].IsObject() ? "true" : "false");
    LOGI("JSON: jdoc[\"root\"].IsArray() %s\n", jdoc["root"].IsArray() ? "true" : "false");

    rapidjson::Value &obj = jdoc["root"];
    std::string name = obj["name"].GetString();
    glm::vec3 pos;
    glm::quat rot;
    rapidjson::Value &pos_read = obj["pos"];
    pos = glm::vec3(pos_read[0].GetFloat(), pos_read[1].GetFloat(), pos_read[2].GetFloat());

    rapidjson::Value &rot_read = obj["rot"];
    double theta = rot_read[0].GetFloat() / M_PI * 180;
    glm::vec3 axis(rot_read[1].GetFloat(), rot_read[2].GetFloat(), rot_read[3].GetFloat());
    glm::normalize(axis);
    rot.w = float(cos(theta/2));
    rot.x = float(sin(theta/2) * axis[0]);
    rot.y = float(sin(theta/2) * axis[1]);
    rot.z = float(sin(theta/2) * axis[2]);



//    rapidjson::GenericObject object = jdoc["root"].GetObject();
//    const char *name = jdoc["root"].GetObject()["name"].GetString();
    LOGI("JSON: %s\n", name.c_str());

#if 0
    // 2. Modify it by DOM.
    rapidjson::Value& s = jdoc["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jdoc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    LOGI("json: %s\n", buffer.GetString());
//    std::cout << buffer.GetString() << std::endl;
#endif

    delete[] fileContent;

    // 关闭文件
    AAsset_close(asset);

    return nullptr;
}


static void JsonReadIteration(std::vector<std::string>&joint_name, \
                                std::vector<glm::vec3> &joint_pos,  \
                                std::vector<glm::quat>&joint_rot,   \
                                std::vector<int> &joint_parent_ID,  \
                                int joint_p_ID, rapidjson::Value &joint_objects)
{


    for (auto &m : joint_objects.GetArray())
    {
        rapidjson::Value &obj = m;

        std::string name = obj["name"].GetString();
        LOGI("JSON: child name = %s\n", name.c_str());

        glm::vec3 pos;
        rapidjson::Value &pos_read = obj["pos"];
        pos = glm::vec3(pos_read[0].GetFloat(), pos_read[1].GetFloat(), pos_read[2].GetFloat());

        glm::quat rot;
        rapidjson::Value &rot_read = obj["rot"];
        double theta = rot_read[0].GetFloat() / M_PI * 180;
        glm::vec3 axis(rot_read[1].GetFloat(), rot_read[2].GetFloat(), rot_read[3].GetFloat());
        glm::normalize(axis);
        rot.w = float(cos(theta/2));
        rot.x = float(sin(theta/2) * axis[0]);
        rot.y = float(sin(theta/2) * axis[1]);
        rot.z = float(sin(theta/2) * axis[2]);

        // 将根节点信息入库
        joint_name.push_back(name);
        joint_pos.push_back(pos);
        joint_rot.push_back(rot);
        joint_parent_ID.push_back(joint_p_ID);

        rapidjson::Value &child = obj["children"];
        int children_counts = child.GetArray().Size();
        LOGI("JSON: children_counts = %d\n", children_counts);

        JsonReadIteration(joint_name, joint_pos, joint_rot, joint_parent_ID, joint_name.size() - 1, child);
    }
}

bool ParseJsonFile(AAssetManager *assetManager, const char *path, \
    std::vector<std::string> &joint_name, \
    std::vector<glm::vec3> &joint_pos, \
    std::vector<glm::quat> &joint_rot, \
    std::vector<int> &joint_parent_ID)
{
    char *fileContent = nullptr;
    int filesize = 0;

    fileContent = LoadFileContent(assetManager, path, filesize);
    if(nullptr == fileContent)
    {
        return false;
    }

    joint_name.reserve(100);
    joint_pos.reserve(100);
    joint_rot.reserve(100);
    joint_parent_ID.reserve(100);

    // 解析JSON文件
    rapidjson::Document jdoc;
    jdoc.Parse(fileContent);

    rapidjson::Value &obj = jdoc["root"];

    std::string name = obj["name"].GetString();
    LOGI("JSON: root name = %s\n", name.c_str());

    glm::vec3 pos;
    rapidjson::Value &pos_read = obj["pos"];
    pos = glm::vec3(pos_read[0].GetFloat(), pos_read[1].GetFloat(), pos_read[2].GetFloat());

    glm::quat rot;
    rapidjson::Value &rot_read = obj["rot"];
    double theta = rot_read[0].GetFloat() / M_PI * 180;
    glm::vec3 axis(rot_read[1].GetFloat(), rot_read[2].GetFloat(), rot_read[3].GetFloat());
    glm::normalize(axis);
    rot.w = float(cos(theta/2));
    rot.x = float(sin(theta/2) * axis[0]);
    rot.y = float(sin(theta/2) * axis[1]);
    rot.z = float(sin(theta/2) * axis[2]);

    // 将根节点信息入库
    joint_name.push_back(name);
    joint_pos.push_back(pos);
    joint_rot.push_back(rot);
    joint_parent_ID.push_back(-1);

    rapidjson::Value &child = obj["children"];
    int children_counts = child.GetArray().Size();
    LOGI("JSON: children_counts = %d\n", children_counts);

    JsonReadIteration(joint_name, joint_pos, joint_rot, joint_parent_ID, joint_name.size() - 1, child);


    delete[] fileContent;

    return true;
}

GLuint CompileShader(GLenum shaderType, const char *shaderCode)
{
    // 根据shader类型创建一个Shader对象（标识符）
    GLuint shader = glCreateShader(shaderType);

    // 加载Shader源码内容
    glShaderSource(shader, 1, &shaderCode, nullptr);

    // 编译Shader源码
    glCompileShader(shader);

    // 获取编译结果
    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (GL_FALSE == compileResult)
    {
        char szLog[1024] = {0};
        GLsizei logLen = 0;
        // 获取编译结果Log日志
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        printf("Compile Shader fail error log : %s \nshader code :\n%s\n", szLog, shaderCode);

        // shader既然编译失败了就释放吧！
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

// vs--Vertex Shader; fs--Fragment Shader
// 注意这个程序是在GPU上面跑，而不是在CPU上面跑！！！！
GLuint CreateProgram(GLuint vsShader, GLuint fsShader)
{
    // 创建一个程序
    GLuint program = glCreateProgram();

    // 将vs/fs Shader绑定到程序上
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);

    // Link程序
    glLinkProgram(program);

    // 解绑定
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);

    // 获取Link结果
    GLint nResult;
    glGetProgramiv(program, GL_LINK_STATUS, &nResult);
    if (GL_FALSE == nResult)
    {
        char log[1024] = {0};
        GLsizei writed = 0;
        // 获取Link结果Log日志
        glGetProgramInfoLog(program, 1024, &writed, log);

        printf("create GPU program failed, link error : %s \n", log);

        // program有问题，删除
        glDeleteProgram(program);

        program = 0;
    }

    return program;
}

float GetFrameTime()
{
    static unsigned long long lastTime = 0, currentTime = 0;

    timeval current;
    gettimeofday(&current, nullptr);
    currentTime = current.tv_sec * 1000 + current.tv_usec / 1000;

    unsigned long long frameTime = (lastTime == 0) ? 0 : currentTime - lastTime;
    lastTime = currentTime;

    return float(frameTime) / 1000.0f;

}

unsigned char*DecodeBMP(unsigned char *bmpFileData, int &width, int &height)
{
    if (0x4D42 == *((unsigned short*)bmpFileData))
    {
        int pixelDataOffset = *((int *)(bmpFileData + 10));
        width = *((int *)(bmpFileData + 18));
        height = *((int *)(bmpFileData + 22));
        unsigned char* pixelData = bmpFileData + pixelDataOffset;

        int size = width * height * 3;
        // 位图BMP文件的像素排列是BGR, OpenGL可识别的像素排列是RGB, 所以需要转换：
        for (int i = 0; i < size; i += 3) {
            unsigned char temp = pixelData[i];
            pixelData[i] = pixelData[i + 2];
            pixelData[i + 2] = temp;
        }
        return pixelData;
    }

    return nullptr;
}

GLuint CreateTexture2D(unsigned char *pixelData, int width, int height, GLenum type)
{
    GLuint texture = 0;

    // 申请一个纹理
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //纹理放大时，用什么算法采集像素
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //纹理缩小时，用什么算法采集像素
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //param1: 纹理编号
    //param2: MipMap级别，OpenGL会选择不同的级别去着色(占内存)。比如原始图像是128x128，缩小一倍就是64x64；原始级别就是0，下一级别是1，
    //param3: internal format, 像素在显卡中的格式
    //param4: 宽
    //param5: 高
    //param6: border, 必须写0
    //param7: 纹理数据在内存中是什么格式
    //param8: 像素数据每个分量是什么类型
    //param9: 像素数据地址
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLuint CreateTexture2DFromBMP(AAssetManager *assetManager, const char*bmpPath)
{
    int nFileSize = 0;

    unsigned char *bmpFileContent = (unsigned char *)LoadFileContent(assetManager, bmpPath, nFileSize);
    if (nullptr == bmpFileContent)
    {
        return 0;
    }

    int bmpWidth = 0, bmpHeight = 0;
    unsigned char *pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
    if (nullptr == pixelData)
    {
        delete[] bmpFileContent;
        return 0;
    }

    GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
    delete[] bmpFileContent;

    LOGI("CreateTexture2DFromBMP: texture = %u", texture);

    return texture;
}

/**
 *
 * @param bufferType
 * @param size
 * @param usage
 * @param data
 * @return
 */
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data /*= nullptr*/)
{
    GLuint object;
    glGenBuffers(1, &object);

    glBindBuffer(bufferType, object);

    glBufferData(bufferType, size, data, usage);

    glBindBuffer(bufferType, 0);

    return object;
}

// 程序纹理
GLuint CreateProcedureTexture(int size)
{
    unsigned char *imageData = new unsigned char[size * size * 4];

    float halfSize = (float)size * 0.5f;
    float maxDistance = sqrtf(halfSize * halfSize + halfSize * halfSize);
    float centerX = halfSize, centerY = halfSize;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int currentPixelOffset = (x + y * size) * 4;
            imageData[currentPixelOffset + 0] = 255;    //RGB都是255——白色
            imageData[currentPixelOffset + 1] = 255;
            imageData[currentPixelOffset + 2] = 255;

            //下面通过坐标点与中心点之间的距离来计算a通道值——透明度
            float deltaX = (float)x - centerX;
            float deltaY = (float)y - centerY;
            float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
            float alpha = powf(1.0f - (distance / maxDistance), 8.0f);
            alpha = (alpha > 1.0f) ? 1.0f : alpha;
            imageData[currentPixelOffset + 3] = (unsigned char)(alpha * 255);
        }
    }

    GLuint texture = CreateTexture2D(imageData, size, size, GL_RGBA);
    delete[] imageData;

    return texture;
}


void print_mat(glm::mat4 &mat)
{
    LOGI("mat:\n");

    float* data = glm::value_ptr(mat);
    for (int i = 0; i < 4; ++i) {// 0
        LOGI("%f\t%f\t%f\t%f\n", data[i+0],data[i+4],data[i+8],data[i+12]);
        //  LOGI("%f\t%f\t%f\t%f\n", mat[i].x,mat[i].x,mat[i].x,mat[i].x);
    }
}

void print_array(float array[16])
{
    LOGI("array:\n");
    float* data = array;
    for (int i = 0; i < 4; ++i) {// 0
        LOGI("%f\t%f\t%f\t%f\n", data[i+0],data[i+4],data[i+8],data[i+12]);
        //  LOGI("%f\t%f\t%f\t%f\n", mat[i].x,mat[i].x,mat[i].x,mat[i].x);
    }
}

