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

#include "Shader.h"

#if defined (BUILD_IOS) && !defined(SEED_ENABLE_OGLES2)

#include <OpenGLES/ES1/gl.h>

#define TAG "[Shader] "

namespace Seed { namespace OpenGL {

OGLES1Shader::OGLES1Shader(eShaderType type)
{
	iShaderType = type;
	bLoaded = false;
	bCompiled = false;
	switch (iShaderType)
	{
		case ShaderTypeVertex: { iShaderHandle = glCreateShader(GL_VERTEX_SHADER); break; }
		case ShaderTypeFragment : { iShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); break; }
		case ShaderTypeGeometry : { iShaderHandle = 0; SEED_ASSERT_MSG(iShaderHandle, "ERROR: Geometry shader not suported yet."); break; }
		case ShaderTypeTesselation : { iShaderHandle = 0; SEED_ASSERT_MSG(iShaderHandle, "ERROR: Tesselation shader not suported yet."); break; }
	}
}

OGLES1Shader::~OGLES1Shader()
{
}

bool OGLES1Shader::Load(const String &filename, ResourceManager *res)
{
	if(IShader::Load(filename, res))
	{
		const char* shaderData = (const char*)pFile->GetData();
		glShaderSource(iShaderHandle, 1, &shaderData, NULL);
		bLoaded = true;
	}
	else
		Log(TAG "ERROR: Could not find/load shader %s.", filename.c_str());

	return bLoaded;
}

void OGLES1Shader::Compile()
{
	if(bLoaded)
	{
		glCompileShader(iShaderHandle);
		bCompiled = true;
	}
}

u32 OGLES1Shader::GetShaderHandle() const
{
	return iShaderHandle;
}

}} // namespace

#endif // BUILD_IOS && !SEED_ENABLE_OGLES2
