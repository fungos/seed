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

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Defines.h"
#include "Enum.h"
#include "MathUtil.h"
#include "Point.h"

namespace Seed {

class ITexture;
struct VertexBuffer;
struct ElementBuffer;

/// Vertex Format
struct sVertex
{
	// To be compatible with DX, we use FVF. So we must respect the DX FVF ordering in this struct.
	Vector3f	cVertex;
	Color4b		cColor;
	Point2f		cCoords;

	sVertex()
		: cVertex(0.0f, 0.0f, 0.0f)
		, cColor(255, 255, 255, 255)
		, cCoords(0.0f, 0.0f)
	{
	}

	sVertex &operator=(const sVertex &v)
	{
		cVertex = v.cVertex;
		cColor = v.cColor;
		cCoords = v.cCoords;
		return *this;
	}
};

enum ePacketFlags
{
	FlagNone		= 0x00,
	FlagWireframe	= 0x01
};

/// Renderer Packet
struct RendererPacket
{
	const Matrix4f			*pTransform;
	ITexture				*pTexture;
	VertexBuffer			*pVertexBuffer;
	ElementBuffer			*pElementBuffer;
	Color					cColor;
	eMeshType				nMeshType;
	eBlendMode				nBlendMode;
	ePacketFlags			iFlags;
	Vector3f				vPivot;
	f32						fRadius;

	RendererPacket()
		: pTransform(NULL)
		, pTexture(NULL)
		, pVertexBuffer(NULL)
		, pElementBuffer(NULL)
		, cColor(0, 0, 0, 255)
		, nMeshType(Seed::TriangleStrip)
		, nBlendMode(Seed::BlendNone)
		, iFlags(FlagNone)
		, vPivot(0.5f, 0.5f, 0.5f)
		, fRadius(0)
	{
	}
};

} // namespace

#endif // __VERTEX_H__
