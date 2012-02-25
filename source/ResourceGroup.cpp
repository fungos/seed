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

/*! \file ResourceGroup.cpp
	\author	Rafael Eduardo Gonchor
	\brief Resource group class
*/

#include "ResourceGroup.h"


namespace Seed {


ResourceGroup::ResourceGroup()
	: queue()
	, bLoaded(false)
{
}


ResourceGroup::~ResourceGroup()
{
	queue.clear();
	std::vector<QueueItem *>().swap( queue );
}


void ResourceGroup::Add(const String &filename, Seed::eObjectType resourceType, ResourceManager *res)
{
	QueueItem *pNewItem 	= New(QueueItem());
	pNewItem->filename 		= filename;
	pNewItem->resource 		= NULL;
	pNewItem->resourceType 	= resourceType;
	pNewItem->resManager	= res;
	pNewItem->startTime		= (u32)pTimer->GetMilliseconds();
	pNewItem->erased		= false;

	queue.push_back(pNewItem);
}


bool ResourceGroup::Load()
{
	QueueIterator it 	= queue.begin();
	QueueIterator end 	= queue.end();

	for (; it != end; ++it)
	{
		QueueItem *pQueueItem = (*it);

		//already loaded
		if (pQueueItem->resource)
			continue;

		ASSERT_NULL(pQueueItem);

		IResource *res;
		res = pQueueItem->resManager->Get(pQueueItem->filename, pQueueItem->resourceType);

		pQueueItem->resource = res;
	}

	return true;
}


bool ResourceGroup::Unload()
{
	QueueIterator it 	= queue.begin();
	QueueIterator end 	= queue.end();

	for (; it != end; ++it)
	{
		QueueItem *pQueueItem = (*it);

		ASSERT_NULL(pQueueItem);
		sRelease(pQueueItem->resource);

		Delete(pQueueItem);
	}

	queue.clear();
	QueueVector().swap(queue);

	return true;
}


void ResourceGroup::SetLoaded()
{
	this->bLoaded = true;
}


bool ResourceGroup::IsLoaded() const
{
	return this->bLoaded;
}


} // namespace

