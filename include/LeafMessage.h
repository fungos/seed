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

#ifndef __LEAF_MESSAGE_H__
#define __LEAF_MESSAGE_H__

#include "Defines.h"
#include "Singleton.h"
#include "api/net/UDPSocket.h"

namespace Seed {

using namespace Seed::Net;

class Leaf
{
	SEED_DECLARE_SINGLETON(Leaf)
	SEED_DISABLE_COPY(Leaf)

	public:
		enum class ePacket : u32
		{
			Log			= 1,
			Error		= 2,
			Debug		= 3,

			Allocation  = 100,
			Free		= 101
		};

	public:
		bool Initialize();
		bool Shutdown();

		void Log(const char *msg);
		void Error(const char *msg);
		void Dbg(const char *msg);
		void Alloc(const void *data, u32 size);
		void Free(const void *data, u32 size);

	protected:
		void Send(ePacket packetId, u32 packetSize, const u8 *packetData);

	private:
		UDPSocket cSocket;
		Address cAddress;
		u32 iPort;

		struct Packet
		{
			ePacket iPacketId;
			u32 iPacketSize;
			// data
		};
};

} // namespace

#endif // __LEAF_MESSAGE_H__
