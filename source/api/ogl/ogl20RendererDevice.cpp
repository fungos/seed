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

#if defined(SEED_ENABLE_OGL20)

#include "Log.h"
#include "Screen.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "Enum.h"

#if defined(BUILD_SDL)
#undef NO_SDL_GLEXT
#define NO_SDL_GLEXT	1
#include <SDL/SDL_opengl.h>
#endif

#if defined(BUILD_IOS)
	#define PIXEL_FORMAT_32 GL_RGBA
	#include "platform/ios/iosView.h"
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
	#define _OPENGL_ES2		1
	#define GL_RGBA8 GL_RGBA8_OES
	#ifndef GL_FRAMEBUFFER
	#define GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
	#endif
	#ifndef GL_RENDERBUFFER
	#define GL_RENDERBUFFER GL_RENDERBUFFER_OES
	#endif
	#ifndef GL_COLOR_ATTACHMENT0
	#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_OES
	#endif
	#define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
	#ifndef GL_DEPTH_ATTACHMENT
	#define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_OES
	#endif
	#ifndef GL_FRAMEBUFFER_COMPLETE
	#define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_OES
	#endif
	#define glGenFramebuffers glGenFramebuffersOES
	#define glDeleteFramebuffers glDeleteFramebuffersOES
	#define glGenRenderbuffers glGenRenderbuffersOES
	#define glDeleteRenderbuffers glDeleteRenderbuffersOES
	#define glBindFramebuffer glBindFramebufferOES
	#define glBindRenderbuffer glBindRenderbufferOES
	#define glRenderbufferStorage glRenderbufferStorageOES
	#define glFramebufferRenderbuffer glFramebufferRenderbufferOES
	#define glFramebufferTexture2D glFramebufferTexture2DOES
	#define glCheckFramebufferStatus glCheckFramebufferStatusOES
	#ifndef GL_STREAM_DRAW
	#define GL_STREAM_DRAW GL_DYNAMIC_DRAW
	#endif
	#define GL_TEXTURE_BIT 0
	#define GL_ENABLE_BIT 0
	#define GL_CURRENT_BIT 0
#else
	#if defined(__APPLE_CC__) || defined(BUILD_GLFW)
		#define PIXEL_FORMAT_32 GL_RGBA
	#else
		#define PIXEL_FORMAT_32 GL_BGRA
	#endif
	#define _OPENGL_2		1
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
#endif

#if defined(DEBUG) && !defined(BUILD_IOS) && defined(USE_API_GLEW)
#define GL_TRACE(x)		if (GLEW_GREMEDY_string_marker) \
						{ \
							glStringMarkerGREMEDY(0, x);\
						}
#else
#define GL_TRACE(x)
#endif

#define GL_MESH(x)		this->GetOpenGLMeshType(x)
#define GL_TARGET(x)	this->GetOpenGLBufferTargetType(x)
#define GL_USAGE(x)		this->GetOpenGLBufferUsageType(x)
#define GL_ELEMTYPE(x)	this->GetOpenGLElementType(x)
#define GL_ELEMSIZE(x)	this->GetOpenGLElementSizeByType(x)

#define TAG "[OGL20RendererDevice] "

namespace Seed { namespace OpenGL {

OGL20RendererDevice::OGL20RendererDevice()
	: vTexture()
	, bHasFrameBuffer(false)
	, bNeedPowerOfTwoTexture(true)
{
	Log(TAG "Initializing...");

#if defined(USE_API_GLEW)
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		Log(TAG "Error: %s\n", glewGetErrorString(err));
	}

	if (GLEW_ARB_texture_non_power_of_two)
		bNeedPowerOfTwoTexture = false;

#if !defined(USE_API_OGL_RT)
	if (GLEW_EXT_framebuffer_object)
		bHasFrameBuffer = true;
#endif

#else
	// check device ext
#endif

#if defined(_OPENGL_ES2)
	pScreen->frameBuffer = 1;
	pScreen->renderBuffer = 1;
#endif

	char *version = (char *)glGetString(GL_VERSION);
	Info(TAG "OpenGL Version: %s", version);

	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	Info(TAG "OpenGL Maximum Texture Size: %d", maxSize);

