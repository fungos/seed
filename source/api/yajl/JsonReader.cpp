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
	pRootNode = NULL;
	pCurNode = NULL;
	pCurArray = NULL;
	iPos = 0;
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
	ASSERT_NULL(data);
	bool ret = false;
	char err[1024];

	//fprintf(stdout, "\n\nJSON: %s\n\n", (char *)data);
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

	u32 len = 0;
	if (YAJL_IS_ARRAY(v))
	{
		Log(TAG "Array %s found, len: %ld", key, YAJL_GET_ARRAY(v)->len);
		qStackNode.push(pCurNode);
		qStackArray.push(pCurArray);
		qStackArrayPos.push(iPos);
		pCurArray = v;
		iPos = 0;
		len = YAJL_GET_ARRAY(v)->len;
	}
	else
		Log(TAG "Array %s not found", key);

	return len;
}

void JsonReader::SelectNext()
{
	if (pCurArray && iPos < YAJL_GET_ARRAY(pCurArray)->len)
	{
		pCurNode = YAJL_GET_ARRAY(pCurArray)->values[iPos];
		iPos++;
	}
}

void JsonReader::UnselectArray()
{
	if (!qStackArray.empty())
	{
		pCurArray = qStackArray.top();
		qStackArray.pop();

		iPos = qStackArrayPos.top();
		qStackArrayPos.pop();

		if (!qStackNode.empty())
		{
			pCurNode = qStackNode.top();
			qStackNode.pop();
		}
	}
}

bool JsonReader::SelectNode(const char *key)
{
	const char *path[] = {key, (const char *)0};
	yajl_val v = yajl_tree_get(pCurNode, path, yajl_t_object);
	bool ret = false;
	if (!YAJL_IS_OBJECT(v))
	{
		Log(TAG "Node %s not found", key);
	}
	else
	{
		Log(TAG "Node %s found", key);
		qStackNode.push(pCurNode);
		pCurNode = v;
		ret = true;
	}

	return ret;
}

void JsonReader::UnselectNode()
{
	if (!qStackNode.empty())
	{
		pCurNode = qStackNode.top();
		qStackNode.pop();
	}
}

}

#endif // SEED_USE_JSON
