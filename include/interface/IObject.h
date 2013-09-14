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

#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "Defines.h"
#include "Enum.h"

namespace Seed {

/// Object Interface
/**
Interface for basic object
*/

typedef ptrdiff_t TypeId;

#define SEED_DECLARE_RTTI(type, base)	protected:														\
											virtual bool IsKindOf_Internal(TypeId t) const override		\
											{															\
												if (GetTypeId() == t)									\
													return true;										\
												else													\
													return Base::IsKindOf_Internal(t);					\
											}															\
																										\
										public:															\
											typedef type class_need_declare_rtti;						\
											typedef base Base;											\
																										\
											static const char *GetTypeName()							\
											{															\
												return #type;											\
											}															\
																										\
											static TypeId GetTypeId()									\
											{															\
												return (TypeId)&s_internal_##type;						\
											}															\
										private:														\




class SEED_CORE_API IObject
{
	SEED_DISABLE_COPY(IObject)

	protected:
		virtual bool IsKindOf_Internal(TypeId t) const
		{
			if (this->GetTypeId() == t)
				return true;
			return false;
		}

		virtual bool IsKindOf(TypeId t) const
		{
			return IsKindOf_Internal(t);
		}

	public:
		IObject()
			: sName()
		{
		}

		virtual ~IObject()
		{
		}

		static const char *GetTypeName()
		{
			return "IObject";
		}

		static TypeId GetTypeId()
		{
			static const char s_internal = 0;
			return (TypeId)&s_internal;
		}

		template<class T>
		bool IsKindOf() const
		{
			return IsKindOf(T::GetTypeId());
		}

		String sName;
};

} // namespace

#endif // __IOBJECT_H__
