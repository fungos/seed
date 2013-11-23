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

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "interface/ISceneObject.h"
#include "Vertex.h"
#include "RendererDevice.h"

namespace Seed {

class ITexture;

ISceneObject *FactoryImage();

/// Renderable Static Image
class SEED_CORE_API Image : public ISceneObject
{
	SEED_DISABLE_COPY(Image)
	SEED_DECLARE_RTTI(Image, ISceneObject)

	public:
		Image();
		Image(const String &filename, ResourceManager *res = pResourceManager);
		virtual ~Image();

		bool Load(ITexture *texture);
		bool Load(const String &filename, ResourceManager *res = pResourceManager);

		// IRenderable
		virtual void Update(f32 delta) override;
		virtual void Render(const Matrix4f &worldTransform) override;

		// IDataObject
		using ISceneObject::Load;
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;
		virtual Image *Clone() const override;
		virtual void Set(Reader &reader) override;

	private:
		void UpdateCoords();

	protected:
		ITexture		*pTexture;
		String			sFilename;

		// Frame related width and heigth used for rendering only
		s32 iHalfWidth; // half width in pixel
		s32 iHalfHeight; // half height in pixel
		u32 iWidth; // width in pixel
		u32 iHeight; // height in pixel
		VertexBuffer cVertexBuffer;
		sVertex vert[4];

		bool bDynamic : 1;
};

} // namespace

#endif // __IMAGE_H__
