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

#include "RendererDevice.h"

#if defined(USE_API_OGL) && !defined(BUILD_IOS)

#include "Log.h"
#include "Screen.h"
#include "Texture.h"

#if defined(BUILD_SDL)
#define NO_SDL_GLEXT	1
#include <SDL/SDL_opengl.h>
#endif

#if defined(__APPLE_CC__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#if defined(BUILD_SDL)
#include "platform/sdl/sdlDefines.h"
#endif

#define TAG "[OGL14RendererDevice] "

namespace Seed { namespace OpenGL {

OGL14RendererDevice::OGL14RendererDevice()
{
	Log(TAG "Initializing...");

	GLenum err = glewInit();
	if (GLEW_OK != err)
		Log(TAG " Error: %s\n", glewGetErrorString(err));

	char *version = (char *)glGetString(GL_VERSION);
	Info(TAG "OpenGL Version: %s", version);

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	Info(TAG "OpenGL Maximum Texture Size: %d", maxSize);

	this->Reset();

	Log(TAG "Initialization completed.");
}

OGL14RendererDevice::~OGL14RendererDevice()
{
	this->Reset();
}

bool OGL14RendererDevice::Initialize()
{
	bool ret = IRendererDevice::Initialize();

#if !defined(BUILD_QT)
	u32 w = pScreen->GetWidth();
	u32 h = pScreen->GetHeight();

	glScissor(0, 0, w, h);
	glViewport(0, 0, w, h);

	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif

	this->Enable2D();

	return ret;
}

bool OGL14RendererDevice::Reset()
{
	arTexture.Truncate();

	return true; // abstract IRenderer::Reset();
}

bool OGL14RendererDevice::Shutdown()
{
	return IRendererDevice::Shutdown();
}

void OGL14RendererDevice::BackbufferClear(const PIXEL color)
{
	if (color)
	{
		uPixel p;
		p.pixel = color;

		glClearColor(p.rgba.r / 255.0f, p.rgba.g / 255.0f, p.rgba.b / 255.0f, p.rgba.a / 255.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

void OGL14RendererDevice::Begin() const
{
	this->TextureRequestProcess();

	glPushMatrix();
	glLoadIdentity();
}

void OGL14RendererDevice::End() const
{
	glPopMatrix();
	pScreen->ApplyFade();
}

void OGL14RendererDevice::SetBlendingOperation(eBlendMode mode, PIXEL color) const
{
	switch (mode)
	{
		/* REPLACE */
		default:
		case BlendNone:
		{
			// http://home.comcast.net/~tom_forsyth/blog.wiki.html#[[Premultiplied%20alpha]]
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		/* BLEND */
		case BlendDefault:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		break;

		case BlendMerge:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		break;

		case BlendScreen:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		}
		break;

		/* DECAL */
		case BlendDecalOverlay:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		/* MODULATE */
		case BlendAdditive:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendOverlay:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendLighten:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendColorDodge:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE, GL_ONE);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;

		case BlendModulateAlpha:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			u8 mA = PIXEL_GET_A(color);

			glColor4ub(255, 255, 255, mA);
		}
		break;

		case BlendModulate:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u8 mR = PIXEL_GET_R(color);
			u8 mG = PIXEL_GET_G(color);
			u8 mB = PIXEL_GET_B(color);
			u8 mA = PIXEL_GET_A(color);

			glColor4ub(mR, mG, mB, mA);
		}
		break;
	}
}

void OGL14RendererDevice::TextureRequestAbort(ITexture *texture)
{
	arTexture.Remove(texture);
}

void OGL14RendererDevice::TextureRequest(ITexture *texture)
{
	arTexture.Add(texture);
}

void OGL14RendererDevice::TextureRequestProcess() const
{
	for (u32 i = 0; i < arTexture.Size(); i++)
	{
		ITexture *texture = arTexture[i];
		if (texture)
		{
			GLint tex = 0;
			glGenTextures(1, (GLuint *)&tex);
			glBindTexture(GL_TEXTURE_2D, (GLuint)tex);

			eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
			eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);

			if (min == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else if (min == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			if (mag == Seed::TextureFilterLinear)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			else if (mag == Seed::TextureFilterNearest)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			GLuint w = texture->GetAtlasWidth();
			GLuint h = texture->GetAtlasHeight();
			const void *data = texture->GetData();

			// if data == NULL then this can be a dynamic texture. we need just the texture id.
			if (data)
			{
				switch (texture->GetBytesPerPixel())
				{
					case 4:
						// OpenGL 1.2+ only GL_EXT_bgra
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
					break;

					case 3:
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;

					case 2:
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
					break;

					case 1:
						glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
					break;

					default:
					break;
				}
			}
			//glBindTexture(GL_TEXTURE_2D, 0);

			texture->iTextureId = tex;
			texture->Close(); // free ram
		}
	}

	arTexture.Truncate();
}

void OGL14RendererDevice::TextureUnload(ITexture *texture)
{
	if (texture->iTextureId)
		glDeleteTextures(1, &texture->iTextureId);
}

void OGL14RendererDevice::TextureDataUpdate(ITexture *texture)
{
	if (texture->iTextureId)
	{
		glBindTexture(GL_TEXTURE_2D, texture->iTextureId);

		GLuint w = texture->GetAtlasWidth();
		GLuint h = texture->GetAtlasHeight();
		const void *data = texture->GetData();
		ASSERT_NULL(data);

		switch (texture->GetBytesPerPixel())
		{
			case 4:
				// OpenGL 1.2+ only GL_EXT_bgra
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
			break;

			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;

			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;

			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;

			default:
			break;
		}
	}
}

void OGL14RendererDevice::UploadData(void *userData)
{
	RendererPacket *packet = static_cast<RendererPacket *>(userData);

	ITexture *texture = packet->pTexture;
	sVertex *data = static_cast<sVertex *>(packet->pVertexData);

	glPushMatrix();
	glLoadIdentity();

	this->SetBlendingOperation(packet->nBlendMode, packet->iColor.pixel);

	glBindTexture(GL_TEXTURE_2D, texture->iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	eTextureFilter min = texture->GetFilter(Seed::TextureFilterTypeMin);
	eTextureFilter mag = texture->GetFilter(Seed::TextureFilterTypeMag);

	if (min == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (min == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (mag == Seed::TextureFilterLinear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else if (mag == Seed::TextureFilterNearest)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(this->GetOpenGLMeshType(packet->nMeshType));
	glEnable(GL_TEXTURE_2D);
	for (u32 i = 0; i < packet->iSize; i++)
	{
		glTexCoord2f(data[i].cCoords.x, data[i].cCoords.y);
		glVertex2f(data[i].cVertex.x, data[i].cVertex.y);//, data[i].cVertex.z);
	}
	glEnd();

	glPopMatrix();
}

int OGL14RendererDevice::GetOpenGLMeshType(eMeshType type) const
{
	UNUSED(type);
	return GL_TRIANGLE_STRIP;
}

void OGL14RendererDevice::BackbufferFill(PIXEL color)
{
	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
	const GLfloat vertices[] = {0.0f, 0.0f, 0.0f, pScreen->GetHeight(), pScreen->GetWidth(), 0.0f, pScreen->GetWidth(), pScreen->GetHeight()};
	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(PIXEL_GET_R(color), PIXEL_GET_G(color), PIXEL_GET_B(color), PIXEL_GET_A(color));
	glPushMatrix();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glPopAttrib();
}

void OGL14RendererDevice::SetViewport(const Rect<f32> &area) const
{
	GLint x, y;
	GLsizei width, height;

	x = static_cast<GLint>(area.x);
	y = static_cast<GLint>(area.y);
	width = static_cast<GLsizei>(area.width);
	height = static_cast<GLsizei>(area.height);

	glViewport(x, y, width, height);
}

void OGL14RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, bool fill) const
{
	GLfloat vertices[8];

	// A
	vertices[0] = x;
	vertices[1] = y;

	// B
	vertices[2] = w;
	vertices[3] = y;

	if (!fill)
	{
		// D
		vertices[4] = w;
		vertices[5] = h;

		// C
		vertices[6] = x;
		vertices[7] = h;
	}
	else
	{
		// C
		vertices[4] = x;
		vertices[5] = h;

		// D
		vertices[6] = w;
		vertices[7] = h;
	}

	glColor4ub(PIXEL_GET_R(color), PIXEL_GET_G(color), PIXEL_GET_B(color), PIXEL_GET_A(color));

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
		glLoadIdentity();
		if (fill)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		else
			glDrawArrays(GL_LINE_LOOP, 0, 4);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

// FIXME: Viewport aspect ratio...
void OGL14RendererDevice::Enable2D() const
{
#if !defined(BUILD_QT)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0f, pScreen->GetWidth(), pScreen->GetHeight(), 0.0f, -SEED_MAX_PRIORITY, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);

	// Save previous Renderer state
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	//glAlphaFunc(GL_GREATER, 0.1f); /* Blending alpha png fudido, arrumar per-texture */
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
#endif
}

void OGL14RendererDevice::Disable2D() const
{
#if !defined(BUILD_QT)
	// Restore previous Renderer state
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
#endif
}


}} // namespace

#endif // USE_API_OGL
