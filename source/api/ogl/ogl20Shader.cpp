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

#if defined(SEED_ENABLE_OGL20)

#include "Memory.h"
#include "api/ogl/oglHeaders.h"
#include "Log.h"

#define TAG "[Shader] "

namespace Seed { namespace OpenGL {

OGL20Shader::OGL20Shader(eShaderType type) // Remove, lazy construction, ie. HardwareBuffer.
	: IShader()
{
	iShaderType = type;

	switch (iShaderType)
	{
		case eShaderType::Vertex: { iShaderHandle = glCreateShader(GL_VERTEX_SHADER); break; }
		case eShaderType::Pixel : { iShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); break; }
		default: { iShaderHandle = 0; SEED_ASSERT_MSG(iShaderHandle, "ERROR: Geometry shader not suported yet."); break; }
	}
}

OGL20Shader::~OGL20Shader()
{
}

bool OGL20Shader::Load(const String &filename, ResourceManager *res)
{
	if (IShader::Load(filename, res))
	{
		const char* shaderData = (const char*)pFile->GetData();
		glShaderSource(iShaderHandle, 1, &shaderData, NULL);
		bLoaded = true;
	}
	else
	{
		Log(TAG "ERROR: Could not find/load shader %s.", filename.c_str());
	}

	return bLoaded;
}

void OGL20Shader::Compile() const
{
	if (bLoaded)
	{
		glCompileShader(iShaderHandle);
		//bCompiled = true;

		GLint len;
		glGetShaderiv(iShaderHandle, GL_INFO_LOG_LENGTH, &len);
		if (len > 1)
		{
			GLchar *buff = (GLchar *)sdAlloc(len);
			glGetShaderInfoLog(iShaderHandle, len, &len, buff);
			Log("[Shader::Compile] %s: %s", sFilename.c_str(), buff);
			sdFree(buff);
		}
	}
}

u32 OGL20Shader::GetShaderHandle() const
{
	return iShaderHandle;
}

}} // namespace

#endif // SEED_ENABLE_OGL20
