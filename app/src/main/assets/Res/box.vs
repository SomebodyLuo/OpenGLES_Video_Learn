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
    vec4 vc1, vc2;
    vec4 finalPos;

    finalPos = boneWeightArray[0] * boneWorldModelMatrixArray[int(boneIdsArray[0])] * boneOffsetMatrixArray[int(boneIdsArray[0])] * originPos;

    return finalPos;
}

void main()
{

    vec4 finalPos;

    V_Color = vec4(0.8, 0.2, 0.9, 1.0);

    finalPos = getFinalPosition();


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
