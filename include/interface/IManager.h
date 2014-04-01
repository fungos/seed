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

#ifndef __IMANAGER_H__
#define __IMANAGER_H__

#include "Defines.h"
#include "interface/IObject.h"

#define SEED_DECLARE_MANAGER(name)	public:										\
										virtual const String GetName() const	\
										{										\
											return #name;						\
										}										\


namespace Seed {

/// Module Interface
/**
Interface for basic manager
*/
class SEED_CORE_API IManager
{
	SEED_DISABLE_COPY(IManager)

	public:
		IManager();
		virtual ~IManager();

		/// Initialize this module, it must initialize all module attributes.
		virtual bool Initialize();

		/// Reset all module attributes as a newly created object.
		virtual bool Reset();

		/// Terminate everything and deinitialize all dependency. Reset to before initialization state.
		virtual bool Shutdown();

		/// Disables this module
		virtual void Disable();

		/// Enabled this module
		virtual void Enable();

		/// Returns true if this module is enabled
		virtual bool IsEnabled() const;

		/// Check if the module is initialized
		bool IsInitialized() const;

		/// If this module is mandatory (it is a base subsystem or critical one)
		virtual bool IsRequired() const;

		virtual const String GetName() const = 0;

	protected:
		bool bInitialized : 1;
		bool bEnabled : 1;
};

} // namespace

#endif // __IMANAGER_H__
