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

#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include "Defines.h"
#include "Enum.h"
#include "Reader.h"
#include "Writer.h"
#include "MathUtil.h"

namespace Seed {

/// Drawable Interface
/**
Interface for renderables objects. Things that will be rendered to the screen.
*/
class SEED_CORE_API IRenderable
{
	SEED_DISABLE_COPY(IRenderable)

	public:
		IRenderable();
		virtual ~IRenderable();

		virtual void Render(const Matrix4f &worldTransform) = 0;
		virtual void Update(f32 delta) = 0;

		virtual void SetColor(u32 r, u32 g, u32 b, u32 a);
		virtual void SetColor(f32 r, f32 g, f32 b, f32 a);
		virtual void SetColor(Color px);
		virtual Color GetColor() const;

		virtual void SetBlending(eBlendMode op);
		virtual String GetBlendingName() const;
		virtual void SetBlendingByName(const String &blending);
		virtual void SetVisible(bool b);
		bool IsVisible() const;

		virtual void Unserialize(Reader &reader);
		virtual void Serialize(Writer &writer);

	protected:
		eBlendMode	nBlendOperation;
		Color		cColor;
		bool		bColorChanged;
		bool		bVisible;

		virtual void Reset();
};

} // namespace

#endif // __IRENDERABLE_H__
