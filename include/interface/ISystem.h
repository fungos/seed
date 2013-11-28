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

#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "Log.h"
#include "IManager.h"
#include "IUpdatable.h"
#include "Enum.h"
#include "IEvent.h"
#include "IEventListener.h"
#include "Container.h"
#include "Timer.h"

namespace Seed {

class EventSystem;
class IEventSystemListener;

/// System interface
/**
Platform system interface.
*/
class SEED_CORE_API ISystem : public IManager, public IUpdatable
{
	typedef Vector<IEventSystemListener *>	ListenerVector;
	typedef ListenerVector::iterator		ListenerIterator;
	SEED_DISABLE_COPY(ISystem)

	public:
		ISystem();
		virtual ~ISystem();

		//! Goes to menu if available.
		/*!
			Unloads application and goes back to SO on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void GoToMenu() = 0;

		//! Disables calls to the home menu.
		/*!
			Disables calls to menu, this is usually done to avoid calls to it while a loading is in progress.
		 */
		virtual void DisableHome() = 0;

		//! Enables calls to the home menu.
		/*!
			Calls to menu are enabled by default.
		 */
		virtual void EnableHome() = 0;

		//! Returns if home calls are enabled.
		/*!
			Returns if home calls are enabled or not.
		 */
		virtual bool IsHomeEnabled() const = 0;

		//! Returns if home is currently running.
		/*!
			Returns if home is running or not.
		 */
		virtual bool IsHomeRunning() const = 0;

		//! Initializes the home button class.
		/*!
			Initializes the home button class.
		 */
		virtual bool InitializeHome() = 0;

		//! Execute before opening home screen.
		/*!
			Execute before opening home screen.
		 */
		virtual void OnHomeCalled() = 0;

		//! Goes to the system data manager if available.
		/*!
			Unloads application and goes back to SO on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void GoToDataManager() = 0;

		//! Performs a system lockup.
		/*!
			When a fatal error occurs and not even a reset is guaranteed, the system should be put on a
			locked state and wait for a hardware reset.
		 */
		virtual void HangUp() = 0;

		//! Check shutdown.
		/*!
			Checks whatever system is shutting down.

			\return true if system is shutting down, false otherwise.
		 */
		virtual bool IsShuttingDown() const = 0;

		//! Check reset.
		/*!
			Checks whatever system is resetting.

			\return true if system is resetting, false otherwise.
		 */
		virtual bool IsResetting() const = 0;

		//! Activate sleep/standby mode.
		/*!
			Activate sleep/standby freature on platforms that supports it. If the platform doesn't
			support, you can implement it anyway you wan't or keep it empty.
		 */
		virtual void Sleep() = 0;

		//! Check sleep/standby mode.
		/*!
			Checks whatever system is in sleep/standby mode.

			\return true if system is in sleep/standby mode, false otherwise.
		 */
		virtual bool IsSleeping() const = 0;

		//! Wait for retrace.
		/*!
			Will make sure that the system doesn't update more than the configured framerate.
		 */
		virtual void WaitForRetrace() = 0;

		//! Get current user name
		/*!
			Get current user name.
			\returns a string containing the user name.
		*/
		virtual const char *GetUsername() const;

		//! Get user home folder
		/*!
			Get current user home folder.
			\returns a string containing the user home folder.
		*/
		virtual const char *GetHomeFolder() const;

		//! Get user application data folder
		/*!
			Where the user application data must be written
			\returns a string containing the user appdata folder.
		*/
		virtual const char *GetApplicationDataFolder() const;

		//! Get save game folder
		/*!
			Where the save game must be written
			\returns a string containing the save game path.
		*/
		virtual const char *GetSaveGameFolder() const;

		//! Enable/Disable default system cursor.
		/*!
			Enable/Disable default system cursor.
			\param b Enable?
		 */
		virtual void EnableDefaultCursor(bool b);

		//! Returns if home calls are enabled.
		/*!
			Returns if home calls are enabled or not.
		 */
		virtual bool IsDefaultCursorEnabled() const;

		//! Set the system language.
		/*!
			Set/changes the system current language.
		 */
		void SetLanguage(Seed::eLanguage lang);

		//! Get current system language.
		/*!
			Get current system language.
			\returns the language being used.
		 */
		Seed::eLanguage GetLanguage() const;

		//! Get current system language.
		/*!
			Get current system language.
			\returns a string representing the language being used.
		 */
		const char *GetLanguageString() const;

		/// Listeners pattern methods
		void AddListener(IEventSystemListener *listener);
		void RemoveListener(IEventSystemListener *listener);

		// IManager
		virtual bool IsRequired() const;

		void SendEventReset(const EventSystem *ev);
		void SendEventShutdown(const EventSystem *ev);
		void SendEventSystemMenu(const EventSystem *ev);
		void SendEventSystemDataManager(const EventSystem *ev);
		void SendEventHomeEnded(const EventSystem *ev);
		void SendEventSleep(const EventSystem *ev);
		void SendEventLanguageChanged(const EventSystem *ev);

		const Timer *GetTimer() const
		{
			return &mTimer;
		}

	protected:
		Timer			mTimer;
		ListenerVector	vListeners;

		const char		*pStrAppName;
		const char		*pStrAppDescription;
		Seed::eLanguage	nLanguage;
		bool			bDefaultCursorEnabled : 1;
};

} // namespace

#define pTimer pSystem->GetTimer()

#endif // __ISYSTEM_H__
