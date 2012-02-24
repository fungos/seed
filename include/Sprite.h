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
	\brief Defines the Sprite class interface
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Defines.h"
#include "interface/IBasicMesh.h"
#include "Reader.h"
#include "SeedInit.h"
#include "Animation.h"
#include "Frame.h"

#define		SPRITE_GLOBAL_FRAME_TIME		60.0f

namespace Seed {

class ITexture;

class SEED_CORE_API Sprite : public IBasicMesh
{
	public:
		friend class ResourceLoader;

	public:
		Sprite();
		virtual ~Sprite();

		virtual bool Load(const Reader &reader, ResourceManager *res = pResourceManager);
		virtual bool Unload();

		virtual ITexture *GetTexture() const;
		virtual const void *GetData() const;

		//virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		//virtual void SetColor(PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual void SetLoop(bool loop);
		virtual bool SetAnimation(u32 index);
		virtual bool SetAnimation(const char *name);
		virtual void SetCurrentFrame(u32 frame);
		virtual void GotoAndStop(u32 frame);
		virtual void GotoAndPlay(u32 frame);
		virtual void Stop();
		virtual void Play();
		virtual bool IsFinished() const;
		virtual void NextFrame();
		virtual void PreviousFrame();
		virtual u32  GetAnimation() const;
		virtual const char *GetAnimationName() const;
		virtual u32 GetAnimationCount() const;
		virtual u32  GetFrameCount() const;
		virtual u32  GetCurrentFrame() const;
		virtual bool IsLoop() const;
		virtual bool IsAnimated() const;
		virtual u32 GetWidthInPixel() const;
		virtual u32 GetHeightInPixel() const;

		//virtual void SetCustomVertexDataArray(Vector3f *myVertexData, u32 qty, eMeshType type = Seed::TriangleStrip);
		//virtual void SetCustomCoordsDataArray(f32 *myCoordsData, u32 qty);

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

		bool bInitialized;
		bool bChanged;
		bool bAnimation;
		bool bLoop;
		bool bPlaying;
		bool bFinished;

		// Frame related width and height *proportional to view port aspect ratio* for rendering only
		f32 fAspectHalfWidth; // real half width
		f32 fAspectHalfHeight; // real half height
		f32 fAspectWidth; // real width
		f32 fAspectHeight; // real height

		u32 iCurrentAnimation;
		u32 iCurrentFrame;
		u32 iAnimations;
		u32 iFrames;
		f32 fCurrentFrameRate;
		f32 fFrameTime;
		//u32 iVertexColor;

		// Frame related width and heigth used for rendering only
		s32 iHalfWidth; // half width in pixel
		s32 iHalfHeight; // half height in pixel
		u32 iWidth; // width in pixel
		u32 iHeight; // height in pixel

		f32 fTexS0;
		f32 fTexS1;
		f32 fTexT0;
		f32 fTexT1;

		ResourceManager *pRes;
		sVertex vert[4];

	private:
		SEED_DISABLE_COPY(Sprite);
};

} // namespace

#endif // __SPRITE_H__
