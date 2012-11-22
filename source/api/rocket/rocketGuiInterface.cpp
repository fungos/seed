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

#include "api/rocket/rocketGuiInterface.h"

#define TAG "[API::Rocket] "

namespace Seed { namespace RocketGui {

GuiInterface::GuiInterface()
{
}

GuiInterface::~GuiInterface()
{
}

// Rocket::Core::RenderInterface
void GuiInterface::RenderGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f &translation)
{
	UNUSED(vertices)
	UNUSED(num_vertices)
	UNUSED(indices)
	UNUSED(num_indices)
	UNUSED(texture)
	UNUSED(translation)
}

Rocket::Core::CompiledGeometryHandle GuiInterface::CompileGeometry(Rocket::Core::Vertex *vertices, int num_vertices, int *indices, int num_indices, Rocket::Core::TextureHandle texture)
{
	UNUSED(vertices)
	UNUSED(num_vertices)
	UNUSED(indices)
	UNUSED(num_indices)
	UNUSED(texture)
	return (Rocket::Core::CompiledGeometryHandle)NULL;
}

void GuiInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f &translation)
{
	UNUSED(geometry)
	UNUSED(translation)
}

void GuiInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	UNUSED(geometry)
}

void GuiInterface::EnableScissorRegion(bool enable)
{
	pRendererDevice->EnableScissor(enable);
}

void GuiInterface::SetScissorRegion(int x, int y, int width, int height)
{
	pRendererDevice->SetScissor(x, pScreen->GetHeight() - (y + height), width, height);
}

bool GuiInterface::LoadTexture(Rocket::Core::TextureHandle &texture_handle, Rocket::Core::Vector2i &texture_dimensions, const Rocket::Core::String &source)
{
	Texture *t = static_cast<Texture *>(pResourceManager->Get(source.CString()));

	texture_dimensions.x = t->GetWidth();
	texture_dimensions.y = t->GetHeight();
	pRendererDevice->TextureRequest(t);

	texture_handle = (Rocket::Core::TextureHandle)t;

	return true;
}

bool GuiInterface::GenerateTexture(Rocket::Core::TextureHandle &texture_handle, const Rocket::Core::byte *source, const Rocket::Core::Vector2i &source_dimensions)
{
	Texture *t = New(Texture());
	u32 w = source_dimensions.x;
	u32 h = source_dimensions.y;
	t->Load(w, h, (Color *)source, w, h);
	pRendererDevice->TextureRequest(t);

	texture_handle = (Rocket::Core::TextureHandle)t;

	return true;
}

void GuiInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	Texture *t = (Texture *)texture_handle;
	pRendererDevice->TextureRequestAbort(t);
	pRendererDevice->TextureUnload(t);

	Delete(t);
}

float GuiInterface::GetHorizontalTexelOffset()
{
	return pRendererDevice->GetHorizontalTexelOffset();
}

float GuiInterface::GetVerticalTexelOffset()
{
	return pRendererDevice->GetVerticalTexelOffset();
}

// Rocket::Core::FileInterface
Rocket::Core::FileHandle GuiInterface::Open(const Rocket::Core::String &path)
{
	FilePtr *fp = New(FilePtr());
	fp->pFile = New(File(path.CString()));
	fp->pFile->GetData();
	fp->iOffset = 0L;

	return (Rocket::Core::FileHandle)fp;
}

void GuiInterface::Close(Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	Delete(fp->pFile);
	Delete(fp);
}

size_t GuiInterface::Read(void *buffer, size_t size, Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	void *ptr = &(fp->pFile->GetData())[fp->iOffset];
	memcpy(buffer, ptr, size);

	return size;
}

bool GuiInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
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

size_t GuiInterface::Tell(Rocket::Core::FileHandle file)
{
	FilePtr *fp = (FilePtr *)file;
	return fp->iOffset;
}

// Rocket::Core::SystemInterface
float GuiInterface::GetElapsedTime()
{
	return static_cast<float>(pTimer->GetMilliseconds() / 1000.0f);
}

}} // namespace

#endif // SEED_USE_ROCKET_GUI
