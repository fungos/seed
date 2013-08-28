#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#if defined(BUILD_IOS)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/glsl/glslES120ShaderProgram.h"
	#else
	#include "api/glsl/glslES100ShaderProgram.h"
	#endif

#elif defined(BUILD_SDL) || defined(BUILD_GLFW)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/glsl/glslES120ShaderProgram.h"
	#else
	#include "api/glsl/glslES100ShaderProgram.h"
	#endif

	#if defined(SEED_ENABLE_OGL20)
	#include "api/glsl/glsl110Shader.h"
	#endif

	#if defined(SEED_ENABLE_OGL30)
	#include "api/glsl/glsl130Shader.h"
	#endif

	#if defined(SEED_ENABLE_OGL40)
	#include "api/glsl/glsl400Shader.h"
	#endif

	#if defined(SEED_ENABLE_D3D8)
	#endif

	#if defined(SEED_ENABLE_D3D9)
	#endif

	#if defined(SEED_ENABLE_D3D10)
	#endif

	#if defined(SEED_ENABLE_D3D11)
	#endif

#elif defined(BUILD_QT)
	#include "api/glsl/glslES100Shader.h"
#endif

#endif // __SHADERPROGRAM_H__
