/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file Container.h
	\author	Danny Angelo Carminati Grein
	\brief Container utils
*/

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <vector>
#include <algorithm>
#include <string>
#include <map>

#ifdef USE_BOOST_ALLOCATOR
	#include <boost/pool/pool_alloc.hpp>
	typedef std::basic_string<char, std::char_traits<char>, boost::fast_pool_allocator<char> > String;
#else
	typedef std::string String;
#endif

#define DECLARE_CONTAINER_HELPER(N, C) \
											\
											template <typename T> \
											class N : public C<T> \
											{ \
												public: \
													void operator+=(const T& element) \
													{ \
														ASSERT_NULL(element); \
														push_back(element); \
													} \
													\
													void operator-=(const T& element) \
													{ \
														ASSERT_NULL(element); \
														this->erase(std::remove(this->begin(), this->end(), element), this->end()); \
													} \
													\
													size_t Size() const \
													{ \
														return this->size(); \
													} \
													\
											};

namespace Seed {
	DECLARE_CONTAINER_HELPER(Vector, std::vector)
}

#define Map std::map
#define DECLARE_CONTAINER_TYPE(cont, type)	typedef cont<type *> type##cont; \
											typedef type##cont::iterator type##cont##Iterator; \
											typedef type##cont::const_iterator Const##type##cont##Iterator;

#define ForEach(type, v, block)				{\
												type##Iterator it = v.begin();\
												type##Iterator end = v.end();\
												for (; it != end; ++it)\
													block\
											}

#define ForEachConst(type, v, block)		{\
												Const##type##Iterator it = v.begin();\
												Const##type##Iterator end = v.end();\
												for (; it != end; ++it)\
													block\
											}

#endif // __CONTAINER_H__