	Log(TAG "Initialization completed.");
}

OGL20RendererDevice::~OGL20RendererDevice()
{
	this->Reset();
}

bool OGL20RendererDevice::Initialize()
{
	bool ret = IRendererDevice::Initialize();

	GL_TRACE("BEGIN Initialize")
#if !defined(BUILD_QT)
	u32 w = pScreen->GetWidth();
	u32 h = pScreen->GetHeight();

	glScissor(0, 0, w, h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif

	this->Enable2D();

	GL_TRACE("END Initialize")
	return ret;
}

bool OGL20RendererDevice::Reset()
{
	#warning FIXME - mutex lock guard here (?)
	ITextureVector().swap(vTexture);
	return true;
}

bool OGL20RendererDevice::Shutdown()
{
	this->Disable2D();
	this->Reset();
	return IRendererDevice::Shutdown();
}

void OGL20RendererDevice::BackbufferClear(const Color &color) const
{
	GL_TRACE("BEGIN BackbufferClear")
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GL_TRACE("END BackbufferClear")
}

void OGL20RendererDevice::Begin() const
{
	GL_TRACE("BEGIN Begin")

	this->TextureRequestProcess();

#if defined(_OPENGL_ES2)
	glBindFramebuffer(GL_FRAMEBUFFER, pScreen->frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, pScreen->renderBuffer);
#endif

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
	glLoadIdentity();
	GL_TRACE("END Begin")
}

void OGL20RendererDevice::End() const
{
	GL_TRACE("BEGIN End")
	glPopMatrix();
	pScreen->ApplyFade();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	GL_TRACE("END End");
}

void OGL20RendererDevice::SetBlendingOperation(eBlendMode mode, const Color &color) const
{
	GL_TRACE("BEGIN SetBlendingOperation")
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
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;

		/* MODULATE */
		case BlendAdditive:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;

		case BlendOverlay:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ONE);
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;

		case BlendLighten:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;

		case BlendColorDodge:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_ONE, GL_ONE);
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;

		case BlendModulateAlpha:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glColor4ub(255, 255, 255, color.a);
		}
		break;

		case BlendModulate:
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(color.r, color.g, color.b, color.a);
		}
		break;
	}
	GL_TRACE("END SetBlendingOperation")
}

void OGL20RendererDevice::SetTextureParameters(const ITexture *texture) const
{
	GL_TRACE("BEGIN SetTextureParameters")
	SEED_ASSERT(texture);

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
	GL_TRACE("END SetTextureParameters")
}

void OGL20RendererDevice::TextureRequestAbort(ITexture *texture)
{
	#warning FIXME - mutex lock guard here
	vTexture -= texture;
}

void OGL20RendererDevice::TextureRequest(ITexture *texture)
{
	#warning FIXME - mutex lock guard here
	vTexture += texture;
}

void OGL20RendererDevice::TextureRequestProcess() const
{
	#warning FIXME - mutex lock guard here

	GL_TRACE("BEGIN TextureRequestProcess")
	ITextureVector::iterator it = vTexture.begin();
	ITextureVector::iterator end = vTexture.end();
	for (; it != end; ++it)
	{
		ITexture *texture = (*it);
		if (texture)
		{
			GLint tex = 0;
			glGenTextures(1, (GLuint *)&tex);
			glBindTexture(GL_TEXTURE_2D, (GLuint)tex);

			this->SetTextureParameters(texture);

			GLuint w = texture->GetAtlasWidth();
			GLuint h = texture->GetAtlasHeight();
			const void *data = texture->GetData();

			// if data == NULL then this can be a dynamic texture/render target. we need just the texture id.
			if (data)
			{
				u32 bpp = texture->GetBytesPerPixel();

				switch (texture->GetCompressionType())
				{
#if defined(BUILD_IOS)
					case TextureCompression_RGB_PVRTC_2BPPV1:
					{
						//GLuint bpp = 2;
						GLsizei size = w * h * bpp / 8;

						if (size < 32)
							size = 32;

						glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, w, h, 0, size, data);
					}
					break;
#endif

					default:
					{
						switch (bpp)
						{
							case 4:
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, PIXEL_FORMAT_32, GL_UNSIGNED_BYTE, data);
							}
							break;

							case 3:
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
							}
							break;

							case 2:
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
							}
							break;

							case 1:
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
							}
							break;

							default:
							break;
						}
					}
				}
			}
			else if (w && h)// Render Target, 32bits only
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, PIXEL_FORMAT_32, GL_UNSIGNED_BYTE, NULL);
			}
			// else is a dynamic texture from image/video.

			texture->iTextureId = tex;
			texture->Close();
		}
	}

	ITextureVector().swap(vTexture);
	GL_TRACE("END TextureRequestProcess")
}

