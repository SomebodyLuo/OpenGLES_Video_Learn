#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_PointColor;

varying vec4 V_Color;



void main()
{
    gl_FragColor = V_Color; //vec4(0.1, 0.9, 0.0, 1.0);
}