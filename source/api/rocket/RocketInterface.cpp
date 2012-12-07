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

#include "api/rocket/RocketInterface.h"

#define TAG "[API::Rocket] "

namespace Seed { namespace RocketGui {

RocketInterface::RocketInterface()
{
	cVertexBuffer.Configure(BufferUsageEveryFrameChange);
	cElementBuffer.Configure(BufferUsageEveryFrameChange, ElementTypeInt);
}

RocketInterface::~RocketInterface()
{
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);
	pRendererDevice->DestroyHardwareBuffer(&cElementBuffer);
}

// Rocket::Core::RenderInterface
void RocketInterface::RenderGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f &translation)
{
	Matrix4f transform = Matrix4f::identity();
	Vector3f t(translation.x, translation.y, 0.0f);
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
	packet.pTexture = (Texture *)texture;
	packet.pElementBuffer = &cElementBuffer;
	packet.pVertexBuffer = &cVertexBuffer;
	packet.pTransform = &transform;
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

	packet->pTexture = (Texture *)texture;

	return (Rocket::Core::CompiledGeometryHandle)packet;
}

void RocketInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f &translation)
{
	RendererPacket *packet = (RendererPacket *)geometry;

	Matrix4f transform = Matrix4f::identity();
	Vector3f t(translation.x, translation.y, 0.0f);
	transform.setTranslation(t);
	packet->pTransform = &transform;

	pRendererDevice->UploadData(packet);
}

void RocketInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	RendererPacket *packet = (RendererPacket *)geometry;

	sVertex *vert = NULL;
	packet->pVertexBuffer->GetData(vert);

	int *elems = NULL;
	packet->pElementBuffer->GetData(elems);

	// inverse order freeing
	pRendererDevice->DestroyHardwareBuffer(packet->pVertexBuffer);
	Delete(packet->pVertexBuffer);
	pRendererDevice->DestroyHardwareBuffer(packet->pElementBuffer);
	Delete(packet->pElementBuffer);

	DeleteArray(vert);
	Free(elems);

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

	texture_dimensions.x = t->GetWidth();
	texture_dimensions.y = t->GetHeight();
	pRendererDevice->TextureRequest(t);

	texture_handle = (Rocket::Core::TextureHandle)t;

	return true;
}

bool RocketInterface::GenerateTexture(Rocket::Core::TextureHandle &texture_handle, const Rocket::Core::byte *source, const Rocket::Core::Vector2i &source_dimensions)
{
	Texture *t = New(Texture());
	u32 w = source_dimensions.x;
	u32 h = source_dimensions.y;
	t->Load(w, h, (Color *)source, w, h);
	pRendererDevice->TextureRequest(t);

	texture_handle = (Rocket::Core::TextureHandle)t;

	return true;
}

void RocketInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	Texture *t = (Texture *)texture_handle;
	pRendererDevice->TextureRequestAbort(t);
	pRendererDevice->TextureUnload(t);

	Delete(t);
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
	#warning "Move to async file loading"
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

	size_t rlen = size > fp->pFile->GetSize() ? fp->pFile->GetSize() : size;
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

}} // namespace

#endif // SEED_USE_ROCKET_GUI
