#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_PointColor;

varying vec4 V_Color;



void main()
{
    gl_FragColor = U_PointColor;
}