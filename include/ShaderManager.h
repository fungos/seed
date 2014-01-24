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

#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#include "interface/IManager.h"
#include "Singleton.h"
#include "Container.h"

namespace Seed {

class IShader;
class IShaderProgram;

typedef Map<String, IShaderProgram*> ShaderProgramMap;
typedef ShaderProgramMap::iterator ShaderProgramMapIterator;

typedef Map<String, IShader*> ShaderMap;
typedef ShaderMap::iterator ShaderMapIterator;

class SEED_CORE_API ShaderManager : public IManager
{
	SEED_DECLARE_SINGLETON(ShaderManager)
	SEED_DECLARE_MANAGER(ShaderManager)
	SEED_DISABLE_COPY(ShaderManager)

	public:
		virtual u32 GetProgramId(const String &shaderProgramName);
		virtual void AttachShader(const String &shaderName, IShader *shader);
		virtual void LoadShaderSource(const String &shaderName, const String &filename, ResourceManager *res);
		virtual void CompileShader(const String &shaderName);
		virtual void BindAttribute(const String &shaderProgramName, u32 index, const String &attributeName);
		virtual void SetTexture(const String &shaderProgramName, u32 unit, const String &uniformName);
		virtual void SetUniform(const String &shaderProgramName, const String &uniformName, f32 value);
		virtual bool LinkShaderProgram(const String &shaderProgramName);
		virtual void AttachShaderToProgram(const String &shaderProgramName, const String &shaderName);
		virtual void Use(const String &shaderProgramName);
		virtual bool Validate(const String &shaderProgramName);
		IShaderProgram *GetShaderProgram(const String &shaderProgramName);

		void Add(const String &shaderProgramName, IShaderProgram *shaderProgram);
		void Remove(const String &shaderProgramName);

		// IModule
		virtual bool Initialize() override;
		virtual bool Reset() override;
		virtual bool Shutdown() override;

		virtual void Disable() override;
		virtual void Enable() override;

	private:
		ShaderProgramMap	mShaderPrograms;
		ShaderMap			mShaders;
		IShaderProgram		*pCurrentProgram;
		bool				bEnabled;
};

#define pShaderManager ShaderManager::GetInstance()

} // namespace

#endif // __SHADERMANAGER_H__
