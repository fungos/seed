#version 120

uniform float fElapsedTime;

attribute vec3 vPosition;
attribute lowp vec4 vColor;
attribute vec2 vUV;

varying vec2 vOutUV;
varying lowp vec4 vOutColor;

void main()
{
	vec2 coord = vec2(gl_MultiTexCoord0);
	vOutUV = coord * (1 - min(cos(fElapsedTime / 2), 0.5f));
	vOutColor = vColor;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition, 1.0f);
}
