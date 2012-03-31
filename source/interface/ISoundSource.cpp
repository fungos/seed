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

#include "interface/ISoundSource.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "Timer.h"

#define TAG		"[ISoundSource] "

namespace Seed {

ISoundSource::ISoundSource()
	: cPosition()
	, cVelocity()
	, cOrientation()
	, fVolume(1.0f)
	, fFadeTime(0.0f)
	, fStartFadeTime(0)
	, eState(SourceNone)
	, bLoop(false)
{
}

ISoundSource::~ISoundSource()
{
}

void ISoundSource::Load(const String &filename, ResourceManager *res)
{
	UNUSED(filename);
	UNUSED(res);
	SEED_ABSTRACT_METHOD;
}

void ISoundSource::Unload()
{
	eState = SourceNone;
}

void ISoundSource::SetPosition(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	cPosition = v;
}

void ISoundSource::SetPosition(Vector3f vec)
{
	cPosition = vec;
}

void ISoundSource::GetPosition(Vector3f *vec) const
{
	*vec = cPosition;
}

void ISoundSource::SetVelocity(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	cVelocity = v;
}

void ISoundSource::SetVelocity(Vector3f vec)
{
	cVelocity = vec;
}

void ISoundSource::GetVelocity(Vector3f *vec) const
{
	*vec = cVelocity;
}

void ISoundSource::SetOrientation(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	cOrientation = v;
}

void ISoundSource::SetOrientation(Vector3f vec)
{
	cOrientation = vec;
}

void ISoundSource::GetOrientation(Vector3f *vec) const
{
	*vec = cOrientation;
}

void ISoundSource::SetVolume(f32 vol)
{
	SEED_ASSERT_MSG((vol >= 0 || vol <= 1.0f), "Source volume must be between 0 and 1");
	fVolume = vol;
}

f32 ISoundSource::GetVolume() const
{
	return fVolume;
}

void ISoundSource::UpdateVolume()
{
	SEED_ABSTRACT_METHOD;
}

bool ISoundSource::IsPlaying() const
{
	return ((eState != SourceStopped) &&
			(eState != SourceStop) &&
			(eState != SourcePause) &&
			(eState != SourcePaused) &&
			(eState != SourceNone));
}

void ISoundSource::Play()
{
	if (eState != SourcePlaying && eState != SourcePlayStarted)
		eState = SourcePlay;
}

void ISoundSource::Stop(f32 ms)
{
	UNUSED(ms);
	eState = SourceStop;
}

void ISoundSource::Pause()
{
	if (this->IsPlaying())
	{
		eState = SourcePause;
	}
}

void ISoundSource::Resume()
{
	if (eState == SourcePause || eState == SourcePaused)
	{
		eState = SourcePlay;
	}
}

void ISoundSource::FadeOut(f32 ms)
{
	if (eState != SourceFadingOut)
		eState = SourceFadeOut;

	fStartFadeTime = pTimer->GetMilliseconds();
	fFadeTime = ms;
}

void ISoundSource::FadeIn(f32 ms)
{
	if (eState != SourceFadingIn)
		eState = SourceFadeIn;

	fStartFadeTime = pTimer->GetMilliseconds();
	fFadeTime = ms;
}

void ISoundSource::SetLoop(bool b)
{
	bLoop = b;
}

bool ISoundSource::IsLoop() const
{
	return bLoop;
}

eSoundSourceState ISoundSource::GetState() const
{
	return eState;
}

const char *ISoundSource::GetObjectName() const
{
	return "ISoundSource";
}

int ISoundSource::GetObjectType() const
{
	return Seed::ObjectSoundSource;
}

} // namespace
