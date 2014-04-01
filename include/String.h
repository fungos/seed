#ifndef _STRING_H_
#define _STRING_H_

#include <string>

typedef std::string String;

#ifdef _MSC_VER
#define SEED_strcasecmp _stricmp
#else
#define SEED_strcasecmp strcasecmp
#endif

struct StringUtil
{
	inline static bool Equals(String a, String b)
	{
		return (SEED_strcasecmp(a.c_str(), b.c_str()) == 0);
	}

	inline static bool Equals(String a, const char *b)
	{
		return (SEED_strcasecmp(a.c_str(), b) == 0);
	}

	inline static bool Equals(const char *a, const char *b)
	{
		return (SEED_strcasecmp(a, b) == 0);
	}
};

#endif // _STRING_H_
