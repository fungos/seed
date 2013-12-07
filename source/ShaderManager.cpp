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

#include "ShaderManager.h"
#include "Log.h"
#include "Shader.h"
#include "ShaderProgram.h"

#define TAG	"[ShaderManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ShaderManager)

ShaderManager::ShaderManager()
	: mShaderPrograms()
	, mShaders()
	, bEnabled(true)
{
}

ShaderManager::~ShaderManager()
{
	this->Reset();
}

u32 ShaderManager::GetProgramId(const String &shaderProgramName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderProgramName);

	if (it == mShaderPrograms.end())
	{
		Log(TAG "The shader program '%s' was not found", shaderProgramName.c_str());
		return 0;
	}
	else
	{
		IShaderProgram *shaderProgram = (*it).second;
		return shaderProgram->GetID();
	}
}

void ShaderManager::AttachShader(const String &shaderName, IShader *shader)
{
	mShaders[shaderName] = shader;
}

void ShaderManager::LoadShaderSource(const String &shaderName, const String &filename, ResourceManager *res)
{
	ShaderMapIterator it = mShaders.find(shaderName);

	if (it != mShaders.end())
	{
		IShader *shader = (*it).second;
		shader->Load(filename, res);
	}
}

void ShaderManager::CompileShader(const String &shaderName)
{
	ShaderMapIterator it = mShaders.find(shaderName);

	if (it != mShaders.end())
	{
		IShader *shader = (*it).second;
		shader->Compile();
	}
}

void ShaderManager::BindAttribute(const String &shaderProgramName, const u32 index, const String &attribName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderProgramName);

	if (it != mShaderPrograms.end())
	{
		IShaderProgram *shaderProgram = (*it).second;
		shaderProgram->BindAttribute(index, attribName);
	}
}


bool ShaderManager::LinkShaderProgram(const String &shaderProgramName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderProgramName);

	if (it != mShaderPrograms.end())
	{
		IShaderProgram *shaderProgram = (*it).second;
		shaderProgram->Link();
	}

	return true; // check linkaged
}

void ShaderManager::AttachShaderToProgram(const String &shaderProgramName, const String &shaderName)
{
	ShaderMapIterator itShader = mShaders.find(shaderName);
	ShaderProgramMapIterator itShaderProgram = mShaderPrograms.find(shaderProgramName);

	if (itShader != mShaders.end() && itShaderProgram != mShaderPrograms.end())
	{
		IShader *shader = (*itShader).second;
		IShaderProgram *shaderProgram = (*itShaderProgram).second;

		shaderProgram->AttachShader(shader);

		// TODO Verify if it will be necessary
		//shader->IncrementReference();
	}
}

void ShaderManager::Use(const String &shaderProgramName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderProgramName);

	if (it != mShaderPrograms.end())
	{
		IShaderProgram *shaderProgram = (*it).second;
		pCurrentProgram = shaderProgram;

		pCurrentProgram->Use();
	}
}

IShaderProgram* ShaderManager::GetShaderProgram(const String &shaderProgramName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderProgramName);

	if (it == mShaderPrograms.end())
	{
		Log(TAG "The shader program '%s' was not found", shaderProgramName.c_str());
		return nullptr;
	}

	return (*it).second;
}

void ShaderManager::Add(const String &shaderProgramName, IShaderProgram *shaderProgram)
{
	if (mShaderPrograms.find(shaderProgramName) != mShaderPrograms.end())
	{
		Log(TAG "The shader program '%s' is already added.", shaderProgramName.c_str());
		return;
	}

	mShaderPrograms[shaderProgramName] = shaderProgram;
}

void ShaderManager::Remove(const String &shaderPogramName)
{
	ShaderProgramMapIterator it = mShaderPrograms.find(shaderPogramName);

	if (it == mShaderPrograms.end())
	{
		Log(TAG "The shader program '%s' was not found", shaderPogramName.c_str());
		return;
	}

	mShaderPrograms.erase(it);
}

// IModule

bool ShaderManager::Initialize()
{
	IManager::Initialize();
	return true;
}

bool ShaderManager::Reset()
{
	return true;
}

bool ShaderManager::Shutdown()
{
	return IManager::Shutdown();
}

void ShaderManager::Disable()
{
	bEnabled = false;
}

void ShaderManager::Enable()
{
	bEnabled = true;
}

} // namespace
