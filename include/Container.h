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

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <stack>

//extern "C" { extern void Log(const char *pMessage, ...); }

#define DECLARE_CONTAINER_HELPER(N, C) \
											\
											template <typename T> \
											class N : public C<T> \
											{ \
												public: \
													void operator+=(const T& element) \
													{ \
														SEED_ASSERT(element); \
														this->push_back(element); \
													} \
													\
													void operator-=(const T& element) \
													{ \
														SEED_ASSERT(element); \
														this->erase(std::remove(this->begin(), this->end(), element), this->end()); \
													} \
													\
													size_t Size() const \
													{ \
														return this->size(); \
													} \
													\
											};

namespace Seed
{
	DECLARE_CONTAINER_HELPER(Vector, std::vector)
}

#define Stack std::stack
#define Map std::map
#define DECLARE_CONTAINER_TYPE(cont, type)	typedef cont<type *> type##cont; \
											typedef type##cont::iterator type##cont##Iterator; \
											typedef type##cont::const_iterator Const##type##cont##Iterator;

#endif // __CONTAINER_H__
