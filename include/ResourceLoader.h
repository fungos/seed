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

#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "interface/IModule.h"
#include "interface/IUpdatable.h"
#include "Mutex.h"
#include "Thread.h"
#include "Singleton.h"
#include "Container.h"

namespace Seed {

class ResourceGroup;
class EventResourceLoader;
class IEventResourceLoaderListener;

/// Resource Loader
/*!
Responsible for Loading resources
*/
class SEED_CORE_API ResourceLoader : public IModule, public IUpdatable, public Thread
{
	SEED_SINGLETON_DECLARE(ResourceLoader)

	public:
		typedef Vector<IEventResourceLoaderListener *> 	ListenerVector;
		typedef ListenerVector::iterator				ListenerIterator;

		typedef Vector<ResourceGroup *> 				GroupVector;
		typedef GroupVector::iterator					GroupIterator;

	public:
		//! Add a load request to the queue.
		/*!
			Add a load request to the queue.
		 */
		virtual void Add(ResourceGroup *group);

		//! Resource loader thread loop
		/*!
			Resource loader thread loop, if it returns false then it has finished.
		 */
		virtual bool Run(); // Thread

		/// Listeners pattern methods
		void AddListener(IEventResourceLoaderListener *listener);
		void RemoveListener(IEventResourceLoaderListener *listener);

		// IUpdatable
		virtual bool Update(f32 dt);

		// IModule
		virtual bool Initialize();
		virtual bool Shutdown();
		virtual bool Reset();

		// IObject
		virtual const String GetClassName() const;

	protected:
		void SendEventGroupLoaded(const EventResourceLoader *ev);
		void SendEventQueueEmpty(const EventResourceLoader *ev);

	private:
		SEED_DISABLE_COPY(ResourceLoader);

	private:
		ListenerVector 	vListeners;
		GroupVector		vGroups;
		bool 			bRunning;
		Mutex			*pMutex;
};

#define pResourceLoader ResourceLoader::GetInstance()

} // namespace

#endif // __RESOURCE_LOADER_H__
