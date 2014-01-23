#version 120

attribute vec3 vPosition;
attribute lowp vec4 vColor;
attribute vec2 vUV;

varying vec2 vOutUV;
varying lowp vec4 vOutColor;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition, 1.0f);
	vOutUV = vUV;//vec2(gl_MultiTexCoord0);
	vOutColor = vColor;
}
