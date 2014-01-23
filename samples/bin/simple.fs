#version 120

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mTexture;
varying vec2 vOutUV;
varying lowp vec4 vOutColor;

void main()
{
	vec4 color = texture2D(mTexture, vOutUV);
	color.x = 1.0f;
	gl_FragColor = vOutColor;
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
