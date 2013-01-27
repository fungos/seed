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

#include "map/IMetadataObject.h"
#include "Screen.h"
#include "RendererDevice.h"
#include "MathUtil.h"

namespace Seed {

IMetadataObject::IMetadataObject()
	: mProperties()
	, pVertices(NULL)
	, pCached(NULL)
	, iVertices(0)
	, ptOffset(0.0f, 0.0f)
	, nType(kMetaTypeRect)
	, rBox()
{
}

IMetadataObject::~IMetadataObject()
{
	this->Unload();
}

bool IMetadataObject::Load(Reader &reader, ResourceManager *res)
{
	UNUSED(res)
	bool ret = false;

	if (this->Unload())
	{
		f32 height = reader.ReadF32("height", 0.0f);
		f32 width = reader.ReadF32("width", 0.0f);
		f32 x = reader.ReadF32("x", 0.0f);
		f32 y = reader.ReadF32("y", 0.0f);
		sName = reader.ReadString("name", "IMetadataObject");
//		String type = reader.ReadString("type", "");
		this->SetVisible(reader.ReadBool("visible", false));
		this->ReadProperties(reader);

		u32 len = reader.SelectArray("polygon");
		if (len)
		{
			nType = kMetaTypePolygon;
			this->ReadVertices(reader, len);
			reader.UnselectArray();
		}
		else
		{
			len = reader.SelectArray("polyline");
			if (len)
			{
				nType = kMetaTypePolyline;
				this->ReadVertices(reader, len);
				reader.UnselectArray();
			}
			else // rect
			{
				sFree(pCached);
				sFree(pVertices);
				pVertices = (f32 *)Alloc(sizeof(f32) * 8);
				pCached = (f32 *)Alloc(sizeof(f32) * 8);

				pVertices[0] = 0.0f;
				pVertices[1] = 0.0f;
				pVertices[2] = width;
				pVertices[3] = 0.0f;
				pVertices[4] = width;
				pVertices[5] = height;
				pVertices[6] = 0.0f;
				pVertices[7] = height;
				iVertices = 4;

				rBox = Rect4f(0.0f, 0.0f, width, height);
			}
		}

		this->SetPosition(x, y);
		this->SetWidth(rBox.Width());
		this->SetHeight(rBox.Height());

		ret = true;
	}

	return ret;
}

bool IMetadataObject::Write(Writer &writer)
{
	UNUSED(writer)
	//#pragma warning "IMPL - IMetadataObject::Write(Writer &writer)"
	return true;
}

bool IMetadataObject::Unload()
{
	iVertices = 0;
	sFree(pCached);
	sFree(pVertices);
	Map<String, String>().swap(mProperties);

	return true;
}

void IMetadataObject::Render(const Matrix4f &worldTransform)
{
	Vector3f t = worldTransform.getTranslation();
	f32 x = this->GetX();
	f32 y = this->GetY();
	f32 w = this->GetWidth();
	f32 h = this->GetHeight();

	switch (nType)
	{
		case kMetaTypeRect:
		{
			x = x + t.getX();
			y = y + t.getY();
			pRendererDevice->DrawRect(x, y, w + x, h + y, Color(255, 0, 255, 255));
		}
		break;

		case kMetaTypePolygon:
		case kMetaTypePolyline:
		{
			for (u32 i = 0; i < iVertices * 2; i += 2)
			{
				pCached[i + 0] = pVertices[i + 0] + x + t.getX();
				pCached[i + 1] = pVertices[i + 1] + y + t.getY();
			}
			pRendererDevice->DrawLines(pCached, iVertices, Color(255, 0, 255, 255));

			x = x + t.getX() + ptOffset.x;
			y = y + t.getY() + ptOffset.y;
			pRendererDevice->DrawRect(x, y, w + x, h + y, Color(255, 0, 255, 255));
		}
		break;

		default:
		break;
	}
}

void IMetadataObject::ReadVertices(Reader &reader, u32 size)
{
	if (!size)
		return;

	sFree(pCached);
	sFree(pVertices);
	iVertices = size;
	pVertices = (f32 *)Alloc(sizeof(f32) * size * 2); // POD
	pCached = (f32 *)Alloc(sizeof(f32) * size * 2);

	f32 x = 0.0f;
	f32 y = 0.0f;
	f32 minX = 999999.f;
	f32 minY = 999999.f;
	f32 maxX = -999999.f;
	f32 maxY = -999999.f;

	for (u32 i = 0, p = 0; i < size; i++)
	{
		reader.SelectNext();

		x = reader.ReadF32("x", 0.0f);
		y = reader.ReadF32("y", 0.0f);

		minX = std::min(x, minX);
		minY = std::min(y, minY);
		maxX = std::max(x, maxX);
		maxY = std::max(y, maxY);

		pVertices[p++] = x;
		pVertices[p++] = y;
	}

	ptOffset.x = minX;
	ptOffset.y = minY;
	rBox = Rect4f(minX, minY, maxX - minX, maxY - minY);
}

void IMetadataObject::ReadProperties(Reader &reader)
{
	if (reader.SelectNode("properties"))
	{
		u32 k = 0;
		while (1)
		{
			const char *key = reader.GetKey(k++);
			if (!key)
				break;

			mProperties[key] = reader.ReadString(key, "");
		}
		reader.UnselectNode();
	}
}

const f32 *IMetadataObject::GetVertices() const
{
	return pVertices;
}

const String IMetadataObject::GetProperty(const String &property) const
{
	Map<String, String>::const_iterator it = mProperties.find(property);
	return it == mProperties.end() ? "" : it->second;
}

bool IMetadataObject::CheckHit(const Rect4f &area, Rect4f &overlap) const
{
	//#warning FIXME - Implementar em coordenadas de mundo
	return area.GetOverlappedRect(rBox, overlap);
}

} // namespace
