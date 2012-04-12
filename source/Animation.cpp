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
#include "Animation.h"
#include "Log.h"
#include "Enum.h"
#include "Frame.h"

#define TAG		"[Animation] "

namespace Seed {

Animation::Animation()
	: vFrames()
	, sName()
	, iIndex(0)
	, iFrames(0)
	, iAnimationId(0)
	, bAnimated(true)
	, bLoop(true)
{
}

Animation::~Animation()
{
	this->Unload();
}

bool Animation::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	if (this->Unload())
	{
		sName = reader.ReadString("name", "animation");
		bAnimated = reader.ReadBool("animated", true);
		bLoop = reader.ReadBool("loop", true);
		iFps = reader.ReadU32("fps", 60);
		iFrames = reader.SelectArray("frames");

		if (iFrames)
		{
			for (u32 i = 0; i < iFrames; i++)
			{
				Frame *f = New(Frame);
				reader.SelectNext();
				f->Load(reader, res);
				f->iIndex = i;

				if (!f->fFrameRate)
					f->fFrameRate = 1.0f / static_cast<f32>(iFps);

				vFrames += f;
			}
			reader.UnselectArray();
		}
	}

	return true;
}

bool Animation::Write(Writer &writer)
{
	bool ret = false;

	writer.OpenNode();
		writer.WriteString("type", this->GetObjectName().c_str());
		writer.WriteString("name", sName.c_str());
		writer.WriteBool("animated", bAnimated);
		writer.WriteU32("fps", iFps);

		writer.OpenArray("frames");
		for (u32 i = 0; i < iFrames; i++)
		{
			Frame *f = vFrames[i];
			f->Write(writer);
		}
		writer.CloseArray();
	writer.CloseNode();

	return ret;
}

bool Animation::Unload()
{
	FrameVectorIterator it = vFrames.begin();
	FrameVectorIterator end = vFrames.end();
	for (; it != end; ++it)
		Delete(*it);

	FrameVector().swap(vFrames);

	return true;
}

FrameVector *Animation::GetFrames()
{
	return &vFrames;
}

const String Animation::GetObjectName() const
{
	return "Animation";
}

int Animation::GetObjectType() const
{
	return Seed::ObjectAnimation;
}

} // namespace
