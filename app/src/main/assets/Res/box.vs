attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute float boneCounts;
attribute vec4 boneIdsArray;
attribute vec4 boneWeightArray;

//--------------------------------------------
// Skeleton Info
uniform int boneIndexArray[100];
uniform mat4 boneWorldTranslateMatrixArray[100];
uniform mat4 boneWorldRotationMatrixArray[100];
uniform mat4 boneWorldModelMatrixArray[100];
uniform mat4 boneOffsetMatrixArray[100];
//--------------------------------------------

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec4 V_Color;

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

                    //vc = boneWeightArray[i] * boneWorldModelMatrixArray[j] * boneOffsetMatrixArray[j] * originPos;
                    vc = boneWorldRotationMatrixArray[j] * originPos * boneWorldTranslateMatrixArray[j] * boneWeightArray[i];

                    finalPos = finalPos + vc;
                }
            }
            V_Color = vec4(0.6, 0.1, 0.9, 1.0);
        }
        else
        {
            V_Color = vec4(0.8, 0.1, 0.4, 1.0);
            return originPos;
        }
    }

    return finalPos;
}

void main()
{

    vec4 finalPos;

    finalPos = getFinalPosition();

    //V_Color = vec4(finalPos.xyz, 1.0);

    vec4 origin = vec4(0.0, 0.0, 0.0, 0.0);
    //--------------------------------------
    //if(0.9 == color.x)
    //if(32 == boneIndexArray[4])
    //{
    //    V_Color = vec4(0.6, 0.1, 0.9, 1.0);
    //}
    //else
    //{
    //    V_Color = vec4(0.1, 0.1, 0.9, 1.0);
    //}

    //gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * finalPos;
}
