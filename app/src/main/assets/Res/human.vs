attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;
attribute vec4 normal;
attribute float boneCounts;
attribute vec4 boneIdsArray;
attribute vec4 boneWeightArray;

//--------------------------------------------
// Skeleton Info
uniform int boneIndexArray[100];
uniform mat4 boneWorldModelMatrixArray[100];
uniform mat4 boneOffsetMatrixArray[100];
//--------------------------------------------

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 thresholdPoint;
uniform mat4 IT_ModelMatrix;

varying vec4 V_Color;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_Texcoord;

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
        if(int(boneCounts) > 0)
        {
            for(int i = 0; i < int(boneCounts); ++i)
            {
                if((int(boneIdsArray[i])) == boneIndexArray[j])
                {
                    //combineMat =  boneWorldModelMatrixArray[j] * boneOffsetMatrixArray[j];
                    //weight = boneWeightArray[int(meshInfoId)][i];

                    vc = boneWeightArray[i] * boneWorldModelMatrixArray[j] * boneOffsetMatrixArray[j] * originPos;

                    finalPos = finalPos + vc;
                }
            }
        }
        else
        {
            return originPos;
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

    V_Color = color;
    V_Normal = IT_ModelMatrix * normal;
    //V_Normal = ModelMatrix * normal;
    V_WorldPos = ModelMatrix * finalPos;
    V_Texcoord = texcoord;
    //gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * finalPos;
}