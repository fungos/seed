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

#ifndef __ROCKET_GUI_INTERFACE_H__
#define __ROCKET_GUI_INTERFACE_H__

#include "Defines.h"

#if SEED_USE_ROCKET_GUI == 1

#include "RendererDevice.h"
#include "File.h"
#include "Texture.h"
#include "interface/ISceneObject.h"
#include "interface/IEventInputPointerListener.h"
#include "interface/IEventInputKeyboardListener.h"
#include <Rocket/Core/RenderInterface.h>
#include <Rocket/Core/FileInterface.h>
#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Context.h>
#include <Rocket/Core/EventListenerInstancer.h>
#include <Rocket/Core/EventListener.h>

namespace Seed { namespace RocketGui {

struct FilePtr
{
	File *pFile;
	long iOffset;
};

struct TexturePtr
{
	ITexture *pTex;
	bool bDynamic;
};

class SEED_CORE_API RocketInterface :
					public Rocket::Core::RenderInterface,
					public Rocket::Core::FileInterface,
					public Rocket::Core::SystemInterface,
					public IEventInputPointerListener,
					public IEventInputKeyboardListener,
					public ISceneObject
{
	public:
		RocketInterface();
		virtual ~RocketInterface();

		virtual void SetCurrentContext(Rocket::Core::Context *ctx);

		// Rocket::Core::RenderInterface
		virtual void RenderGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f &translation);
		virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture);
		virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f &translation);
		virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);
		virtual void EnableScissorRegion(bool enable);
		virtual void SetScissorRegion(int x, int y, int width, int height);
		virtual bool LoadTexture(Rocket::Core::TextureHandle &texture_handle, Rocket::Core::Vector2i &texture_dimensions, const Rocket::Core::String &source);
		virtual bool GenerateTexture(Rocket::Core::TextureHandle &texture_handle, const Rocket::Core::byte *source, const Rocket::Core::Vector2i &source_dimensions);
		virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);
		virtual float GetHorizontalTexelOffset();
		virtual float GetVerticalTexelOffset();

		// Rocket::Core::FileInterface
		virtual Rocket::Core::FileHandle Open(const Rocket::Core::String &path);
		virtual void Close(Rocket::Core::FileHandle file);
		virtual size_t Read(void *buffer, size_t size, Rocket::Core::FileHandle file);
		virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);
		virtual size_t Tell(Rocket::Core::FileHandle file);

		// Rocket::Core::SystemInterface
		virtual float GetElapsedTime();
		virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String &message);

		// IEventInputPointerListener
		virtual void OnInputPointerPress(const EventInputPointer *ev) override;
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;
		virtual void OnInputPointerMove(const EventInputPointer *ev) override;

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev) override;
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IDataObject
		virtual bool Load(Reader &, ResourceManager *) override { return true; }
		virtual bool Write(Writer &) override { return true; }
		virtual bool Unload() override { return true; }

		// IRenderable
		virtual void Render(const Matrix4f &worldTransform) override;
		virtual void Update(f32 delta) override;

		// IObject
		virtual const String GetClassName() const override;
		virtual int GetObjectType() const override;

	private:
		SEED_DISABLE_COPY(RocketInterface);

		VertexBuffer cVertexBuffer;
		ElementBuffer cElementBuffer;
		Rocket::Core::Context *pCurrent;
		u32 iModifierState;
};

class SEED_CORE_API RocketEventInstancer : public Rocket::Core::EventListenerInstancer
{
	public:
		RocketEventInstancer();
		virtual ~RocketEventInstancer();

		/// Instances a new event
		virtual Rocket::Core::EventListener *InstanceEventListener(const Rocket::Core::String &value, Rocket::Core::Element *element);

		/// Destroys the instancer.
		virtual void Release();
};

class SEED_CORE_API IRocketEventListener
{
	public:
		IRocketEventListener() {}
		virtual ~IRocketEventListener() {}

		virtual void OnGuiEvent(Rocket::Core::Event &event, const Rocket::Core::String &script) = 0;
};

class SEED_CORE_API RocketEventManager
{
	DECLARE_CONTAINER_TYPE(Vector, IRocketEventListener)
	public:
		static void AddListener(IRocketEventListener *listener);
		static void RemoveListener(IRocketEventListener *listener);
		static void SendEvent(Rocket::Core::Event &event, const Rocket::Core::String &value);

	private:
		static IRocketEventListenerVector vListeners;
};

class SEED_CORE_API RocketEvent : public Rocket::Core::EventListener
{
	public:
		RocketEvent(const Rocket::Core::String &value);
		virtual ~RocketEvent();

		/// Sends the event value through our event processing system.
		virtual void ProcessEvent(Rocket::Core::Event &event);

		/// Destroys the event.
		virtual void OnDetach(Rocket::Core::Element *element);

	private:
		Rocket::Core::String sValue;
};

}} // namespace

#endif // SEED_USE_ROCKET_GUI

#endif // __ROCKET_GUI_INTERFACE_H__
