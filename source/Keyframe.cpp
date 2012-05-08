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

#include "Keyframe.h"
#include "Enum.h"
#include <algorithm>

namespace Seed {

IDataObject *FactoryKeyframe()
{
	return New(Keyframe());
}

Keyframe::Keyframe()
	: ptPos(0.0f, 0.0f)
	, ptPivot(0.0f, 0.0f)
	, ptScale(1.0f, 1.0f)
	, fRotation(0.0f)
	, fEasing(0.0f)
	, iFrame(0)
	, iFrameToJump(-1)
	, iEvent(KeyframeEventNone)
	, sName("")
	, bTween(false)
	, bBlank(false)
	, iColorR(255)
	, iColorG(255)
	, iColorB(255)
	, iColorA(255)
{
}

Keyframe::~Keyframe()
{
	this->Unload();
}

bool Keyframe::Unload()
{
	return true;
}

bool Keyframe::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("sName", "keyframe");
		fRotation = reader.ReadF32("fRotation", 0.0f);
		fEasing = reader.ReadF32("fEasing", 0.0f);

		iFrame = reader.ReadS32("iFrame", -1);
		SEED_ASSERT_MSG(iFrame != -1, "Keyframe without a frame number");

		iFrameToJump = reader.ReadS32("iGoto", -1);

		bTween = reader.ReadBool("bTween", false);
		bBlank = reader.ReadBool("bBlank", false);

		String event(reader.ReadString("sEvent", "none"));
		std::transform(event.begin(), event.end(), event.begin(), tolower);
		if (event == "stop")
		{
			iEvent = KeyframeEventStop;
		}
		else if (event == "restart")
		{
			iEvent = KeyframeEventRestart;
		}
		else if (event == "goto")
		{
			iEvent = KeyframeEventJumpToFrame;
		}
		else
		{
			iEvent = KeyframeEventNone;
		}

		if (reader.SelectNode("cPosition"))
		{
			ptPos.x = reader.ReadF32("x", 0.0f);
			ptPos.y = reader.ReadF32("y", 0.0f);
			reader.UnselectNode();
		}

		if (reader.SelectNode("cPivot"))
		{
			ptPivot.x = reader.ReadF32("x", 0.0f);
			ptPivot.y = reader.ReadF32("y", 0.0f);
			reader.UnselectNode();
		}

		if (reader.SelectNode("cScale"))
		{
			ptScale.x = reader.ReadF32("x", 1.0f);
			ptScale.y = reader.ReadF32("y", 1.0f);
			reader.UnselectNode();
		}

		if (reader.SelectNode("cColor"))
		{
			iColorR = reader.ReadS32("r", 255);
			iColorG = reader.ReadS32("g", 255);
			iColorB = reader.ReadS32("b", 255);
			iColorA = reader.ReadS32("a", 255);
			reader.UnselectNode();
		}

		ret = true;
	}

	return ret;
}

bool Keyframe::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetObjectName().c_str());
		writer.WriteString("sName", sName.c_str());
		writer.WriteS32("iFrame", iFrame);
		writer.WriteS32("iGoto", iFrameToJump);
		writer.WriteF32("fRotation", fRotation);
		writer.WriteBool("bTween", bTween);
		writer.WriteBool("bBlank", bBlank);

		if (iEvent == KeyframeEventStop)
		{
			writer.WriteString("sEvent", "stop");
		}
		else if (iEvent == KeyframeEventRestart)
		{
			writer.WriteString("sEvent", "restart");
		}
		else if (iEvent == KeyframeEventJumpToFrame)
		{
			writer.WriteString("sEvent", "goto");
		}
		else
		{
			writer.WriteString("sEvent", "none");
		}

		writer.OpenNode("cPosition");
			writer.WriteF32("x", ptPos.x);
			writer.WriteF32("y", ptPos.y);
		writer.CloseNode();

		writer.OpenNode("cPivot");
			writer.WriteF32("x", ptPivot.x);
			writer.WriteF32("y", ptPivot.y);
		writer.CloseNode();

		writer.OpenNode("cScale");
			writer.WriteF32("x", ptScale.x);
			writer.WriteF32("y", ptScale.y);
		writer.CloseNode();

		writer.OpenNode("cColor");
			if (iColorR != 255) writer.WriteU32("r", iColorR);
			if (iColorG != 255) writer.WriteU32("g", iColorG);
			if (iColorB != 255) writer.WriteU32("b", iColorB);
			if (iColorA != 255) writer.WriteU32("a", iColorA);
		writer.CloseNode();
	writer.CloseNode();

	return true;
}

const String Keyframe::GetObjectName() const
{
	return "Keyframe";
}

int Keyframe::GetObjectType() const
{
	return Seed::TypeKeyframe;
}

} // namespace
