#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#if defined(BUILD_IOS)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/ogl/oglES2ShaderProgram.h"
	#endif

#elif defined(BUILD_SDL) || defined(BUILD_GLFW)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/ogl/oglES2ShaderProgram.h"
	#endif

	#if defined(SEED_ENABLE_OGL20)
	#include "api/ogl/ogl20ShaderProgram.h"
	#endif

	#if defined(SEED_ENABLE_OGL30)
	#include "api/ogl/ogl3ShaderProgram.h"
	#endif

	#if defined(SEED_ENABLE_OGL40)
	#include "api/ogl/ogl4ShaderProgram.h"
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
	#include "api/ogl/oglES1ShaderProgram.h"
#endif

#endif // __SHADERPROGRAM_H__
