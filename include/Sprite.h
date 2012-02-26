/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file Sprite.h
	\author	Danny Angelo Carminati Grein
	\brief Sprite is an animated 2D image composed with animations and frames.
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Defines.h"
#include "interface/IBasicMesh.h"
#include "Reader.h"
#include "Vertex.h"

namespace Seed {

class ITexture;
class Animation;
class Frame;

class SEED_CORE_API Sprite : public IBasicMesh
{
	public:
		friend class ResourceLoader;

	public:
		Sprite();
		virtual ~Sprite();

		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager);
		virtual bool Unload();

		virtual ITexture *GetTexture() const;
		virtual const void *GetData() const;

		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual void SetLoop(bool loop);
		virtual bool SetAnimation(u32 index);
		virtual bool SetAnimation(String name);
		virtual void SetCurrentFrame(u32 frame);
		virtual void GotoAndStop(u32 frame);
		virtual void GotoAndPlay(u32 frame);
		virtual void Stop();
		virtual void Play();
		virtual bool IsFinished() const;
		virtual void NextFrame();
		virtual void PreviousFrame();
		virtual u32  GetAnimation() const;
		virtual const String GetAnimationName() const;
		virtual u32 GetAnimationCount() const;
		virtual u32  GetFrameCount() const;
		virtual u32  GetCurrentFrame() const;
		virtual bool IsLoop() const;
		virtual bool IsAnimated() const;

		virtual void Reset();
		virtual void Initialize();

		// ISceneObject
		virtual void Update(f32 delta);
		virtual void Render();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		virtual void ReconfigureAnimation();
		virtual void ReconfigureFrame();

	protected:
		Animation	**ppAnimations;
		Animation	*pAnimation;
		Frame		**ppAnimationFrames;
		Frame		*pFrame;
		ITexture	*pFrameTexture;

		u32 iCurrentAnimation;
		u32 iCurrentFrame;
		u32 iAnimations;
		u32 iFrames;
		f32 fFrameTime;

		sVertex vert[4];
		String sName;

		bool bInitialized;
		bool bChanged;
		bool bAnimation;
		bool bLoop;
		bool bPlaying;
		bool bFinished;

	private:
		SEED_DISABLE_COPY(Sprite);
};

} // namespace

#endif // __SPRITE_H__
