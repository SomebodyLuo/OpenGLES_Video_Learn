attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;

attribute vec4 boneNum;
attribute vec4 boneIdArray;
attribute vec4 boneWeightArray;

attribute mat4 boneWorldMatrix0;
attribute mat4 boneWorldMatrix1;
attribute mat4 boneWorldMatrix2;
attribute mat4 boneWorldMatrix3;

attribute mat4 boneOffsetMatrix0;
attribute mat4 boneOffsetMatrix1;
attribute mat4 boneOffsetMatrix2;
attribute mat4 boneOffsetMatrix3;

//--------------------------------------------

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 VertexMoveMatrix;
uniform vec4 U_BoneInfo0;
uniform vec4 U_BoneInfo1;
uniform vec4 U_BoneInfo2;
uniform vec4 U_BoneInfo3;

varying vec4 V_Color;
varying vec3 V_Normal;
varying vec3 V_WorldPos;

void main()
{
    vec4 finalPostion;
    for(int i = 0 ;i < boneNum.x;i++)
    {
        mat4 combineMat;
        float weight;
        if(0 == i)
        {
            combineMat = boneWorldMatrix0 * boneOffsetMatrix0;
            weight = boneWeightArray.x;
        }
        else if(1 == i)
        {
            combineMat = boneWorldMatrix1 * boneOffsetMatrix1;
            weight = boneWeightArray.y;
        }
        else if(2 == i)
        {
            combineMat = boneWorldMatrix2 * boneOffsetMatrix2;
            weight = boneWeightArray.z;
        }
        else if(3 == i)
        {
            combineMat = boneWorldMatrix3 * boneOffsetMatrix3;
            weight = boneWeightArray.w;
        }

        vec4 vc = combineMat * position;
        vc = vc * weight;
        vc.w = 1;

        finalPostion = finalPostion + vc;
    }

    V_Color = vec4(boneWeightArray.xyz, 1.0);

    V_Normal = normal.xyz;
    V_WorldPos = (ModelMatrix * position).xyz;
    gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * finalPostion;
}

//{
//    //do the vertex blending,get the vertex's pos in world space
//    glm::vec4 vAfterVc = glm::vec4();
//    for(int i=0; i < mBoneInfo[vertexIndex].m_boneNum; ++i)
//    {
//        //
//        glm::mat4 mat =  glm::mul(mBoneInfo[vertexIndex].m_bones[i]->mWorldMatrix, mBoneInfo[vertexIndex].m_bones[i]->m_boneOffset.mOffsetMatrix);
//        LOGE("luoyouren1----------\n");
//        print_mat(mat);
//        LOGE("luoyouren2----------\n");
//        VertexData vdIndex = mVertexes[vertexIndex];
//        glm::vec4 vec41= glm::vec4(vdIndex.Position[0],vdIndex.Position[1],vdIndex.Position[2],vdIndex.Position[3]);
//        glm::vec4 newVector  = glm::mul(mat,vec41);
//
//        newVector = newVector * mBoneInfo[vertexIndex].m_boneWeights[i];
//        newVector.w = 1;
//
//        vAfterVc += newVector ;
//    }
//    afterVertexBuffer->SetPosition(vertexIndex, vAfterVc.x, vAfterVc.y, vAfterVc.z );
//}