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

#if defined(SEED_ENABLE_OGLES2)

namespace Seed { namespace GLSL {

GLSLES120Shader::GLSLES120Shader()
{
}

GLSLES120Shader::~GLSLES120Shader()
{
}

void GLSLES120Shader::Load(String name) const
{
	if(m_source !=0)
	{
		std::cerr<<"deleting existing source code\n";
		delete m_source;
	}
	std::ifstream shaderSource(_name.c_str());
	if (!shaderSource.is_open())
	{
		std::cerr<<"File not found "<<_name.c_str()<<"\n";
		exit(EXIT_FAILURE);
	}

	m_source = new std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
	shaderSource.close();
	*m_source+="\0";

	const char* data=m_source->c_str();
	glShaderSource(m_shaderHandle , 1, &data,NULL);
	m_compiled=false;

	if (m_debugState == true)
	{
		std::cerr<<"Shader Loaded and source attached\n";
		printInfoLog(m_shaderHandle);
	}
}

void GLSLES120Shader::Compile() const
{

}

u32 GLSLES120Shader::GetShaderHandle() const
{

}

void GLSLES120Shader::ToggleDebug() const
{

}

}} // namespace

#endif // SEED_ENABLE_OGLES2
