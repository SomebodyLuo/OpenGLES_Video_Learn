attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;

//--------------------------------------------
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

//uniform mat4 VertexMoveMatrix;

uniform vec4 U_Bool;

varying vec4 V_Color;
varying vec3 V_Normal;
varying vec3 V_WorldPos;



void main()
{
    vec4 originPos = position;

    //--------------------------------------
    vec4 tempPos = position;

    vec4 finalPos;
    mat4 combineMat;
    float weight = 0.0;
    vec4 vc;

    //for(float i = 0.0; i < boneNum.x; i = i + 1.0)
    //{
    //    if(0.0 == i)
    //    {
    //        vc = boneWorldMatrix0 * boneOffsetMatrix0 * tempPos;
    //        weight = boneWeightArray.x;
    //    }
    //    else if(1.0 == i)
    //    {
    //        vc = boneWorldMatrix1 * boneOffsetMatrix1 * tempPos;
    //        weight = boneWeightArray.y;
    //    }
    //    else if(2.0 == i)
    //    {
    //        vc = boneWorldMatrix2 * boneOffsetMatrix2 * tempPos;
    //        weight = boneWeightArray.z;
    //    }
    //    //else if(3.0 == i)
    //    //{
    //    //    vc = boneWorldMatrix3 * boneOffsetMatrix3 * tempPos;
    //    //    weight = boneWeightArray.w;
    //    //}
//
    //    vc = vc * weight;
    //    vc.w = 1.0;
    //    finalPos = finalPos + vc;
    //}

    tempPos = tempPos;

    vec4 origin = vec4(0.0, 0.0, 0.0, 0.0);
    //--------------------------------------

    V_Color = vec4(0.0, 0.0, boneWorldMatrix0[3].w, 1.0);

    //V_Color = vec4(boneWorldMatrix0[0].xyz, 1.0);

    V_Normal = normal.xyz;
    V_WorldPos = (ModelMatrix * position).xyz;
    gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * tempPos;
}
