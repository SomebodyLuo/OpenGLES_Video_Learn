#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_LightPos;
uniform vec4 U_LightAmbient;
uniform vec4 U_AmbientMaterial;
uniform vec4 U_LightDiffuse;
uniform vec4 U_DiffuseMaterial;
uniform vec4 U_PointColor;

varying vec4 V_Color;
varying vec3 V_Normal;
varying vec3 V_WorldPos;



void main()
{
    gl_FragColor = V_Color;
}