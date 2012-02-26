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

#ifndef __ISOUND_H__
#define __ISOUND_H__

#include "IResource.h"
#include "ResourceManager.h"

namespace Seed {

/// Sound interface
class SEED_CORE_API ISound : public IResource
{
	public:
		ISound();
		virtual ~ISound();

		virtual const void *GetData() const = 0;

		// IResource
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager) = 0;
		virtual bool Unload() = 0;

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(ISound);
};

} // namespace

#endif // __ISOUND_H__
