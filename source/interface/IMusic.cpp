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

#include "interface/IMusic.h"

namespace Seed {

IMusic::IMusic()
	: fVolume(1.0f)
	, eState(Seed::MusicNone)
	, bAutoUnload(false)
{
}

IMusic::~IMusic()
{
}

bool IMusic::Unload()
{
	SEED_ABSTRACT_METHOD;
	return false;
}

bool IMusic::Load(const String &filename, ResourceManager *res)
{
	UNUSED(filename);
	UNUSED(res);
	SEED_ABSTRACT_METHOD;

	return false;
}

bool IMusic::Update(f32 dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;

	return false;
}

const void *IMusic::GetData() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

void IMusic::SetVolume(f32 volume)
{
	fVolume = volume;
}

f32 IMusic::GetVolume() const
{
	return fVolume;
}

void IMusic::SetAutoUnload(bool b)
{
	bAutoUnload = b;
}

int IMusic::GetObjectType() const
{
	return Seed::ObjectMusic;
}

const char *IMusic::GetObjectName() const
{
	return "IMusic";
}

void IMusic::Play()
{
	SEED_ABSTRACT_METHOD;
}

void IMusic::Stop()
{
	SEED_ABSTRACT_METHOD;
}

void IMusic::Pause()
{
	SEED_ABSTRACT_METHOD;
}

bool IMusic::IsPlaying() const
{
	return ((eState != MusicStopped) &&
			(eState != MusicStop) &&
			(eState != MusicPause) &&
			(eState != MusicPaused) &&
			(eState != MusicNone));
}

} // namespace
