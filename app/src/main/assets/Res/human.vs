attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;
attribute vec4 normal;
attribute float meshInfoId;

//--------------------------------------------
// Skeleton Mesh Info
uniform int boneCounts[100000];
uniform ivec4 boneIdsArray[100000];
uniform vec4 boneWeightArray[100000];

uniform int boneIndexArray[10];
uniform mat4 boneWorldModelMatrixArray[10];
uniform mat4 boneOffsetMatrixArray[10];
//--------------------------------------------

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 thresholdPoint;

varying vec4 tmp_Color;

vec4 getFinalPosition()
{
    vec4 originPos = position;
    mat4 worldMat;
    mat4 offsetMat;
    mat4 combineMat;
    float weight = 0.0;
    vec4 vc;
    vec4 finalPos;

    for(int j = 0; j < boneIndexArray.length(); ++j)
    {
        for(int i = 0; i < boneCounts[int(meshInfoId)]; ++i)
        {
            if(boneIdsArray[int(meshInfoId)][i] == boneIndexArray[j])
            {
                //combineMat =  boneWorldModelMatrixArray[j] * boneOffsetMatrixArray[j];
                //weight = boneWeightArray[int(meshInfoId)][i];

                vc = boneWeightArray[int(meshInfoId)][i] * boneWorldModelMatrixArray[j] * boneOffsetMatrixArray[j] * originPos;
                //vc.w = 1.0;
                finalPos = finalPos + vc;
            }
        }
    }

    return finalPos;
}

void main()
{
    vec4 tmpPos = position;
    vec4 finalPos;

    finalPos = getFinalPosition();

    tmp_Color = finalPos;

    vec4 origin = vec4(0.0, 0.0, 0.0, 0.0);
    //--------------------------------------
    //if(0.9 == color.x)
    //if(32 == boneIndexArray[4])
    //{
    //    tmp_Color = vec4(0.6, 0.1, 0.9, 1.0);
    //}
    //else
    //{
    //    tmp_Color = vec4(0.1, 0.1, 0.9, 1.0);
    //}

    //gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;
}