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

#if SEED_USE_LEAF == 1

#include "api/net/Address.h"

namespace Seed {

SEED_SINGLETON_DEFINE(Leaf)

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

bool Leaf::Initialize()
{
	iPort = 11115;
	cAddress = Address(127, 0, 0, 1, iPort);

	// FIXME: performance check: locks, interlockexchange, critical sections, etc.
	ScopedMutexLock lock(mMutex);
	bEnabled = true;

	//cSocket.Open(iPort + 1);

	return true;
}

bool Leaf::Shutdown()
{
	ScopedMutexLock lock(mMutex);
	//cSocket.Close();
	bEnabled = false;

	return true;
}

void Leaf::Log(const char *msg) const
{
	Send(ePacket::Log, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Error(const char *msg) const
{
	Send(ePacket::Error, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Dbg(const char *msg) const
{
	Send(ePacket::Debug, strlen(msg) + 1, (const u8 *)msg);
}

void Leaf::Alloc(const void *data, u32 size) const
{
	Send(ePacket::Allocation, size, (const u8 *)data);
}

void Leaf::Free(const void *data, u32 size) const
{
	Send(ePacket::Free, size, (const u8 *)data);
}

void Leaf::Start() const
{
	Send(ePacket::Start, 0, nullptr);
}

void Leaf::Stop() const
{
	Send(ePacket::Stop, 0, nullptr);
}

void Leaf::Send(ePacket packetId, u32 packetSize, const u8 *packetData) const
{
	if (!bEnabled)
		return;

	Packet p;
	p.iPacketId = packetId;
	p.iPacketSize = packetSize;

	ScopedMutexLock lock(mMutex);
	cSocket.Send(cAddress, &p, sizeof(p));
	if (packetSize && packetData)
		cSocket.Send(cAddress, packetData, packetSize);
}

} // namespace

#endif
