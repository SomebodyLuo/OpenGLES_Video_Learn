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

    V_Color = vec4(boneWeightArray.xyz, 1.0);
    //V_Color = vec4(boneWorldMatrix0[0].xyz, 1.0);

    V_Normal = normal.xyz;
    V_WorldPos = (ModelMatrix * position).xyz;
    gl_PointSize = 8.0;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;
}
