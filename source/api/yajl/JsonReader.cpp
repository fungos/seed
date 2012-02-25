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
	: pRootNode(NULL)
	, pCurNode(NULL)
	, pCurArray(NULL)
	, iPos(0)
{
}

JsonReader::~JsonReader()
{
	yajl_tree_free(pRootNode);
}

JsonReader::JsonReader(const JsonReader &other)
{
	pRootNode = NULL;
	pCurNode = other.pCurNode;
	pCurArray = other.pCurArray;
	iPos = other.iPos;
}

JsonReader::JsonReader(const yajl_val node)
{
	pRootNode = NULL;
	pCurNode = node;
	pCurArray = NULL;
	iPos = 0;
}

JsonReader &JsonReader::operator=(const JsonReader &other)
{
	if (this != &other)
	{
		pRootNode = NULL;
		pCurNode = other.pCurNode;
		pCurArray = other.pCurArray;
		iPos = other.iPos;
	}

	return *this;
}

bool JsonReader::Load(const void *data)
{
	bool ret = false;
	char err[1024];

	yajl_tree_free(pRootNode);
	pCurNode = pRootNode = yajl_tree_parse((const char *)data, err, sizeof(err));

	if (pCurNode != NULL)
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

bool JsonReader::Load(const IReader &reader)
{
	bool ret = false;

	yajl_tree_free(pRootNode);
	pCurNode = static_cast<const JsonReader *>(&reader)->pCurNode;
	pCurArray = static_cast<const JsonReader *>(&reader)->pCurArray;
	iPos = static_cast<const JsonReader *>(&reader)->iPos;

	if (pCurNode != NULL)
		ret = true;

	return ret;
}

const char *JsonReader::ReadString(const char *key, const char *value) const
{
	const char *ret = value;

	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_string);
	if (v && YAJL_IS_STRING(v))
		ret = YAJL_GET_STRING(v);

	return ret;
}

u32 JsonReader::ReadU32(const char *key, u32 value) const
{
	u32 ret = value;

	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_number);
	if (v && YAJL_IS_INTEGER(v))
		ret = YAJL_GET_INTEGER(v);
	else
		Log(TAG "Error reading a u32 for key %s", key);

	return ret;
}

s32 JsonReader::ReadS32(const char *key, s32 value) const
{
	s32 ret = value;

	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_number);
	if (v && YAJL_IS_INTEGER(v))
		ret = (s32)YAJL_GET_INTEGER(v);
	else
		Log(TAG "Error reading a s32 for key %s", key);

	return ret;
}

f32 JsonReader::ReadF32(const char *key, f32 value) const
{
	f32 ret = value;

	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_number);
	if (v && YAJL_IS_DOUBLE(v))
		ret = (f32)YAJL_GET_DOUBLE(v);
	else
		Log(TAG "Error reading a f32 for key %s", key);

	return ret;
}

bool JsonReader::ReadBool(const char *key, bool value) const
{
	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_true);

	bool ret = value;
	if (v)
		ret = YAJL_IS_TRUE(v);

	return ret;
}

u32 JsonReader::SelectArray(const char *key)
{
	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_array);

	if (YAJL_IS_ARRAY(v))
	{
		Log(TAG "Array %s found, len: %ld", key, YAJL_GET_ARRAY(v)->len);
		pCurArray = v;
		iPos = 0;
	}
	else
		Log(TAG "Array %s not found", key);

	return YAJL_GET_ARRAY(v)->len;
}

void JsonReader::SelectNext()
{
	if (pCurArray && iPos < YAJL_GET_ARRAY(pCurArray)->len)
	{
		pCurNode = YAJL_GET_ARRAY(pCurArray)->values[iPos];
		iPos++;
	}
}

void JsonReader::SelectNode(const char *key)
{
	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_object);

	if (!YAJL_IS_OBJECT(v))
	{
		Log(TAG "Node %s not found", key);
	}
	else
	{
		Log(TAG "Node %s found", key);
		pCurNode = v;
	}
}

void JsonReader::Unselect()
{
	pCurNode = pRootNode;
}

}

#endif // SEED_USE_JSON