void OGL20RendererDevice::TextureUnload(ITexture *texture)
{
	GL_TRACE("BEGIN TextureUnload")
	SEED_ASSERT(texture);
	if (texture->iTextureId)
		glDeleteTextures(1, &texture->iTextureId);
	GL_TRACE("END TextureUnload")
}

void OGL20RendererDevice::TextureDataUpdate(ITexture *texture)
{
	GL_TRACE("BEGIN TextureDataUpdate")
	SEED_ASSERT(texture);
	if (texture->iTextureId)
	{
		glBindTexture(GL_TEXTURE_2D, texture->iTextureId);

		GLuint w = texture->GetAtlasWidth();
		GLuint h = texture->GetAtlasHeight();
		const void *data = texture->GetData();
		SEED_ASSERT(data);

		switch (texture->GetBytesPerPixel())
		{
			case 4:
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, PIXEL_FORMAT_32, GL_UNSIGNED_BYTE, data);
			}
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
	GL_TRACE("END TextureDataUpdate")
}

void OGL20RendererDevice::UploadData(void *userData)
{
	GL_TRACE("BEGIN UploadData")

	RendererPacket *packet = static_cast<RendererPacket *>(userData);
	SEED_ASSERT(packet->pTransform);
	SEED_ASSERT(packet->pVertexBuffer);

	const ITexture *texture = packet->pTexture;
	VertexBuffer *vbo = (VertexBuffer *)packet->pVertexBuffer;
	ElementBuffer *ebo = (ElementBuffer *)packet->pElementBuffer;
	Vector3f pivot = packet->vPivot;

	this->SetBlendingOperation(packet->nBlendMode, packet->cColor);

	if (texture)
	{
		this->SetTextureParameters(texture);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->iTextureId);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	GLfloat *pfm = (GLfloat *)packet->pTransform;
	glLoadMatrixf(pfm);

	void *elemPtr = NULL;
#if USE_API_OGL_VBO
	if (ebo)
	{
		if (!ebo->iBuffer)
			glGenBuffers(1, &ebo->iBuffer);

		glBindBuffer(GL_TARGET(ebo->nTarget), ebo->iBuffer);
		if (ebo->bUpdated)
		{
			glBufferData(GL_TARGET(ebo->nTarget), GL_ELEMSIZE(ebo->nElemType) * ebo->iLength, ebo->pData, GL_USAGE(ebo->nUsage));
			ebo->bUpdated = false;
		}
	}

	if (!vbo->iBuffer)
		glGenBuffers(1, &vbo->iBuffer);

	glBindBuffer(GL_TARGET(vbo->nTarget), vbo->iBuffer);
	if (vbo->bUpdated)
	{
		glBufferData(GL_TARGET(vbo->nTarget), sizeof(sVertex) * vbo->iLength, vbo->pData, GL_USAGE(vbo->nUsage));
		vbo->bUpdated = false;
	}

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sVertex), (void *)offsetof(sVertex, cColor));
	if (texture)
		glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), (void *)offsetof(sVertex, cCoords));
	glVertexPointer(3, GL_FLOAT, sizeof(sVertex), (void *)offsetof(sVertex, cVertex));
#else
	if (ebo)
		elemPtr = ebo->pData;

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sVertex), &vbo->pData[0].cColor);
	if (texture)
		glTexCoordPointer(2, GL_FLOAT, sizeof(sVertex), &vbo->pData[0].cCoords);
	glVertexPointer(3, GL_FLOAT, sizeof(sVertex), &vbo->pData[0].cVertex);
