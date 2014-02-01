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

#include "LeafMessage.h"

#if defined(SEED_USE_LEAF)

#include "api/net/Address.h"

namespace Seed {

SEED_SINGLETON_DEFINE(Leaf)

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

void Leaf::Initialize()
{
	iPort = 11115;
	cAddress = Address(127, 0, 0, 1, iPort);

	//cSocket.Open(iPort);
}

void Leaf::Log(const char *msg)
{
	Send(ePacket::Log, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Error(const char *msg)
{
	Send(ePacket::Error, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Dbg(const char *msg)
{
	Send(ePacket::Debug, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Send(ePacket packetId, u32 packetSize, const u8 *packetData)
{
	Packet p;
	p.iPacketId = packetId;
	p.iPacketSize = packetSize;

	cSocket.Send(cAddress, &p, sizeof(p));
	cSocket.Send(cAddress, packetData, packetSize);
}

} // namespace

#endif
