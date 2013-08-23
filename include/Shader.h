/*
* Copyright (c) 2012, Seed Developers
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. A credit line is required in the game credits, as the following line:
*    Powered by Seed Framework
* 5. A Seed Framework logo should be displayed within a splash screen, or in
*    the case where the game haven't or cannot have a splash, a logo should be
*    displayed somewhere in the game (eg. menu, credits, other place).
*
* THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __SHADER_H__
#define __SHADER_H__

/*
GLSL Version	OpenGL Version
	1.10.59		2.0
	1.20.8		2.1
	1.30.10		3.0
	1.40.08		3.1
	1.50.11		3.2
	3.30.6		3.3
	4.00.9		4.0
	4.10.6		4.1
	4.20.11		4.2
	4.30.8		4.3
	4.40		4.4
*/

#if defined(BUILD_IOS)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/glsl/glslES120Shader.h"
	#else
	#include "api/glsl/glslES100Shader.h"
	#endif

#elif defined(BUILD_SDL) || defined(BUILD_GLFW)
	#if defined(SEED_ENABLE_OGLES2)
	#include "api/glsl/glslES120Shader.h"
	#else
	#include "api/glsl/glslES100Shader.h"
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

#endif // __SHADER_H__
