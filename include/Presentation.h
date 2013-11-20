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

#ifndef __PRESENTATION_H__
#define __PRESENTATION_H__

#include "interface/IDataObject.h"
#include "ResourceManager.h"
#include "Container.h"

namespace Seed {

class Viewport;
class Camera;
class Renderer;
class SceneNode;
class SceneFileLoader;
class PrefabFileLoader;
class IEventPresentationListener;

SEED_DECLARE_CONTAINER(Vector, SceneNode)
SEED_DECLARE_CONTAINER(Vector, Viewport)
SEED_DECLARE_CONTAINER(Vector, Renderer)

class SEED_CORE_API Presentation : public IDataObject
{
	friend class RendererSceneLoader;
	/*!
	* \brief Callback
	* Callback to execute when the presentation load is finished. If the presentation load was aborted due some error,
	* it will call the callback with a renderer parameter with the faulty rebderer, otherwise the renderer will be null.
	* All renderers loaded in a presentation can be queried/get by name.
	*/
	typedef std::function<void(Presentation *, Renderer *)> Callback;
	SEED_DISABLE_COPY(Presentation)
	SEED_DECLARE_RTTI(Presentation, IDataObject)

	public:
		Presentation();
		virtual ~Presentation();

		bool Load(const String &filename, Callback cb, ResourceManager *res = pResourceManager);

		Renderer *GetRendererByName(const String &name);
		Viewport *GetViewportByName(const String &name);

		// IDataObject
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;

	protected:
		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager);

	private:
		void SceneLoaded(SceneFileLoader *ldr);
		void SceneAborted(SceneFileLoader *ldr);

		void PrefabLoaded(PrefabFileLoader *ldr);
		void PrefabAborted(PrefabFileLoader *ldr);

		void GotoScenePhase();
		void PrefabsPhase();
		void ScenesPhase();

		// IDataObject
		virtual Presentation *Clone() const override { SEED_ASSERT("[Presentation] Not clonable."); return nullptr; }
		virtual void Set(Reader &) override {}

	private:
		ResourceManager *pRes;
		bool *pFinishedScenes;
		bool *pFinishedPrefabs;
		u32 iPrefabsCount;

		Callback fnCallback;
		ViewportVector vViewport;
		RendererVector vRenderer;
		SceneNodeVector vScenes;
};


} // namespace

#endif // __PRESENTATION_H__
