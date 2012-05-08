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

#ifndef __SEED_INIT_H__
#define __SEED_INIT_H__

#include "ResourceManager.h"
#include "interface/IGameApp.h"
#include "interface/IInput.h"
#include "interface/IRendererDevice.h"
#include "Timer.h"

extern "C" {

/// Seed SDK base namespace
/**
Everything from Seed SDK will be inside Seed namespace.
*/
namespace Seed {

	SEED_CORE_API extern ResourceManager *pResourceManager;
	SEED_CORE_API extern Configuration *pConfiguration;

	/// Set the Game Application main class.
	/**
	When creating a new Game project, you must have a class that will use IGameApp as a base class. This class will be the main
	entrypoint to your game and must be set by usign this function.

	\param app A pointer to your game application main class.
	\param argc Command line argument count.
	\param argv A pointer to the command line arguments.
	*/
	SEED_CORE_API void SetGameApp(IGameApp *app, int argc, const char **argv);

	/// Displays a output message by using IGameApp::WriteOut implementation.
	/**
	This method is a channeling point to all output messages that Seed will use. If you do not override IGameApp::WriteOut, it
	will use the default stdout output pipe.

	\param msg Pointer to a string to be displayed.
	*/
	SEED_CORE_API void WriteOut(const char *msg);

	/// Displays a error message by using IGameApp::WriteErr implementation.
	/**
	This method is a channeling point to all error messages that Seed will use. If you do not override IGameApp::WriteErr, it
	will use the default stderr output pipe.
	*/
	SEED_CORE_API void WriteErr(const char *msg);

	/// Initialize Seed
	/**
	This will initialize all Seed states and subsystems and must be called before anything and after Seed::SetGameApp.
	*/
	SEED_CORE_API bool Initialize();

	/// Update Seed
	/**
	Will update all Seed states and subsystems and will be called once per tick until a shutdown or error happens.
	*/
	SEED_CORE_API void Update();

	/// Render
	/**
	Renders the game state and will be called once per frame.
	*/
	SEED_CORE_API void Render();

	/// Shutdown Seed
	/**
	Will reset all Seed states and shutdown all subsystems. Nothing more from Seed can be used until a new Initialize happens.
	*/
	SEED_CORE_API void Shutdown();

	/// Get the version of the linked Seed
	/**
	Get the linked library version number, you can pass NULL to ignore any parameter.
	*/
	SEED_CORE_API void GetVersion(u32 *major, u32 *middle, u32 *minor);

	SEED_CORE_API int CommandLineParameter(const char **argv, int pos);
	SEED_CORE_API void CommandLineParse(int argc, const char **argv);

	/// Private states that Seed maintain, must not be accessed directly.
	namespace Private
	{
		/// A pointer to the IGameApp implementation.
		SEED_CORE_API extern IGameApp	*pApplication;

		/// Flag indicating that Seed is Initialized or not.
		SEED_CORE_API extern bool		bInitialized;

		/// Command line argument count.
		SEED_CORE_API extern int		iArgc;

		/// Command line command string.
		SEED_CORE_API extern const char	**pcArgv;

		/// Last frame time
		SEED_CORE_API extern f32 fCurrentTime;
	} // namespace

	/// iPhone platform related implementation using iPhoneOS SDK
	namespace iOS
	{
	} // namespace

	/// SDL (Win32, Win64, Linux, MacOSX) platform related implementation
	namespace SDL
	{
	} // namespace

	/// Qt platform related implementation
	namespace QT
	{
	} // namespace
} // namespace

} // extern C

#endif // __SEED_INIT_H__
