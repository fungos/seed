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

namespace Seed {

Keyframe::Keyframe()
	: sName("")
	, fRotation(0.0f)
	, ptPos(0.0f, 0.0f)
	, ptPivot(0.0f, 0.0f)
	, ptScale(1.0f, 1.0f)
	, iEvent(KeyframeEventNone)
	, iFrameToJump(-1)
	, fEasing(0.0f)
	, iColorR(255)
	, iColorG(255)
	, iColorB(255)
	, iColorA(255)
	, bTween(false)
	, bBlank(false)
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
	ASSERT_NULL(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("name", "keyframe");
		fRotation = reader.ReadF32("rotation", 0.0f);
		fEasing = reader.ReadF32("easing", 0.0f);

		iFrame = reader.ReadS32("frame", -1);
		ASSERT_MSG(iFrame != -1, "Keyframe without a frame number");

		iFrameToJump = reader.ReadS32("goto", -1);

		bTween = reader.ReadBool("tweeb", false);
		bBlank = reader.ReadBool("blank", false);

		String event(reader.ReadString("event", "none"));
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

		if (reader.SelectNode("position"))
		{
			ptPos.x = reader.ReadF32("x", 0.0f);
			ptPos.y = reader.ReadF32("y", 0.0f);
			reader.Unselect();
		}

		if (reader.SelectNode("pivot"))
		{
			ptPivot.x = reader.ReadF32("x", 0.0f);
			ptPivot.y = reader.ReadF32("y", 0.0f);
			reader.Unselect();
		}

		if (reader.SelectNode("scale"))
		{
			ptScale.x = reader.ReadF32("x", 1.0f);
			ptScale.y = reader.ReadF32("y", 1.0f);
			reader.Unselect();
		}

		if (reader.SelectNode("color"))
		{
			iColorR = reader.ReadS32("r", 255);
			iColorG = reader.ReadS32("g", 255);
			iColorB = reader.ReadS32("b", 255);
			iColorA = reader.ReadS32("a", 255);
			reader.Unselect();
		}

		ret = true;
	}

	return ret;
}

} // namespace