#endif

	if (!ebo)
		glDrawArrays(GL_MESH(packet->nMeshType), 0, vbo->iLength);
	else
		glDrawElements(GL_MESH(packet->nMeshType), ebo->iLength, GL_ELEMTYPE(ebo->nElemType), elemPtr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_TRACE("END UploadData")


	if (packet->iFlags & FlagWireframe)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		#if !defined(BUILD_IOS)
			glPushAttrib(GL_POLYGON_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(this->GetOpenGLMeshType(packet->nMeshType), 0, vbo->iLength);
			glPopAttrib();
		#else
			glDrawArrays(GL_LINE_STRIP, 0, vbo->iLength);
		#endif

		if (packet->fRadius)
		{
			Vector3f op = packet->pTransform->getTranslation();
			pRendererDevice->DrawCircle(op.getX(), op.getY(), packet->fRadius, Color(255, 0, 255, 255));
		}

		glPointSize(5.0f);
		if (!ebo)
			glDrawArrays(GL_POINTS, 0, vbo->iLength);
		else
			glDrawElements(GL_POINTS, ebo->iLength, GL_ELEMTYPE(ebo->nElemType), elemPtr);

//		glPointSize(7.0f);
//		glBegin(GL_POINTS);
//			glColor3f(1.0f, 0.0f, 1.0f);
//			glVertex3f(pivot.getX(), pivot.getY(), pivot.getZ());
//		glEnd();

		Vector3f op = packet->pTransform->getTranslation();
		pRendererDevice->DrawCircle(pivot.getX() + op.getX(), pivot.getY() + op.getY(), 3, Color(255, 0, 255, 255));

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}
}

int OGL20RendererDevice::GetOpenGLBufferUsageType(eBufferUsage usage) const
{
	int usages[BufferUsageCount] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
	return usages[usage];
}

int OGL20RendererDevice::GetOpenGLElementType(eElementType type) const
{
	int types[ElementTypeCount] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT};
	return types[type];
}

int OGL20RendererDevice::GetOpenGLElementSizeByType(eElementType type) const
{
	int sizes[ElementTypeCount] = {1, 2, 4};
	return sizes[type];
}

int OGL20RendererDevice::GetOpenGLBufferTargetType(eBufferTarget type) const
{
	int types[BufferTargetCount] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	return types[type];
}

int OGL20RendererDevice::GetOpenGLMeshType(eMeshType type) const
{
	int types[MeshTypeCount] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP};
	return types[type];
}

void OGL20RendererDevice::DestroyHardwareBuffer(IHardwareBuffer *buf) const
{
	if (buf->iBuffer)
	{
		glDeleteBuffers(1, &buf->iBuffer);
		buf->iBuffer = 0;
	}
}

void OGL20RendererDevice::BackbufferFill(const Color &color) const
{
	GL_TRACE("BEGIN BackbufferFill")
	const GLfloat vertices[] = {0.0f, 0.0f, 0.0f, pScreen->GetHeight(), pScreen->GetWidth(), 0.0f, pScreen->GetWidth(), pScreen->GetHeight()};

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(color.r, color.g, color.b, color.a);
	glPushMatrix();
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	GL_TRACE("END BackbufferFill")
}

u32 OGL20RendererDevice::CreateFrameBuffer(ITexture *texture)
{
	GL_TRACE("BEGIN CreateFrameBuffer")
	GLuint fb;

#if defined(USE_API_OGL_RT)
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	if (texture && texture->iTextureId)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->iTextureId, 0);
#endif

	GL_TRACE("END CreateFrameBuffer")
	return fb;
}

void OGL20RendererDevice::DestroyFrameBuffer(u32 buffer)
{
	GL_TRACE("BEGIN DestroyFrameBuffer")

#if defined(USE_API_OGL_RT)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &buffer);
#endif

	GL_TRACE("END DestroyFrameBuffer")
}

u32 OGL20RendererDevice::CreateDepthBuffer(u32 w, u32 h)
{
	GL_TRACE("BEGIN CreateDepthBuffer")
	GLuint db;

#if defined(USE_API_OGL_RT)
	glGenRenderbuffers(1, &db);
	glBindRenderbuffer(GL_RENDERBUFFER, db);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
#endif

	GL_TRACE("END CreateDepthBuffer")

	return db;
}

