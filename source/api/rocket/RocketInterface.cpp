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

#include "Defines.h"

#if SEED_USE_ROCKET_GUI == 1

#include "Timer.h"
#include "File.h"
#include "ResourceManager.h"
#include "RendererDevice.h"
#include "Screen.h"
#include "Texture.h"
#include "contrib/soil/SOIL.h"

#include "api/rocket/RocketInterface.h"

#include <Rocket/Core/Factory.h>

#define TAG "[API::Rocket] "

namespace Seed { namespace RocketGui {

RocketInterface::RocketInterface()
	: pCurrent(NULL)
	, iModifierState(0)
{
	cVertexBuffer.Configure(BufferUsageEveryFrameChange);
	cElementBuffer.Configure(BufferUsageEveryFrameChange, ElementTypeInt);
	this->SetWidth(pScreen->GetWidth());
	this->SetHeight(pScreen->GetHeight());

	RocketEventInstancer *inst = new RocketEventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(inst);
	inst->RemoveReference();
}

RocketInterface::~RocketInterface()
{
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);
	pRendererDevice->DestroyHardwareBuffer(&cElementBuffer);
	pCurrent = NULL;
}

// Rocket::Core::RenderInterface
void RocketInterface::RenderGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f &translation)
{
	Matrix4f transform = Matrix4f::identity();
	Vector3f t(translation.x + this->GetHorizontalTexelOffset(), translation.y + this->GetVerticalTexelOffset(), 0.0f);
	transform.setTranslation(t);

	sVertex *vert = NewArray(sVertex, num_vertices);
	for (int i = 0; i < num_vertices; ++i)
	{
		vert[i].cVertex.setX(vertices[i].position.x);
		vert[i].cVertex.setY(vertices[i].position.y);
		vert[i].cVertex.setZ(0);

		vert[i].cColor.r = vertices[i].colour.red;
		vert[i].cColor.g = vertices[i].colour.green;
		vert[i].cColor.b = vertices[i].colour.blue;
		vert[i].cColor.a = vertices[i].colour.alpha;

		vert[i].cCoords.x = vertices[i].tex_coord[0];
		vert[i].cCoords.y = vertices[i].tex_coord[1];
	}
	cElementBuffer.SetData(indices, num_indices);
	cVertexBuffer.SetData(vert, num_vertices);

	RendererPacket packet;
	packet.pElementBuffer = &cElementBuffer;
	packet.pVertexBuffer = &cVertexBuffer;
	packet.pTransform = &transform;
	packet.nMeshType = Seed::Triangles;
	packet.nBlendMode = Seed::BlendModulate;
	TexturePtr *tex = (TexturePtr *)texture;
	if (tex)
		packet.pTexture = tex->pTex;

	pRendererDevice->UploadData(&packet);

	DeleteArray(vert);
}

Rocket::Core::CompiledGeometryHandle RocketInterface::CompileGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture)
{
	RendererPacket *packet = New(RendererPacket());

	int *elems = (int *)Alloc(sizeof(int) * num_indices);
	memcpy(elems, indices, num_indices * sizeof(int));

	sVertex *vert = NewArray(sVertex, num_vertices);
	for (int i = 0; i < num_vertices; ++i)
	{
		vert[i].cVertex.setX(vertices[i].position.x);
		vert[i].cVertex.setY(vertices[i].position.y);
		vert[i].cVertex.setZ(0);

		vert[i].cColor.r = vertices[i].colour.red;
		vert[i].cColor.g = vertices[i].colour.green;
		vert[i].cColor.b = vertices[i].colour.blue;
		vert[i].cColor.a = vertices[i].colour.alpha;

		vert[i].cCoords.x = vertices[i].tex_coord[0];
		vert[i].cCoords.y = vertices[i].tex_coord[1];
	}

	packet->pElementBuffer = New(ElementBuffer());
	packet->pElementBuffer->Configure(BufferUsageNeverChange, ElementTypeInt);
	packet->pElementBuffer->SetData(elems, num_indices);

	packet->pVertexBuffer = New(VertexBuffer());
	packet->pVertexBuffer->Configure(BufferUsageNeverChange);
	packet->pVertexBuffer->SetData(vert, num_vertices);

	TexturePtr *tex = (TexturePtr *)texture;
	if (tex)
		packet->pTexture = tex->pTex;
	packet->nMeshType = Seed::Triangles;
	packet->nBlendMode = Seed::BlendModulate;

	return (Rocket::Core::CompiledGeometryHandle)packet;
}

void RocketInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f &translation)
{
	RendererPacket *packet = (RendererPacket *)geometry;

	Matrix4f transform = Matrix4f::identity();
	Vector3f t(translation.x + this->GetHorizontalTexelOffset(), translation.y + this->GetVerticalTexelOffset(), 0.0f);
	transform.setTranslation(t);
	packet->pTransform = &transform;

	pRendererDevice->UploadData(packet);
}

void RocketInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	RendererPacket *packet = (RendererPacket *)geometry;

	void *vert = NULL;
	packet->pVertexBuffer->GetData(&vert);

	void *elems = NULL;
	packet->pElementBuffer->GetData(&elems);

	// inverse order freeing
	pRendererDevice->DestroyHardwareBuffer(packet->pVertexBuffer);
	Delete(packet->pVertexBuffer);
	pRendererDevice->DestroyHardwareBuffer(packet->pElementBuffer);
	Delete(packet->pElementBuffer);

	sVertex *v = (sVertex *)vert;
	DeleteArray(v);

	int *e = (int *)elems;
	Free(e);

	Delete(packet);
}

void RocketInterface::EnableScissorRegion(bool enable)
{
	pRendererDevice->EnableScissor(enable);
}

void RocketInterface::SetScissorRegion(int x, int y, int width, int height)
{
	pRendererDevice->SetScissor(x, pScreen->GetHeight() - (y + height), width, height);
}

bool RocketInterface::LoadTexture(Rocket::Core::TextureHandle &texture_handle, Rocket::Core::Vector2i &texture_dimensions, const Rocket::Core::String &source)
{
	Texture *t = static_cast<Texture *>(pResourceManager->Get(source.CString(), Seed::TypeTexture));
	t->SetFilter(Seed::TextureFilterTypeMin, Seed::TextureFilterLinear);
	t->SetFilter(Seed::TextureFilterTypeMag, Seed::TextureFilterLinear);
	texture_dimensions.x = t->GetWidth();
	texture_dimensions.y = t->GetHeight();

	TexturePtr *tex = New(TexturePtr());
	tex->pTex = t;
	tex->bDynamic = false;

	texture_handle = (Rocket::Core::TextureHandle)tex;

	return true;
}

bool RocketInterface::GenerateTexture(Rocket::Core::TextureHandle &texture_handle, const Rocket::Core::byte *source, const Rocket::Core::Vector2i &source_dimensions)
{
	Texture *t = New(Texture());
	u32 w = source_dimensions.x;
	u32 h = source_dimensions.y;
	t->Load("[RocketDynamicTexture]", w, h, (Color *)source, w, h, true);
	t->SetFilter(Seed::TextureFilterTypeMin, Seed::TextureFilterLinear);
	t->SetFilter(Seed::TextureFilterTypeMag, Seed::TextureFilterLinear);

	TexturePtr *tex = New(TexturePtr());
	tex->pTex = t;
	tex->bDynamic = true;
	texture_handle = (Rocket::Core::TextureHandle)tex;

	return true;
}

void RocketInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	TexturePtr *tex = (TexturePtr *)texture_handle;
	if (tex->bDynamic)
	{
		Delete(tex->pTex);
	}
	else
	{
		sRelease(tex->pTex);
	}

	Delete(tex);
}

float RocketInterface::GetHorizontalTexelOffset()
{
	return pRendererDevice->GetHorizontalTexelOffset();
}

float RocketInterface::GetVerticalTexelOffset()
{
	return pRendererDevice->GetVerticalTexelOffset();
}

// Rocket::Core::FileInterface
Rocket::Core::FileHandle RocketInterface::Open(const Rocket::Core::String &path)
{
	FilePtr *fp = New(FilePtr());
	#warning TODO - Move to async file loading
	fp->pFile = New(File(path.CString()));
	fp->pFile->GetData();
	fp->iOffset = 0L;

	return (Rocket::Core::FileHandle)fp;
}

void RocketInterface::Close(Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	Delete(fp->pFile);
	Delete(fp);
}

size_t RocketInterface::Read(void *buffer, size_t size, Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	void *ptr = &(fp->pFile->GetData())[fp->iOffset];

	size_t rlen = (fp->iOffset + size) > fp->pFile->GetSize() ? (fp->pFile->GetSize() - fp->iOffset) : size;
	fp->iOffset += rlen;
	memcpy(buffer, ptr, rlen);

	return rlen;
}

