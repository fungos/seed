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

/*! \file JsonReader.cpp
	\author	Danny Angelo Carminati Grein
	\brief Json Reader
*/

#include "Defines.h"

#if SEED_USE_JSON == 1

#include "api/yajl/JsonReader.h"
#include "Log.h"

#define TAG "[JsonReader] "

namespace Seed {

JsonReader::JsonReader()
{
}

JsonReader::~JsonReader()
{
	yajl_tree_free(_node);
}

bool JsonReader::Load(void *data)
{
	bool ret = false;
	char err[1024];

	yajl_tree_free(_node);
	_node = yajl_tree_parse((const char *)data, err, sizeof(err));

	if (_node != NULL)
	{
		ret = true;
	}
	else
	{
		if (strlen(err))
			Log(TAG "Error: %s", err);
		else
			Log(TAG "Error: unknown.");
	}

	return ret;
}

const char *JsonReader::ReadString(const char **path)
{
	const char *ret = NULL;

	yajl_val v = yajl_tree_get(_node, path, yajl_t_string);
	if (v && YAJL_IS_STRING(v))
		ret = YAJL_GET_STRING(v);

	return ret;
}

s32 JsonReader::ReadS32(const char **path)
{
	s32 ret = 0;

	yajl_val v = yajl_tree_get(_node, path, yajl_t_number);
	if (v && YAJL_IS_INTEGER(v))
		ret = (s32)YAJL_GET_INTEGER(v);
	else
		Log(TAG _FUNCTION_ "Error reading a s32 somewhere at %s", path[0]);

	return ret;
}

f32 JsonReader::ReadF32(const char **path)
{
	f32 ret = 0.0f;

	yajl_val v = yajl_tree_get(_node, path, yajl_t_number);
	if (v && YAJL_IS_DOUBLE(v))
		ret = (f32)YAJL_GET_DOUBLE(v);
	else
		Log(TAG _FUNCTION_ "Error reading a f32 somewhere at %s", path[0]);

	return ret;
}

bool JsonReader::ReadBool(const char **path)
{
	yajl_val v = yajl_tree_get(_node, path, yajl_t_true);
	return (v && YAJL_IS_TRUE(v));
}

}


#endif // SEED_USE_JSON
