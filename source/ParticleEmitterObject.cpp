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

/*! \file ParticleEmitterObject.cpp
	\author	Danny Angelo Carminati Grein
	\brief A particle emitter configuration file
*/

#include "ParticleEmitterObject.h"
#include "Enum.h"


namespace Seed {


IResource *ParticleEmitterObjectResourceLoader(const char *filename, ResourceManager *res)
{
	ParticleEmitterObject *obj = New(ParticleEmitterObject());
	obj->Load(filename, res);

	return obj;
}

ParticleEmitterObject::ParticleEmitterObject()
	: iMemory(0)
{
}

ParticleEmitterObject::~ParticleEmitterObject()
{
	this->Unload();
}

bool ParticleEmitterObject::Unload()
{
	return TRUE;
}

bool ParticleEmitterObject::Load(const char *filename, ResourceManager *res)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pFilename = filename;
		pRes 	= res;
		iMemory = 0;

		File f(filename);
		SECURITY_CHECK(f.GetData(), "Particle Emitter couldn't be opened");
		//u8 *ptr = const_cast<u8 *>(static_cast<const u8 *>(stFile.GetData()));
		//ObjectHeader *block = NULL;
		//READ_STRUCT(block, ObjectHeader, ptr);
		//SECURITY_CHECK(seed_validate_block(&stFile, block, SPRITE_OBJECT_MAGIC, SPRITE_OBJECT_VERSION), "Invalid block header for sound.");

		iMemory += f.GetSize() + sizeof(this);
		stFile = f;

		bLoaded = TRUE;
	}

	return bLoaded;
}

const ParticleEmitterInfo *ParticleEmitterObject::GetData() const
{
	return reinterpret_cast<const ParticleEmitterInfo *>(stFile.GetData());
}

u32 ParticleEmitterObject::GetUsedMemory() const
{
	return iMemory;
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
