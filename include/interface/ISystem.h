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

#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "Log.h"
#include "IModule.h"
#include "IUpdatable.h"
#include "Enum.h"
#include "IEvent.h"
#include "IEventListener.h"

#include <vector>
#include <algorithm>

#define SYSTEM_RETRACE_HISTORY_MAX	64

namespace Seed {

class EventSystem;
class IEventSystemListener;

/// System interface
/**
Platform system interface.
*/
class SEED_CORE_API ISystem : public IModule, public IUpdatable
{
	typedef Vector<IEventSystemListener *>	ListenerVector;
	typedef ListenerVector::iterator		ListenerIterator;

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
			\param frameRate Retrace rate.
		 */
		virtual void WaitForRetrace(eSystemFrameRate frameRate) = 0;

		//! Get current user name
		/*!
			Get current user name.
			\returns a string containing the user name.
		*/
		virtual const FilePath *GetUsername() const;

		//! Get user home folder
		/*!
			Get current user home folder.
			\returns a string containing the user home folder.
		*/
		virtual const FilePath *GetHomeFolder() const;

		//! Get user application data folder
		/*!
			Where the user application data must be written
			\returns a string containing the user appdata folder.
		*/
		virtual const FilePath *GetApplicationDataFolder() const;

		//! Get save game folder
		/*!
			Where the save game must be written
			\returns a string containing the save game path.
		*/
		virtual const FilePath *GetSaveGameFolder() const;

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

		// IModule
		virtual bool IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

		void SendEventReset(const EventSystem *ev);
		void SendEventShutdown(const EventSystem *ev);
		void SendEventSystemMenu(const EventSystem *ev);
		void SendEventSystemDataManager(const EventSystem *ev);
		void SendEventHomeEnded(const EventSystem *ev);
		void SendEventSleep(const EventSystem *ev);
		void SendEventLanguageChanged(const EventSystem *ev);

	protected:
		ListenerVector vListeners;

		const char		*pStrAppName;
		const char		*pStrAppDescription;
		Seed::eLanguage nLanguage;

		bool			bDefaultCursorEnabled;
		u32				iRetraceIndex;
		u32				arRetraceCount[SYSTEM_RETRACE_HISTORY_MAX];

	private:
		SEED_DISABLE_COPY(ISystem);
};

} // namespace

#endif // __ISYSTEM_H__
