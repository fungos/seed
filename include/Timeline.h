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

#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include "Defines.h"
#include "Point.h"
#include "Container.h"
#include "interface/IDataObject.h"

namespace Seed {

class Movie;
class ISceneObject;
class Keyframe;
class IEventMovieListener;

/// Movie Timeline
class SEED_CORE_API Timeline : public IDataObject
{
	SEED_DISABLE_COPY(Timeline)
	SEED_DECLARE_RTTI(Timeline, IDataObject)

	typedef Map<u32, Keyframe *> KeyframeMap;
	typedef KeyframeMap::iterator KeyframeMapIterator;

	public:
		Timeline();
		virtual ~Timeline();

		void Reset();
		void Update();
		void Rewind();

		void AddKeyframe(Keyframe *keyframe);

		void GotoAndPlay(u32 frame);
		void GotoAndPlay(const char *strKeyframeName);

		void SetObject(ISceneObject *object);
		ISceneObject *GetObject() const;
		void SetPriority(u32 p);
		u32 GetPriority() const;
		f32 GetWidth() const;
		f32 GetHeight() const;
		const String &GetName() const;

		u32 GetCurrentFrame() const;
		Keyframe *GetCurrentKeyframe();

		void SetLocalPosition(f32 posX, f32 posY);
		void SetPosition(f32 posX, f32 posY);
		void SetScale(f32 scaleX, f32 scaleY);
		void SetRotation(f32 rotation);

		void SetParent(Movie *parent);
		Movie *GetParent() const;

		void SetListener(IEventMovieListener *listener);

		// IDataObject
		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager) override;
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;

	private:
		s32 FindNextKeyframe();
		s32 FindPreviousKeyframe();
		s32 FindKeyframeByName(const String &name);

		f32 EaseQuadPercent(f32 time, f32 begin, f32 change, f32 duration, f32 easing);

	private:
		Movie				*pParent;
		ISceneObject		*pObject;
		IEventMovieListener *pListener;

		String		sName;

		f32			fElapsedTime;
		f32			fElapsedKeyframeTime;
		s32			iCurrentFrame;
		s32			iKeyframeFrom;
		s32			iKeyframeTo;
		s32			iPriority;

		Point2f		ptParentPosition;
		Point2f		ptParentLocalPosition;
		Point2f		ptParentScale;
		f32			fParentRotation;

		KeyframeMap	mapKeyframes;
};

} // namespace

#endif // __TIMELINE_H__
