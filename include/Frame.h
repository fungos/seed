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

#ifndef __FRAME_H__
#define __FRAME_H__

#include "Defines.h"
#include "interface/IDataObject.h"

namespace Seed {

class ITexture;

/// Sprite Frame
/**
Frame is a static region from a Texture
*/
class SEED_CORE_API Frame : public IDataObject
{
	public:
		ITexture	*pTexture;
		String		sName;
		String		sTexture;
		u32			iFps;
		u32			iIndex;
		u32			iX;
		u32			iY;
		u32			iWidth;
		u32			iHeight;
		u32			iHalfWidth;
		u32			iHalfHeight;
		f32			fFrameRate;
		f32			fTexS0;
		f32			fTexS1;
		f32			fTexT0;
		f32			fTexT1;

		Frame();
		virtual ~Frame();

		// IDataObject
		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager);
		virtual bool Write(Writer &writer);
		virtual bool Unload();

		// IObject
		virtual const String GetObjectName() const;
		virtual int GetObjectType() const;

		// IMPLEMENT COPY
        SEED_DISABLE_COPY(Frame);
};

} // namespace

#endif // __FRAME_H__
