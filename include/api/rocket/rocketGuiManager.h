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

#ifndef __ROCKET_GUI_MANAGER_H__
#define __ROCKET_GUI_MANAGER_H__

#include "Defines.h"

#if SEED_USE_ROCKET_GUI == 1

#include "Container.h"
#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "interface/IRenderable.h"
#include "Singleton.h"

#include <Rocket/Core/Context.h>

namespace Seed { namespace RocketGui {

typedef Vector<Rocket::Core::Context *> ContextVector;
typedef ContextVector::iterator ContextVectorIterator;
typedef ContextVector::const_iterator ConstContextVectorIterator;

class GuiInterface;

class SEED_CORE_API GuiManager : public IModule, public IRenderable
{
	SEED_SINGLETON_DECLARE(GuiManager)
	public:
		void AddContext(Rocket::Core::Context *context);
		void RemoveContext(Rocket::Core::Context *context);

		// IModule
		virtual bool Initialize();
		virtual bool Shutdown();

		// IRenderable
		virtual void Render(const Matrix4f &worldTransform);
		virtual void Update(f32 dt);

		// IObject
		virtual const String GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(GuiManager);

	private:
		GuiInterface *pInterface;
		ContextVector vContext;
};

#define pGuiManager Seed::RocketGui::GuiManager::GetInstance()

}} // namespace

#else // SEED_USE_ROCKET_GUI
	#error "Include 'GuiManager.h' instead 'api/rocket/rocketGuiManager.h' directly."
#endif // SEED_USE_ROCKET_GUI
#endif // __ROCKET_GUI_MANAGER_H__