bool RocketInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
	FilePtr *fp = (FilePtr *)file;

	long cur = 0;
	if (origin == SEEK_CUR)
		cur = fp->iOffset;
	else if (origin == SEEK_END)
		cur = fp->pFile->GetSize();

	fp->iOffset = cur + offset;

	return (fp->iOffset <= (long)fp->pFile->GetSize()); // when seek will fail for libRocket?
}

size_t RocketInterface::Tell(Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	return fp->iOffset;
}

// Rocket::Core::SystemInterface
float RocketInterface::GetElapsedTime()
{
	return static_cast<float>(pTimer->GetMilliseconds() / 1000.0f);
}

bool RocketInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
	UNUSED(type);
	Log(TAG "%s", message.CString());

	return true;
}

void RocketInterface::OnInputPointerPress(const EventInputPointer *ev)
{
	u32 btn = 0;

	switch (ev->GetPressed())
	{
		case ButtonLeft: break;
		case ButtonRight: btn = 1; break;
		case ButtonMiddle: btn = 2; break;
		case ButtonUp: pCurrent->ProcessMouseWheel(-1, iModifierState); return;
		case ButtonDown: pCurrent->ProcessMouseWheel(1, iModifierState); return;
		default: break;
	}

	pCurrent->ProcessMouseButtonDown(btn, iModifierState);
}

void RocketInterface::OnInputPointerRelease(const EventInputPointer *ev)
{
	u32 btn = 0;

	switch (ev->GetPressed())
	{
		case ButtonLeft: break;
		case ButtonRight: btn = 1; break;
		case ButtonMiddle: btn = 2; break;
		default: break;
	}

	pCurrent->ProcessMouseButtonUp(btn, iModifierState);
}

void RocketInterface::OnInputPointerMove(const EventInputPointer *ev)
{
	pCurrent->ProcessMouseMove(ev->GetX(), ev->GetY(), iModifierState);
}

void RocketInterface::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Rocket::Core::Input::KeyIdentifier key = (Rocket::Core::Input::KeyIdentifier)ev->GetKey().GetValue();
	pCurrent->ProcessKeyDown(key, iModifierState);

	// FIXME: Do full key->text processing
	pCurrent->ProcessTextInput(key);
}

void RocketInterface::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Rocket::Core::Input::KeyIdentifier key = (Rocket::Core::Input::KeyIdentifier)ev->GetKey().GetValue();
	pCurrent->ProcessKeyUp(key, iModifierState);
}

void RocketInterface::Render(const Matrix4f &worldTransform)
{
	UNUSED(worldTransform)
	if (pCurrent)
		pCurrent->Render();
}

void RocketInterface::Update(f32 delta)
{
	UNUSED(delta)
	if (pCurrent)
		pCurrent->Update();
}

const String RocketInterface::GetClassName() const
{
	return "RocketInterface";
}

int RocketInterface::GetObjectType() const
{
	return TypeRocketInterface;
}

void RocketInterface::SetCurrentContext(Rocket::Core::Context *ctx)
{
	pCurrent = ctx;
}



RocketEventInstancer::RocketEventInstancer()
{
}

RocketEventInstancer::~RocketEventInstancer()
{
}

Rocket::Core::EventListener *RocketEventInstancer::InstanceEventListener(const Rocket::Core::String &value, Rocket::Core::Element *element)
{
	UNUSED(element)
	return new RocketEvent(value);
}

void RocketEventInstancer::Release()
{
	delete this;
}

DECLARE_CONTAINER_TYPE(Vector, IRocketEventListener)
IRocketEventListenerVector RocketEventManager::vListeners;

void RocketEventManager::AddListener(IRocketEventListener *listener)
{
	vListeners += listener;
}

void RocketEventManager::RemoveListener(IRocketEventListener *listener)
{
	vListeners -= listener;
}

void RocketEventManager::SendEvent(Rocket::Core::Event &event, const Rocket::Core::String &value)
{
	IRocketEventListenerVectorIterator it = vListeners.begin();
	IRocketEventListenerVectorIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IRocketEventListener *obj = (*it);
		obj->OnGuiEvent(event, value);
	}
}


RocketEvent::RocketEvent(const Rocket::Core::String &value)
	: sValue(value)
{
}

RocketEvent::~RocketEvent()
{
}

void RocketEvent::ProcessEvent(Rocket::Core::Event &event)
{
	RocketEventManager::SendEvent(event, sValue);
}

void RocketEvent::OnDetach(Rocket::Core::Element *element)
{
	UNUSED(element)
	delete this;
}

}} // namespace

#endif // SEED_USE_ROCKET_GUI