void OGL20RendererDevice::DestroyDepthBuffer(u32 buffer)
{
	GL_TRACE("BEGIN DestroyDepthBuffer")

#if defined(USE_API_OGL_RT)
	glDeleteRenderbuffers(1, &buffer);
#endif

	GL_TRACE("END DestroyDepthBuffer")
}

void OGL20RendererDevice::AttachDepthBuffer(u32 buffer)
{
	GL_TRACE("BEGIN AttachDepthBuffer")

#if defined(USE_API_OGL_RT)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
#endif

	GL_TRACE("END AttachDepthBuffer")
}

void OGL20RendererDevice::ActivateFrameBuffer(u32 buffer)
{
	GL_TRACE("BEGIN ActivateFrameBuffer")

#if defined(USE_API_OGL_RT)
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
#endif

	GL_TRACE("END ActivateFrameBuffer")
}

void OGL20RendererDevice::ActivateDepthBuffer(u32 buffer)
{
	GL_TRACE("BEGIN ActivateDepthBuffer")

#if defined(USE_API_OGL_RT)
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
#endif

	GL_TRACE("END ActivateDepthBuffer")
}

bool OGL20RendererDevice::CheckFrameBufferStatus() const
{
#if defined(USE_API_OGL_RT)
	GL_TRACE("BEGIN CheckFrameBufferStatus")
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	GL_TRACE("END CheckFrameBufferStatus")

	return (status == GL_FRAMEBUFFER_COMPLETE);
#else
	return false;
#endif
}

void OGL20RendererDevice::EnableScissor(bool b) const
{
	if (b)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void OGL20RendererDevice::SetScissor(f32 x, f32 y, f32 w, f32 h) const
{
	glScissor(x, y, w, h);
}

void OGL20RendererDevice::SetViewport(f32 x, f32 y, f32 w, f32 h) const
{
	GL_TRACE("BEGIN SetViewport")
	glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	GL_TRACE("END SetViewport")
}

void OGL20RendererDevice::DrawCircle(f32 x, f32 y, f32 radius, const Color &color) const
{
	GL_TRACE("BEGIN DrawCircle")
	static const int points = 50;
	float ang = (float)(2 * M_PI / (float)points);
	float cur = 0;
	GLfloat vertices[points * 2];

	for (int i = 0, v = 0; i < points; i++, v += 2)
	{
		vertices[v] = x + sin(cur) * radius;
		vertices[v + 1] = y + cos(cur) * radius;
		cur += ang;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glColor4ub(color.r, color.g, color.b, color.a);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
	glLoadIdentity();
	glDrawArrays(GL_LINE_LOOP, 0, points);

	glPopMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	GL_TRACE("END DrawCircle")
}

void OGL20RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, const Color &color, bool fill) const
{
	GL_TRACE("BEGIN DrawRect")
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

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glColor4ub(color.r, color.g, color.b, color.a);
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
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GL_TRACE("END DrawRect")
}

void OGL20RendererDevice::DrawLines(f32 *points, u32 len, const Color &color) const
{
	GL_TRACE("BEGIN DrawLines")
	GLfloat *vertices = points;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glColor4ub(color.r, color.g, color.b, color.a);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
		glLoadIdentity();
		glDrawArrays(GL_LINE_LOOP, 0, len);
	glPopMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	GL_TRACE("END DrawLines")
}

void OGL20RendererDevice::Enable2D() const
{
	GL_TRACE("BEGIN Enable2D")

#if !defined(BUILD_QT)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

#if defined(_OPENGL_ES2)
	glOrthof(0.0f, pScreen->GetWidth(), pScreen->GetHeight(), 0, SEED_MAX_PRIORITY, -SEED_MAX_PRIORITY);
#else
	glOrtho(0.0f, pScreen->GetWidth(), pScreen->GetHeight(), 0, SEED_MAX_PRIORITY, -SEED_MAX_PRIORITY);
#endif

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_ALWAYS);
//	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
#endif
	GL_TRACE("END Enable2D")
}

void OGL20RendererDevice::Disable2D() const
{
	GL_TRACE("BEGIN Disable2D")
#if !defined(BUILD_QT)
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
#endif
	GL_TRACE("END Disable2D")
}

bool OGL20RendererDevice::NeedPowerOfTwoTextures() const
{
	return bNeedPowerOfTwoTexture;
}

}} // namespace

#endif // USE_API_OGL
