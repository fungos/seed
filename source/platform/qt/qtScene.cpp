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

#if defined(BUILD_QT)

#include "platform/qt/qtScene.h"
#include "platform/qt/qtInput.h"
#include "platform/qt/qtScreen.h"
#include "Timer.h"

#define TAG "[Scene] "

namespace Seed { namespace QT {

Scene::Scene(u32 width, u32 height)
	: iWidth(width)
	, iHeight(height)
	, fLastTime(0.0f)
{
	this->setSceneRect(0, 0, width, height);
}

Scene::~Scene()
{
}

void Scene::drawBackground(QPainter *painter, const QRectF &)
{
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());

	painter->beginNativePainting();

	this->setStates(width/height);
	this->drawScene();
	this->defaultStates();

	painter->endNativePainting();
}

bool Scene::drawScene()
{
	f32 newTime		= f32(pTimer->GetSeconds());
//	f32 dt			= newTime - fLastTime;
	fLastTime		= newTime;

	//Private::pRenderer->Render(dt); // FIXME: Viewport rendering!

	return true;
}

// OpenGL specific code

void Scene::setStates(f32 aspect)
{
	UNUSED(aspect)

	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	f32 w = static_cast<f32>(pScreen->GetWidth());
	f32 h = static_cast<f32>(pScreen->GetHeight());

	f32 w2 = static_cast<f32>(width());
	f32 h2 = static_cast<f32>(height());

	//glOrtho(0.0f, 1.0f, h/w, 0.0f, -10000, 0);
	glOrtho(0.0f, w2 / w, h2 / (h + h * (1 - (h / w))), 0.0f, -100000, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);

	// Save previous Renderer state
	glPushAttrib(GL_DEPTH_BUFFER_BIT);

#if SEED_ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#else
	glDisable(GL_DEPTH_TEST);
#endif

	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
}

void Scene::defaultStates()
{
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

QPointF Scene::pixelPosToViewPos(const QPointF& p)
{
	//return QPointF(2.0 * float(p.x()) / width() - 1.0, 1.0 - 2.0 * float(p.y()) / height());
	//return QPointF(float(p.x()) / width(), float(p.y()) / height());
	return QPointF(float(p.x()) / float(pScreen->GetWidth()), float(p.y()) / float(pScreen->GetHeight()));
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());
	pInput->PointerMove(viewpos.x(), viewpos.y());
	//Log(">>>> MOVE: %f,%f", viewpos.x(), viewpos.y());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->buttons() & Qt::LeftButton)
	{
		btn |= static_cast<u32>(Seed::ButtonLeft);
	}
	if (event->buttons() & Qt::RightButton)
	{
		btn |= static_cast<u32>(Seed::ButtonRight);
	}
	if (event->buttons() & Qt::MidButton)
	{
		btn |= static_cast<u32>(Seed::ButtonMiddle);
	}

	pInput->PointerPress(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> PRESS: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());
	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->button() == Qt::LeftButton)
	{
		btn |= static_cast<u32>(Seed::ButtonLeft);
	}
	if (event->button() == Qt::RightButton)
	{
		btn |= static_cast<u32>(Seed::ButtonRight);
	}
	if (event->button() == Qt::MidButton)
	{
		btn |= static_cast<u32>(Seed::ButtonMiddle);
	}

	pInput->PointerRelease(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> RELEASE: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);

	event->accept();
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	QGraphicsScene::wheelEvent(event);
	if (event->isAccepted())
		return;

	QPointF viewpos = pixelPosToViewPos(event->scenePos());

	pInput->fX = static_cast<f32>(viewpos.x());
	pInput->fY = static_cast<f32>(viewpos.y());

	u32 btn = 0;
	if (event->delta() > 0)
	{
		btn |= static_cast<u32>(Seed::ButtonUp);
	}
	else if (event->delta() < 0)
	{
		btn |= static_cast<u32>(Seed::ButtonDown);
	}

	pInput->PointerPress(viewpos.x(), viewpos.y(), btn);
	pInput->PointerRelease(viewpos.x(), viewpos.y(), btn);
	Dbg(">>>> WHEEL: %f,%f BTN: %x", viewpos.x(), viewpos.y(), btn);

	event->accept();
}

void Scene::keyPressEvent(QKeyEvent *event)
{
	QGraphicsScene::keyPressEvent(event);
	if (event->isAutoRepeat() || event->isAccepted())
		return;

	u32 key = static_cast<u32>(event->key());
	u32 mod = static_cast<u32>(event->modifiers());

	pInput->KeyboardPress(key, mod);
	//Dbg(">>>> Key Press: %c [%d, %x] Modifier: %x", key, key, key, mod);

	event->accept();
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
	QGraphicsScene::keyReleaseEvent(event);
	if (event->isAutoRepeat() || event->isAccepted())
		return;

	u32 key = static_cast<u32>(event->key());
	u32 mod = static_cast<u32>(event->modifiers());

	pInput->KeyboardRelease(key, mod);
	//Dbg(">>>> Key Release: %c [%d, %x] Modifier: %x", key, key, key, mod);

	event->accept();
}

}} // namespace

#endif // BUILD_QT
