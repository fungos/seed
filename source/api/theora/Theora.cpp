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

#if SEED_USE_THEORA == 1 && !defined(BUILD_IOS)

#include "api/theora/Theora.h"
#include "Log.h"
#include "Timer.h"
#include "Screen.h"

#define TAG "[Theora] "
#define OGGPLAY_BUFFER_SIZE		20

namespace Seed {

Theora::Theora()
	: pPlayer(nullptr)
	, pTexData(nullptr)
	, pSem(nullptr)
	, iDuration(0)
	, fFps(0.0f)
	, iFpsDenom(1000)
	, iFpsNum(25000)
	, fDelay(0.0f)
	, iFrameCount(0)
	, iUntilFrame(0)
	, iTrack(0)
	, iTotalFrames(0)
	, iWidth(0)
	, iHeight(0)
	, iUVWidth(0)
	, iUVHeight(0)
	, iTexWidth(0)
	, iTexHeight(0)
	, fTexScaleX(0.0f)
	, fTexScaleY(0.0f)
	, fLastFrameTime(0.0f)
	, fElapsedTime(0.0f)
	, cTexture()
	, bLoaded(false)
	, bPaused(true)
	, bPlaying(false)
	, bFinished(false)
	, bTerminateThread(false)
{
}

Theora::~Theora()
{
	this->Reset();
	pPlayer = nullptr;
}

void Theora::Reset()
{
	IRenderable::Reset();

	bFinished = false;
	bPaused = true;
	bPlaying = false;
	bTerminateThread = false;

	this->Unload();
}

bool Theora::Unload()
{
	Image::Unload();

	bFinished = true;
	bTerminateThread = true;
	bPlaying = false;
	bPaused = true;

	Free(pTexData);

	return true;
}

bool Theora::Run()
{
	bool ret = this->IsRunning();
	if (ret)
	{
		if (bPlaying && !bFinished)
		{
	test:
			OggPlayErrorCode r = E_OGGPLAY_TIMEOUT;
			if (pSem)
				pSem->Wait();

			while (r == E_OGGPLAY_TIMEOUT && !bTerminateThread)
			{
				r = oggplay_step_decoding(pPlayer);
			}

			if (r != E_OGGPLAY_CONTINUE && r != E_OGGPLAY_USER_INTERRUPT)
			{
				bFinished = true;
				pTimer->Sleep(10);
				if (pSem)
					pSem->Wait();
			}
		}

		ret = !bTerminateThread;
	}

	if (bTerminateThread)
	{
		sdDelete(pSem);
		if (pPlayer)
			oggplay_close(pPlayer);
	}

	pTimer->Sleep(10);
	//Log("%d", bTerminateThread);
	return ret && this->IsRunning();
}

bool Theora::Load(const String &filename)
{
	return this->Load(filename, pResourceManager, pool);
}

bool Theora::Load(const String &filename, ResourceManager *res)
{
	pRes = res;

	if (this->Unload())
	{
		OggPlayReader *reader = oggplay_file_reader_new(filename);
		pPlayer = oggplay_open_with_reader(reader);

		bLoaded = false;
		bTerminateThread = false;

		if (pPlayer)
		{
			u32 tracks = oggplay_get_num_tracks(pPlayer);
			Log(TAG "There are %d tracks.", tracks);

			for (u32 i = 0; i < tracks; i++)
			{
				Log(TAG "Track %d is of type %s", i, oggplay_get_track_typename(pPlayer, i));
				if (oggplay_get_track_type(pPlayer, i) == OGGZ_CONTENT_THEORA)
				{
					oggplay_set_callback_num_frames(pPlayer, i, 1);

					oggplay_get_video_fps(pPlayer, i, (int*)&iFpsDenom, (int*)&iFpsNum);
					iDuration = (u32)oggplay_get_duration(pPlayer);
					fFps = static_cast<f32>(iFpsNum) / static_cast<f32>(iFpsDenom);
					fDelay = 1000 / fFps;
					iTotalFrames = static_cast<u32>(iDuration * fFps / 1000) + 1;

					if (oggplay_set_track_active(pPlayer, i) < 0)
					{
						Log(TAG "Note: Could not set this track active!");
					}
					else
					{
						iTrack = i;
					}
				}
			}

			oggplay_use_buffer(pPlayer, OGGPLAY_BUFFER_SIZE);

			if (!pSem)
				pSem = sdNew(Semaphore(OGGPLAY_BUFFER_SIZE));

			pSem->Wait();
			this->ConfigureRendering();
		}
		else
		{
			reader = nullptr;
			Log(TAG "ERROR: could not initialise oggplay with '%s'", filename);
		}
	}

	return bLoaded;
}

void Theora::Update(Seconds dt)
{
	if (bPlaying)
	{
		Image::Update(dt);
		if (iUntilFrame && iUntilFrame == iFrameCount)
		{
			this->Pause();
			return;
		}

		if (!this->WaitFrameRate())
			return;

		OggPlayDataHeader **headers;
		OggPlayVideoData *video_data;
		OggPlayDataType type;
		OggPlayCallbackInfo **track_info;

		u32 num_tracks = oggplay_get_num_tracks(pPlayer);
		u32 required = 0;

		track_info = oggplay_buffer_retrieve_next(pPlayer);
		if (track_info == nullptr)
		{
			if (bFinished)
				this->Stop();
			// else?
			return;
		}

		for (u32 i = 0; i < num_tracks; i++)
		{
			type = oggplay_callback_info_get_type(track_info[i]);
			headers = oggplay_callback_info_get_headers(track_info[i]);

			switch (type)
			{
				case OGGPLAY_INACTIVE:
				break;

				case OGGPLAY_YUV_VIDEO:
					/*
					* there should only be one record
					*/
					required = oggplay_callback_info_get_required(track_info[i]);
					if (required == 0)
					{
						oggplay_buffer_release(pPlayer, track_info);
						goto next_frame;
					}
					video_data = oggplay_callback_info_get_video_data(headers[0]);
					this->ProcessVideoData(/*i,*/video_data);
				break;

				case OGGPLAY_RGBA_VIDEO:
					// Overlay
				break;

				case OGGPLAY_FLOATS_AUDIO:
				break;

				case OGGPLAY_CMML:
					if (oggplay_callback_info_get_required(track_info[i]) > 0)
						Info(TAG "%s", oggplay_callback_info_get_text_data(headers[0]));
				break;

				case OGGPLAY_KATE:
					required = oggplay_callback_info_get_required(track_info[i]);
					for (u32 j = 0; j < required; j++)
						Info(TAG "[%d] %s", j, oggplay_callback_info_get_text_data(headers[j]));
				break;

				default:
				break;
			}
		}

		iFrameCount++;
		oggplay_buffer_release(pPlayer, track_info);

	next_frame:
		if (pSem)
			pSem->Notify();
	}
}

bool Theora::GoToFrame(u32 frame)
{
	SEED_ASSERT(frame < iTotalFrames);

	bool ret = false;
	iFrameCount = frame;
	ogg_int64_t pos = static_cast<ogg_int64_t>(frame * fDelay);

	if (oggplay_seek(pPlayer, pos) == E_OGGPLAY_CANT_SEEK)
	{
		Log(TAG "Cant seek forward.");
	}
	else
	{
		ret = true;
		if (pSem)
			pSem->Notify();
	}

	return ret;
}

void Theora::Rewind()
{
	bPaused = true;
	iFrameCount = 0;
	if (oggplay_seek(pPlayer, 0) == E_OGGPLAY_CANT_SEEK)
	{
		Log(TAG "Cant seek backwards.");
	}

	if (pSem)
		pSem->Notify();
}

u32 Theora::GetFrameCount() const
{
	return iFrameCount;
}

f32 Theora::GetFrameRate() const
{
	return fFps;
}

void Theora::PlayToFrame(u32 frame)
{
	iUntilFrame = frame;
	this->DoPlay();
}

void Theora::Play()
{
	iUntilFrame = 0;
	this->DoPlay();
}

void Theora::DoPlay()
{
	if (bPlaying)
		return;

	if (bLoaded)
	{
		if (!bPaused)
			this->Rewind();

		bFinished = false;
		bPlaying = true;
		bPaused = false;
	}
}

void Theora::Stop()
{
	bPlaying = false;
	bPaused = false;
}

bool Theora::IsStopped() const
{
	return (!bPlaying || bFinished) && !bPaused;
}

void Theora::Pause()
{
	fLastFrameTime = 0.0f;
	bPaused = true;
	bPlaying = false;
}

bool Theora::IsPaused() const
{
	return bPaused;
}

bool Theora::IsPlaying() const
{
	return bPlaying;
}

u32 Theora::Size() const
{
	return iTotalFrames;
}

void Theora::ProcessVideoData(OggPlayVideoData *data)
{
	OggPlayYUVChannels yuv;
	OggPlayRGBChannels rgb;

	yuv.ptry = data->y;
	yuv.ptru = data->u;
	yuv.ptrv = data->v;
	yuv.uv_width = iUVWidth;
	yuv.uv_height = iUVHeight;
	yuv.y_width = iWidth;
	yuv.y_height = iHeight;

	rgb.ptro = pTexData;
	rgb.rgb_width = iTexWidth;
	rgb.rgb_height = iTexHeight;

	/*eRendererDeviceType type = pConfiguration->GetRendererDeviceType();
	if (type == Seed::RendererDeviceOpenGL14 || type == Seed::RendererDeviceOpenGL20 ||
		type == Seed::RendererDeviceOpenGL30 || type == Seed::RendererDeviceOpenGL40)
	{
		oggplay_yuv2bgra(&yuv, &rgb);
	}
	else // DirectX
	{
		oggplay_yuv2argb(&yuv, &rgb);
	}*/

	oggplay_yuv2bgra(&yuv, &rgb);
	cTexture.Update(static_cast<uPixel *>((void *)pTexData));
}

bool Theora::WaitFrameRate()
{
	bool ret = false;
	if (!fLastFrameTime)
		fLastFrameTime = pTimer->GetMilliseconds();

	//hold fps
	Milliseconds fTime	 = pTimer->GetMilliseconds();
	fElapsedTime		+= (fTime - fLastFrameTime);
	fLastFrameTime		 = fTime;

	if (fElapsedTime >= fDelay)
	{
		fElapsedTime -= fDelay;
		ret = true;
	}

	return ret;
}

void Theora::ConfigureRendering()
{
	u32 po2_width = 0;
	u32 po2_height = 0;

	oggplay_get_video_y_size(pPlayer, iTrack, reinterpret_cast<int *>(&iWidth), reinterpret_cast<int *>(&iHeight));
	oggplay_get_video_uv_size(pPlayer, iTrack, reinterpret_cast<int *>(&iUVWidth), reinterpret_cast<int *>(&iUVHeight));

	SEED_ASSERT_MSG((iUVWidth == (iWidth / 2)), "Invalid width ");
	SEED_ASSERT_MSG((iUVHeight == (iHeight / 2)), "Invalid height ");

	for (po2_width = 1; po2_width < iWidth; po2_width <<= 1);
	for (po2_height = 1; po2_height < iHeight; po2_height <<= 1);

	fTexScaleX = static_cast<f32>(iWidth) / static_cast<f32>(po2_width);
	fTexScaleY = static_cast<f32>(iHeight) / static_cast<f32>(po2_height);

	if (pTexData == nullptr)
	{
		//pTexData = reinterpret_cast<u8 *>(calloc(1, po2_width * po2_height * 4));
		u32 size = po2_width * po2_height * 4;
		pTexData = reinterpret_cast<u8 *>(Alloc(size));
		MEMSET(pTexData, '\0', size);
		iTexWidth = po2_width;
		iTexHeight = po2_height;
	}

	cTexture.Load(ROUND_UP(iWidth, 32), iHeight, static_cast<uPixel *>((void *)pTexData), iTexWidth, iTexHeight);
	Image::Load(&cTexture);

	bTerminateThread = false;
	bLoaded = true;
	bPlaying = false;
	bFinished = false;
	bPaused = true;

	this->Create();
	this->Run();
}

void Theora::Render()
{
	Image::Render();
}


} // namespace


#endif // SEED_USE_THEORA
