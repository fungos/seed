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

#include "platform/pc/platform.h"

#if defined(__linux__) || defined(__APPLE_CC__) || defined(__FLASHPLAYER)

#include "Log.h"

#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string>
#include <locale>
#include <iostream>
#include <sstream>

#include "SeedInit.h"

#define TAG	"[Platform] "

#if defined(BUILD_IOS)
#include "platform/ios/iosoneView.h"
#elif defined(__APPLE_CC__)
static char pcBundle[2048];
#endif

#if SEED_PATH_WIDE == 1
std::wstring widen(const std::string &str)
{
	using namespace std;

	wostringstream wstm;
	const ctype<wchar_t> &ctfacet = use_facet< ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);

	return wstm.str();
}

std::string narrow(const std::wstring &str)
{
	using namespace std;

	ostringstream stm;
	const ctype<char> &ctfacet = use_facet< ctype<char> >(stm.getloc());
	for (size_t i = 0 ; i < str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);

	return stm.str();
}
#endif

bool create_directory(const char *path)
{
	bool ret = false;
	int err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if (err == -1)
	{
		switch (errno)
		{
			case EACCES:
			{
				Info(TAG "Permission denied to create '%s'.", path);
			}
			break;

			case EEXIST:
			{
				Info(TAG "Path '%s' already exists.", path);
			}
			break;

			case ENAMETOOLONG:
			{
				Info(TAG "Path name too long: '%s'", path);
			}
			break;

			case ENOENT:
			{
				Info(TAG "Path '%s' does not name an existing entry.", path);
			}
			break;

			case ENOSPC:
			{
				Info(TAG "Not enought space to create '%s'.", path);
			}
			break;

			case ENOTDIR:
			{
				Info(TAG "A component of the path '%s' is not a directory.", path);
			}
			break;

			case EROFS:
			{
				Info(TAG "Read-only filesystem, could not create '%s'.", path);
			}
			break;

			default:
			{
				Info(TAG "An error '%d' ocurred trying to create '%s'.", err, path);
			}
			break;
		}
	}
	else
	{
		ret = true;
	}

	return ret;
}

const char *get_user_name()
{
	const char *name = (char *)getenv("LOGNAME");
	if (!name)
		name = (const char *)"Noname";
	return name;
}

const char *get_user_savegame_folder()
{
	return get_user_home_folder();
}

const char *get_user_appdata_folder()
{
	return get_user_home_folder();
}

const char *get_user_home_folder()
{
	const char *chome = getenv("HOME");
	const char *home = (char *)chome;
	if (!home)
		home = (const char *)"./";

	return home;
}

void get_current_directory(char *buff, int size)
{
#if defined(BUILD_IOS)
	memset(buff, '\0', size);
	memcpy(buff, iphGetRootPath(), size);
#elif defined(__APPLE_CC__)
	Seed::Private::iArgc = 1;
	int len = (int)strlen(Seed::Private::pcArgv[0]);
	memcpy(pcBundle, Seed::Private::pcArgv[0], len);
	while (pcBundle[len] != '/') len--;
	len -= (u32)strlen("MacOS");
	memset(&pcBundle[len], '\0', sizeof(pcBundle) - len);
	strcpy(&pcBundle[len], "Resources");

	memcpy(buff, pcBundle, size);
#else
	char *b = (char *)static_cast<void *>(buff);
	getcwd(b, size);
#endif
}

bool change_directory(const char *to)
{
	bool ret = false;
	const char *path = (const char *)to;
	int err = chdir(path);

	if (err == -1)
	{
		switch (errno)
		{
			case EACCES:
			{
				Info(TAG "Permission denied to create '%s'.", path);
			}
			break;

			case EEXIST:
			{
				Info(TAG "Path '%s' already exists.", path);
			}
			break;

			case ENAMETOOLONG:
			{
				Info(TAG "Path name too long: '%s'", path);
			}
			break;

			case ENOENT:
			{
				Info(TAG "Path '%s' does not name an existing entry.", path);
			}
			break;

			case ENOSPC:
			{
				Info(TAG "Not enought space to create '%s'.", path);
			}
			break;

			case ENOTDIR:
			{
				Info(TAG "A component of the path '%s' is not a directory.", path);
			}
			break;

			case EROFS:
			{
				Info(TAG "Read-only filesystem, could not create '%s'.", path);
			}
			break;

			default:
			{
				Info(TAG "An error '%d' ocurred trying to create '%s'.", err, path);
			}
			break;
		}
	}
	else
	{
		ret = true;
	}

	return ret;
}

void print_system_info()
{
	#warning "Must implement function print_system_info"
}

bool system_check_multiple_instance()
{
	#warning "Must implement function system_check_multiple_instance"
	return true;
}

#endif
