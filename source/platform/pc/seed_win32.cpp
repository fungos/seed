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

#if defined(WIN32)

#include "Defines.h"
#include "Log.h"
#include "Configuration.h"

#pragma push_macro("Delete")
#pragma push_macro("Free")
#undef Free
#undef Delete
#include <io.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#pragma pop_macro("Free")
#pragma pop_macro("Delete")


#define TAG	"[Platform] "

#define SYSTEM_USERNAME_MAXLEN		1024
#define SYSTEM_USERHOME_MAXLEN		1024
#define SYSTEM_USERAPPDATA_MAXLEN	1024
#define SYSTEM_USERSAVEGAME_MAXLEN	1024
#define SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN	"Saved Games"
#define SYSTEM_SAVEGAME_FOLDER_XP			""
#define SYSTEM_APPDATA_FOLDER_VISTA_SEVEN	"%LOCALAPPDATA%"
#define SYSTEM_APPDATA_FOLDER_XP			"%APPDATA%"

#define CREATE_MUTEX_INITIAL_OWNER 0x00000001

static char strUserName[SYSTEM_USERNAME_MAXLEN];
static char strUserHomePath[SYSTEM_USERHOME_MAXLEN];
static char strUserAppDataPath[SYSTEM_USERAPPDATA_MAXLEN];
static char strUserSaveGamePath[SYSTEM_USERSAVEGAME_MAXLEN];
static bool bIsVistaOrSeven = false;

void system_version()
{
	OSVERSIONINFO os;

	os.dwOSVersionInfoSize = sizeof(os);
	GetVersionEx(&os);

	bIsVistaOrSeven = false;
	if (os.dwMajorVersion >= 6)
		bIsVistaOrSeven = true;
}

bool create_directory(const wchar_t *path)
{
	CreateDirectoryW(path, nullptr);
	u32 err = GetLastError();

	return (err == 0);
}

const char *get_user_name()
{
	u32 len = SYSTEM_USERNAME_MAXLEN;
	memset((void *)strUserName, '\0', sizeof(strUserName));

	if (!GetUserNameA((LPSTR)&strUserName[0], (LPDWORD)&len))
	{
		return "Noname";
	}

	return strUserName;
}

const char *get_user_home_folder()
{
	memset((void *)strUserHomePath, '\0', sizeof(strUserHomePath));
	u32 count = ExpandEnvironmentStringsA("%USERPROFILE%\\", (LPSTR)&strUserHomePath[0], SYSTEM_USERHOME_MAXLEN);
	if (count > SYSTEM_USERHOME_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user home folder (too big!)");
		return "./";
	}

	return strUserHomePath;
}

const char *get_user_savegame_folder()
{
	const char *s = get_user_home_folder();

	memset((void *)strUserSaveGamePath, '\0', sizeof(strUserSaveGamePath));

#if defined(__GNUC__)
#define _snprintf snprintf
#endif

	system_version();
	if (bIsVistaOrSeven)
		_snprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, "%s/%s/", s, SYSTEM_SAVEGAME_FOLDER_VISTA_SEVEN);
	else
		_snprintf(strUserSaveGamePath, SYSTEM_USERSAVEGAME_MAXLEN, "%s/", s);

	return strUserSaveGamePath;
}

const char *get_user_appdata_folder()
{
	memset((void *)strUserAppDataPath, '\0', sizeof(strUserAppDataPath));
	u32 count = 0;

	system_version();
	if (bIsVistaOrSeven)
		ExpandEnvironmentStringsA(SYSTEM_APPDATA_FOLDER_VISTA_SEVEN "\\", (LPSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);
	else
		ExpandEnvironmentStringsA(SYSTEM_APPDATA_FOLDER_XP "\\", (LPSTR)&strUserAppDataPath[0], SYSTEM_USERAPPDATA_MAXLEN);

	if (count > SYSTEM_USERAPPDATA_MAXLEN)
	{
		Log(TAG "WARNING: Could not get user appdata folder (too big!)");
		return "./";
	}

	return strUserAppDataPath;
}

void get_current_directory(wchar_t *buff, int size)
{
	(void)_wgetcwd(buff, size);
}

bool change_directory(const wchar_t *to)
{
	return (_wchdir(to) == 0);
}

void print_system_info()
{
	OSVERSIONINFO os;
	MEMORYSTATUS mem;

	os.dwOSVersionInfoSize = sizeof(os);
	GetVersionEx(&os);
	Info(TAG "Windows %ld.%ld.%ld (%s)", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber, os.szCSDVersion);

	GlobalMemoryStatus(&mem);
	Info(TAG "Memory: %ldKb total, %ldKb free", mem.dwTotalPhys/1024L, mem.dwAvailPhys/1024L);
}

bool system_check_multiple_instance()
{
#if !defined(BUILD_QT)
	DWORD error = 0;

	HANDLE handleProcess;
	LPCTSTR lpName = (LPCTSTR)Seed::pConfiguration->GetApplicationTitle().c_str();

	handleProcess = CreateMutex(nullptr, CREATE_MUTEX_INITIAL_OWNER, lpName);
	error = GetLastError();
	if (!handleProcess)
	{
		Log(TAG "ERROR! could not create a object mutex. Error:%d ", error);
		return true;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		HWND hWnd = FindWindowA(nullptr, Seed::pConfiguration->GetApplicationTitle().c_str());
		if (hWnd)
		{
			if (Seed::pConfiguration->GetWarningMultipleInstances())
			{
				MessageBoxA(nullptr, "There is already an instance of this application running!", Seed::pConfiguration->GetApplicationTitle().c_str(), MB_ICONWARNING);
			}
#if (_WIN32_WINNT >= 0x0500)
			SwitchToThisWindow(hWnd, false);
#endif
			ShowWindow(hWnd, SW_RESTORE);
			return false;
		}
	}
#else
	WARNING(Implementar 'system_check_multiple_instance' no Qt)
#endif
	return true;
}

#endif // WIN32
