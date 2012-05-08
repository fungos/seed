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

#include "Particle.h"
#include "Enum.h"

namespace Seed {

IDataObject *FactoryParticle()
{
	return New(Particle());
}

Particle::Particle()
	: Sprite()
	, vVelocity(0.0f, 0.0f, 0.0f)
	, fGravity(0.0f)
	, fRadialAccel(0.0f)
	, fTangentialAccel(0.0f)
	, fSpin(0.0f)
	, fSpinDelta(0.0f)
	, fSize(0.0f)
	, fSizeDelta(0.0f)
	, fAge(0.0f)
	, fTerminalAge(0.0f)
	, fColorR(0.0f)
	, fColorG(0.0f)
	, fColorB(0.0f)
	, fColorA(0.0f)
	, fColorDeltaR(0.0f)
	, fColorDeltaG(0.0f)
	, fColorDeltaB(0.0f)
	, fColorDeltaA(0.0f)
	, bActive(false)
{
}

Particle::~Particle()
{
}

Particle::Particle(const Particle &other)
	: Sprite(other)
	, vVelocity(other.vVelocity)
	, fGravity(other.fGravity)
	, fRadialAccel(other.fRadialAccel)
	, fTangentialAccel(other.fTangentialAccel)
	, fSpin(other.fSpin)
	, fSpinDelta(other.fSpinDelta)
	, fSize(other.fSize)
	, fSizeDelta(other.fSizeDelta)
	, fAge(other.fAge)
	, fTerminalAge(other.fTerminalAge)
	, fColorR(other.fColorR)
	, fColorG(other.fColorG)
	, fColorB(other.fColorB)
	, fColorA(other.fColorA)
	, fColorDeltaR(other.fColorDeltaR)
	, fColorDeltaG(other.fColorDeltaG)
	, fColorDeltaB(other.fColorDeltaB)
	, fColorDeltaA(other.fColorDeltaA)
	, bActive(other.bActive)
{
}

Particle &Particle::operator=(const Particle &other)
{
	if (this != &other)
	{
		Sprite::operator=(other);

		vVelocity = other.vVelocity;

		fGravity = other.fGravity;
		fRadialAccel = other.fRadialAccel;
		fTangentialAccel = other.fTangentialAccel;
		fSpin = other.fSpin;
		fSpinDelta = other.fSpinDelta;
		fSize = other.fSize;
		fSizeDelta = other.fSizeDelta;
		fAge = other.fAge;
		fTerminalAge = other.fTerminalAge;
		fColorR = other.fColorR;
		fColorG = other.fColorG;
		fColorB = other.fColorB;
		fColorA = other.fColorA;
		fColorDeltaR = other.fColorDeltaR;
		fColorDeltaG = other.fColorDeltaG;
		fColorDeltaB = other.fColorDeltaB;
		fColorDeltaA = other.fColorDeltaA;

		bActive = other.bActive;
	}
	return *this;
}


} // namespace
