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
	: pSound(NULL)
	, fVolume(1.0f)
	, fFadeTime(0.0f)
	, fStartFadeTime(0)
	, eState(SourceNone)
	, bLoop(false)
	, bAutoPlay(false)
{
}

ISoundSource::~ISoundSource()
{
}

bool ISoundSource::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);
	bool ret = false;

	if (pSoundSystem->IsInitialized() && this->Unload())
	{
		sName = reader.ReadString("sName", "sound");
		String fname = reader.ReadString("sResource", "");

		fVolume = reader.ReadF32("fVolume", 1.0f);
		bLoop = reader.ReadBool("bLoop", false);
		bAutoPlay = reader.ReadBool("bAutoPlay", false);

		ITransformable::Unserialize(reader);
		IRenderable::Unserialize(reader);

		pSound = static_cast<Sound *>(res->Get(fname, Seed::TypeSound));
		ret = this->OnLoadFinished();

		pSoundSystem->Add(this);
		if (bAutoPlay)
			this->Play();
	}

	return ret;
}

bool ISoundSource::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetObjectName().c_str());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sResource", pSound->GetFilename().c_str());
		writer.WriteF32("fVolume", fVolume);
		writer.WriteBool("bLoop", bLoop);
		writer.WriteBool("bAutoPlay", bAutoPlay);

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);

		writer.CloseArray();
	writer.CloseNode();

	return true;
}

bool ISoundSource::Unload()
{
	bool ret = OnUnloadRequest();
	pSoundSystem->Remove(this);
	eState = SourceNone;
	return ret;
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

void ISoundSource::Render(const Matrix4f &worldTransform)
{
	UNUSED(worldTransform);
}

const String ISoundSource::GetObjectName() const
{
	return "SoundSource";
}

int ISoundSource::GetObjectType() const
{
	return Seed::TypeSoundSource;
}

} // namespace
