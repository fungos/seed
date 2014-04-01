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
#include "System.h"

#define TAG		"[ISoundSource] "

namespace Seed {

ISoundSource::ISoundSource()
	: pSound(nullptr)
	, fVolume(1.0f)
	, fFadeTime(0.0f)
	, fStartFadeTime(0)
	, nState(eSoundSourceState::None)
	, bLoop(false)
	, bAutoPlay(false)
{
}

ISoundSource::~ISoundSource()
{
}

void ISoundSource::Set(Reader &reader)
{
	if (!pSoundSystem->IsInitialized())
		return;

	sName = reader.ReadString("sName", sName.c_str());
	auto resource = String("");
	if (pSound)
		resource = pSound->GetFilename();

	auto fname = String(reader.ReadString("sResource", resource.c_str()));

	fVolume = reader.ReadF32("fVolume", fVolume);
	bLoop = reader.ReadBool("bLoop", bLoop);
	bAutoPlay = reader.ReadBool("bAutoPlay", bAutoPlay);

	ITransformable::Unserialize(reader);
	IRenderable::Unserialize(reader);

	if (fname != resource)
	{
		pSound = static_cast<Sound *>(pRes->Get(fname, ISound::GetTypeId()));
		this->OnLoadFinished();
	}

	if (bAutoPlay)
		this->Play();
}

bool ISoundSource::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
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
	auto ret = OnUnloadRequest();
	sdRelease(pSound);
	pSoundSystem->Remove(this);
	nState = eSoundSourceState::None;
	sName = this->GetTypeName();
	return ret;
}

void ISoundSource::DoClone(ISoundSource *obj) const
{
	sdAcquire(pSound);
	obj->GenerateCloneName(sName);
	obj->pSound = pSound;
	obj->fVolume = fVolume;
	obj->fFadeTime = fFadeTime;
	obj->fStartFadeTime = fStartFadeTime;
	obj->nState = nState;
	obj->bLoop = bLoop;
	obj->bAutoPlay = bAutoPlay;

	// ISceneObject
	obj->bMarkForDeletion = true;

	// ITransformable
	obj->pParent = pParent;
	obj->mTransform = mTransform;
	obj->vPos = vPos;
	obj->vPivot = vPivot;
	obj->vTransformedPivot = vTransformedPivot;
	obj->vScale = vScale;
	obj->vBoundingBox = vBoundingBox;
	obj->fRotation = fRotation;
	obj->bTransformationChanged = bTransformationChanged;

	// IRenderable
	obj->nBlendOperation = nBlendOperation;
	obj->cColor = cColor;
	obj->bColorChanged = bColorChanged;
	obj->bVisible = bVisible;
}

void ISoundSource::SetVolume(f32 vol)
{
	SEED_ASSERT_MSG((vol >= 0 && vol <= 1.0f), "Source volume must be between 0 and 1");
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
	return ((nState != eSoundSourceState::Stopped) &&
			(nState != eSoundSourceState::Stop) &&
			(nState != eSoundSourceState::Pause) &&
			(nState != eSoundSourceState::Paused) &&
			(nState != eSoundSourceState::None));
}

void ISoundSource::Play()
{
	if (nState != eSoundSourceState::Playing && nState != eSoundSourceState::PlayStarted)
		nState = eSoundSourceState::Play;
}

void ISoundSource::Stop(Seconds s)
{
	UNUSED(s);
	nState = eSoundSourceState::Stop;
}

void ISoundSource::Pause()
{
	if (this->IsPlaying())
	{
		nState = eSoundSourceState::Pause;
	}
}

void ISoundSource::Resume()
{
	if (nState == eSoundSourceState::Pause || nState == eSoundSourceState::Paused)
	{
		nState = eSoundSourceState::Play;
	}
}

void ISoundSource::FadeOut(Seconds s)
{
	if (nState != eSoundSourceState::FadingOut)
		nState = eSoundSourceState::FadeOut;

	fStartFadeTime = pTimer->GetSeconds();
	fFadeTime = s;
}

void ISoundSource::FadeIn(Seconds s)
{
	if (nState != eSoundSourceState::FadingIn)
		nState = eSoundSourceState::FadeIn;

	fStartFadeTime = pTimer->GetSeconds();
	fFadeTime = s;
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
	return nState;
}

void ISoundSource::Render(const mat4 &worldTransform)
{
	UNUSED(worldTransform);
}

} // namespace
