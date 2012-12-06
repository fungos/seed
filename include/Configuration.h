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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "Defines.h"
#include "interface/IObject.h"
#include "Enum.h"
#include "Singleton.h"

namespace Seed {

/// Run-time Configuration
class SEED_CORE_API Configuration : public IObject
{
	SEED_SINGLETON_DECLARE(Configuration)
	public:
		void Load(const String &file);

		u32 GetResolutionWidth() const;
		u32 GetResolutionHeight() const;

		void SetWorkingDirectory(const String path);
		const String &GetWorkingDirectory() const;

		//! Sets the desired frame rate.
		/*!
			Sets the desired frame rate for running the entire system.
			\param frameRate Frame rate of the system
		 */
		void SetFrameRate(u32 frameRate);

		//! Gets the desired frame rate.
		/*!
			Returns the selected frame rate.
			\returns a ISystem::eFrameRate enumeration;
		 */
		u32 GetFrameRate() const;

		//! Sets the application title.
		/*!
			Sets the application title, mainly used for savegame data.
			\param title Title of the application;
		 */
		void SetApplicationTitle(const String &title);

		//! Returns the application title.
		/*!
			Returns the application title.
			\returns a const char * string;
		 */
		const String &GetApplicationTitle() const;

		//! Sets the application description.
		/*!
			Sets the application description, mainly used for savegame data.
			\param descr Description of the application ;
		 */
		void SetApplicationDescription(const String &desc);

		//! Returns the application description.
		/*!
			Returns the application description.
			\returns a const char * string;
		 */
		const String &GetApplicationDescription() const;

		//! Sets the application publisher name.
		/*!
			Sets the application publisher name, mainly used for savegame data.
			\param descr Publisher of the application ;
		 */
		void SetPublisherName(const String desc);

		//! Returns the application publisher name.
		/*!
			Returns the application publisher name.
			\returns a const char * string;
		 */
		const String &GetPublisherName() const;

		void SetCanHaveMultipleInstances(bool enable);
		bool GetCanHaveMultipleInstances() const;

		void SetWarningMultipleInstances(bool warnUser);
		bool GetWarningMultipleInstances() const;

		void SetFullScreen(bool enable);
		bool GetFullScreen() const;

		void SetRendererDeviceType(eRendererDeviceType deviceType);
		eRendererDeviceType GetRendererDeviceType() const;

		void SetReaderType(eReaderType readerType);
		eReaderType GetReaderType() const;

		// IObject
		virtual const String GetClassName() const;
		virtual int GetObjectType() const;

		bool bDebugSprite;

	private:
		SEED_DISABLE_COPY(Configuration);

		String sWorkingDirectory;
		String sTitle;
		String sDescription;
		String sPublisherName;

		eRendererDeviceType iRendererDeviceType;
		eReaderType iReaderType;

		u32 iFrameRate;
		u32 iResolutionWidth;
		u32 iResolutionHeight;

		bool bMultipleInstances;
		bool bWarningMultipleInstances;
		bool bFullScreen;
};

#define pConfiguration Configuration::GetInstance()

} // namespace

#endif // __CONFIGURATION_H__
