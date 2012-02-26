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

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "interface/IResource.h"
#include "Enum.h"

#include <map>

namespace Seed {

typedef IResource *(*pResourceLoaderFunc)(const String &filename, ResourceManager *res);

/// Debugging util
struct FakeMap
{
	String sKey;
	void *pValue;
};

class SEED_CORE_API ResourceManager
{
	public:
		ResourceManager(const String &name);
		~ResourceManager();

		void Reset();
		IResource *Get(const String &filename, Seed::eObjectType resourceType = Seed::ObjectTexture);
		void GarbageCollect();

		u32 GetTotalUsedMemory();
		void PrintUsedMemoryByResource();

		static void Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc);
		static void Unregister(Seed::eObjectType resourceType);

		void Unload(Seed::eObjectType resourceType);
		void Reload(Seed::eObjectType resourceType);
		void Print();

	private:
		SEED_DISABLE_COPY(ResourceManager);

		void Add(const String &filename, IResource *res);
		void Remove(const String &filename);

	private:
		bool bHasUnusedResource;
		String sName;

		FakeMap mapResources[100];
		static void *mapLoaders[ObjectUser];
};

} // namespace

#endif // __RESOURCE_MANAGER_H__
