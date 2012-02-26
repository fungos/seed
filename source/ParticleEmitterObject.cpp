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

#include "ParticleEmitterObject.h"
#include "Enum.h"

namespace Seed {

IResource *ParticleEmitterObjectResourceLoader(const String &filename, ResourceManager *res)
{
	ParticleEmitterObject *obj = New(ParticleEmitterObject());
	obj->Load(filename, res);

	return obj;
}

ParticleEmitterObject::ParticleEmitterObject()
	: IResource()
	, sInfo()
{
}

ParticleEmitterObject::~ParticleEmitterObject()
{
	this->Unload();
}

bool ParticleEmitterObject::Unload()
{
	return true;
}

bool ParticleEmitterObject::Load(const String &filename, ResourceManager *res)
{
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pRes 	= res;
		bLoaded = true;
	}

	return bLoaded;
}

const ParticleEmitterInfo *ParticleEmitterObject::GetData() const
{
	return &sInfo;
}

u32 ParticleEmitterObject::GetUsedMemory() const
{
	return sizeof(this);
}

const char *ParticleEmitterObject::GetObjectName() const
{
	return "ParticleEmitterObject";
}

int ParticleEmitterObject::GetObjectType() const
{
	return Seed::ObjectParticleEmitterObject;
}

} // namespace
