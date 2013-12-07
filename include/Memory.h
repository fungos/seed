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

#ifndef MEMORY_H
#define MEMORY_H

#include "LeakReport.h"

#if defined(DEBUG)

#define sdNew(T)				SeedLogNew((new T), #T, __FILE__, __LINE__, __FUNC__)
#define sdDelete(ptr)			{ if (ptr) SeedLogDelete(ptr); ptr = nullptr; }

#define sdNewArray(T, L)		pLeakReport->LogNew((new T[L]), #T, __FILE__, __LINE__, __FUNC__)
#define sdDeleteArray(ptr)		{ if (ptr) SeedLogDeleteArray(ptr); ptr = nullptr; }
#define sdAlloc(S)				malloc(S)
#define sdFree(ptr)				{ if (ptr) free(ptr); ptr = nullptr; }

namespace Seed {

template <class T>
T *SeedLogNew(T *obj, const char *stmt, const char *file, int line, const char *func)
{
	pLeakReport->LogNew(obj, stmt, file, line, func);
	return obj;
}

template <class T>
void SeedLogDelete(T *ptr)
{
	pLeakReport->LogDelete((void *)ptr);
	delete ptr;
}

template <class T>
void SeedLogDeleteArray(T *ptr)
{
	pLeakReport->LogDelete((void *)ptr);
	delete [] ptr;
}

#define SEED_CREATE_DECLARATION_NEW(type)			class type;							\
													template <>							\
													type *SeedLogNew(type *obj, const char *stmt, const char *file, int line, const char *func);

#define SEED_CREATE_DECLARATION_DELETE(type)		class type;							\
													template <>							\
													void SeedLogDelete(type *ptr);


#define SEED_CREATE_DECLARATION(type)				SEED_CREATE_DECLARATION_NEW(type) \
													SEED_CREATE_DECLARATION_DELETE(type)

SEED_CREATE_DECLARATION(Sprite)
SEED_CREATE_DECLARATION(Image)
SEED_CREATE_DECLARATION(ParticleEmitter)
SEED_CREATE_DECLARATION(Camera)
SEED_CREATE_DECLARATION(Frame)
SEED_CREATE_DECLARATION(IDataObject)
SEED_CREATE_DECLARATION(ISceneObject)
SEED_CREATE_DECLARATION(MetadataObject)
SEED_CREATE_DECLARATION(MapLayerMetadata)
SEED_CREATE_DECLARATION(MapLayerMosaic)
SEED_CREATE_DECLARATION(MapLayerTiled)
SEED_CREATE_DECLARATION(TileSet)
SEED_CREATE_DECLARATION(GameMap)

#undef SEED_CREATE_DECLARATION_DELETE
#undef SEED_CREATE_DECLARATION_NEW
#undef SEED_CREATE_DECLARATION


}

#else // DEBUG

#define sdNew(T)				new T
#define sdDelete(ptr)			{ if (ptr) delete ptr; ptr = nullptr; }
#define sdNewArray(T, L)		new T[L]
#define sdDeleteArray(ptr)		{ if (ptr) delete []ptr; ptr = nullptr; }
#define sdAlloc(S)				malloc(S)
#define sdFree(ptr)				{ free(ptr); ptr = nullptr;}

#endif // DEBUG

#endif // MEMORY_H
