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

#include "Rand.h"
#include <stdlib.h>
#include <math.h>

namespace Seed {

SEED_SINGLETON_DEFINE(Rand)

Rand::Rand()
	: iSeed(0)
{
}

Rand::~Rand()
{
}

void Rand::Initialize(u32 seed)
{
	if (!seed)
		iSeed = RAND;
	else
		iSeed = seed;
}

u32 Rand::Get(u32 max)
{
	return this->Get(0, max);
}

f32 Rand::Get(f32 max)
{
	return this->Get(0, max);
}

u32 Rand::Get(u32 min, u32 max)
{
	if (min == max)
		return min;

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed ^ iSeed >> 15) % (max - min);

	//return min + (RAND >> 15) % (max - min);
}

f32 Rand::Get(f32 min, f32 max)
{
	if (min == max)
		return min;

	iSeed = 214013 * iSeed + 2531011;
	return min + (iSeed >> 16) * (1.0f / 65535.0f) * (max - min);

	//return min + (RAND >> 16) * (1.0f / 65535.0f) * (max - min);
}

} // namespace
