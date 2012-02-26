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
