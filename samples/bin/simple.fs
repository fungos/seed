#version 120

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mTexture;
uniform float fElapsedTime;

varying vec2 vOutUV;
varying lowp vec4 vOutColor;

void main()
{
	vec4 color = texture2D(mTexture, vOutUV);
	float factor = max(sin(fElapsedTime * 3), 0.1f);
	color.rgb = color.rgb * color.a * factor; // basic blending, need simulate fixed pipeline here
	gl_FragColor = color;
}
