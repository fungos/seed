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

#ifndef __QT_SCENE_H__
#define __QT_SCENE_H__

#if defined(BUILD_QT)

#include "Defines.h"
#include <QPainter>
#include <QGraphicsScene>

namespace Seed { namespace QT {

/// Qt Graphic Scene
class Scene : public QGraphicsScene
{
	Q_OBJECT

	public:
		Scene(u32 width, u32 height);
		virtual ~Scene();

	protected:
		bool drawScene();
		void drawBackground(QPainter *painter, const QRectF &);
		void setStates(f32 aspect);
		void defaultStates();

		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void wheelEvent(QGraphicsSceneWheelEvent * event);
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);

	private:
		SEED_DISABLE_COPY(Scene);

		QPointF pixelPosToViewPos(const QPointF& p);

	private:
		u32	iWidth;
		u32	iHeight;

		f32 fLastTime;
};

}} // namespace

#endif // BUILD_QT
#endif // __QT_SCENE_H__
