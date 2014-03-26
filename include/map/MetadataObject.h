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

#ifndef __IMETADATAOBJECT_H__
#define __IMETADATAOBJECT_H__

#include "Defines.h"
#include "Rect.h"
#include "SceneNode.h"
#include "Point.h"

namespace Seed {

class SEED_CORE_API MetadataObject : public ISceneNode
{
	SEED_DISABLE_COPY(MetadataObject)

	public:
		MetadataObject();
		virtual ~MetadataObject();

		virtual const f32 *GetVertices() const;
		virtual const String GetProperty(const String &property) const;
		virtual void SetProperty(const String &key, const String &value = "");
		virtual bool CheckHit(const Rect4f &area, Rect4f &overlap) const;

		// IRenderable
		virtual void Render(const Matrix4f &worldTransform) override;

		// ITransformable
		virtual void SetHeight(f32 h) override;
		virtual void SetWidth(f32 w) override;

		// IDataObject
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;
		virtual MetadataObject *Clone() const override;
		virtual void Set(Reader &reader) override;

	protected:
		void ReadProperties(Reader &reader);
		void WriteProperties(Writer &writer);

		void ReadVertices(Reader &reader, u32 size);
		void WriteVertices(Writer &writer);

	private:
		enum class eMetaType {
			Rect,
			Polygon,
			Polyline
		};

		Map<String, String> mProperties;
		f32					*pVertices;
		f32					*pCached;
		u32					iVertices;
		Point2f				ptOffset;
		eMetaType			nType;
		Rect4f				rBox;
};

} // namespace

#endif // __IMETADATAOBJECT_H__
