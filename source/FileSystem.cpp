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

#include "FileSystem.h"
#include "Log.h"
#include "File.h"
#include "System.h"
#include "SeedInit.h"
#include "physfs/physfs.h"

#define FS_CHECK(x)	{ if (!x) { const char *_err = PHYSFS_getLastError(); Log(TAG "Error: %s", _err); }}

#define TAG	"[FileSystem] "

namespace Seed {

SEED_SINGLETON_DEFINE(FileSystem)

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
	this->Shutdown();
}

bool FileSystem::Initialize()
{
	FS_CHECK(PHYSFS_init(Seed::Private::pcArgv[0]));
	FS_CHECK(PHYSFS_setSaneConfig(Seed::pConfiguration->GetPublisherName(), Seed::pConfiguration->GetApplicationTitle(), "zip", false, false));

	return true;
}

bool FileSystem::Shutdown()
{
	FS_CHECK(PHYSFS_deinit());
	return true;
}

const FilePath *FileSystem::GetWorkDirectory() const
{
	return PHYSFS_getUserDir();
}

const FilePath *FileSystem::GetWriteableDirectory() const
{
	return PHYSFS_getWriteDir();
}

bool FileSystem::IsRequired() const
{
	return true;
}

const char *FileSystem::GetObjectName() const
{
	return "FileSystem";
}

} // namespace
