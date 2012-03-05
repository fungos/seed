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
#include "MathUtil.h"
#include "Point.h"

namespace Seed {

class ITexture;
class ITransformable;

/// Vertex Format
struct sVertex
{
	// To be compatible with DX, we use FVF. So we must respect the DX FVF ordering in this struct.
	Vector3f	cVertex;
	uPixel		iColor;
	Point2f		cCoords;

	sVertex()
		: cVertex(0.0f, 0.0f, 0.0f)
		, iColor(255, 255, 255, 255)
		, cCoords(0.0f, 0.0f)
	{
	}

	sVertex &operator=(const sVertex &v)
	{
		cVertex = v.cVertex;
		iColor = v.iColor;
		cCoords = v.cCoords;
		return *this;
	}
};

/// Renderer Packet
struct RendererPacket
{
	Matrix4f				*pTransform;
	ITexture				*pTexture;
	void					*pVertexData;
	u32						iSize;
	uPixel					iColor;
	eMeshType				nMeshType;
	eBlendMode				nBlendMode;

	RendererPacket()
		: pTransform(NULL)
		, pTexture(NULL)
		, pVertexData(NULL)
		, iSize(0)
		, iColor(0, 0, 0, 255)
		, nMeshType(Seed::TriangleStrip)
		, nBlendMode(Seed::BlendNone)
	{
	}
};

} // namespace

#endif // __VERTEX_H__
