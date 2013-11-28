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
#include "Memory.h"

#define TAG		"[Animation] "

namespace Seed {

Animation::Animation()
	: pRes(nullptr)
	, vFrames()
	, sName()
	, iFps(0)
	, iIndex(0)
	, iFrames(0)
	, iAnimationId(0)
	, bAnimated(false)
	, bLoop(false)
{
}

Animation::~Animation()
{
	this->Unload();
}

void Animation::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());
	bAnimated = reader.ReadBool("bAnimated", bAnimated);
	bLoop = reader.ReadBool("bLoop", bLoop);
	iFps = reader.ReadU32("iFps", iFps);

	iFrames = reader.SelectArray("aFrames");
	if (iFrames)
	{
		for (u32 i = 0; i < iFrames; i++)
		{
			auto f = sdNew(Frame);
			reader.SelectNext();
			f->Load(reader, pRes);
			f->iIndex = i;

			if (!f->fFrameRate)
				f->fFrameRate = 1.0f / static_cast<f32>(iFps);

			vFrames += f;
		}
		reader.UnselectArray();
	}
	SEED_WARNING(vFrames.Size() == 0, "Animation '%s': has no frames [aFrames].", sName.c_str());
}

bool Animation::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	if (!this->Unload())
		return false;

	pRes = res;
	this->Set(reader);

	return true;
}

bool Animation::Write(Writer &writer)
{
	auto ret = false;

	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());
		writer.WriteBool("bAnimated", bAnimated);
		writer.WriteU32("iFps", iFps);

		writer.OpenArray("aFrames");
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
	for (auto frame: vFrames)
		sdDelete(frame);

	FrameVector().swap(vFrames);
	sName = this->GetTypeName();

	return true;
}

Animation *Animation::Clone() const
{
	auto obj = sdNew(Animation);

	obj->GenerateCloneName(sName);
	obj->iFps = iFps;
	obj->iIndex = iIndex;
	obj->iFrames = iFrames;
	obj->iAnimationId = iAnimationId;
	obj->bAnimated = bAnimated;
	obj->bLoop = bLoop;

	for (auto frame: vFrames)
		obj->vFrames += frame->Clone();

	return obj;
}

FrameVector *Animation::GetFrames()
{
	return &vFrames;
}

} // namespace
