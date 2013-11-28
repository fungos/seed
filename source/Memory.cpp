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

#include "Memory.h"
#include "Sprite.h"
#include "Image.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Frame.h"
#include "map/GameMap.h"
#include "map/MetadataObject.h"
#include "map/MapLayerMetadata.h"
#include "map/MapLayerTiled.h"
#include "map/MapLayerMosaic.h"
#include "map/TileSet.h"
#include "interface/IDataObject.h"

namespace Seed {

#ifdef DEBUG

#define SEED_CREATE_DEFINITION_NEW(type)			template <>	\
													type *SeedLogNew(type *obj, const char *stmt, const char *file, int line, const char *func) \
													{ \
														pLeakReport->LogNew((IObject *)obj, stmt, file, line, func); \
														return obj; \
													}

#define SEED_CREATE_DEFINITION_DELETE(type)			template <>	\
													void SeedLogDelete(type *ptr) \
													{ \
														pLeakReport->LogDelete((IObject *)ptr); \
														delete ptr; \
													}

#define SEED_CREATE_DEFINITION(type)				SEED_CREATE_DEFINITION_NEW(type) \
													SEED_CREATE_DEFINITION_DELETE(type)


SEED_CREATE_DEFINITION_DELETE(IDataObject)
SEED_CREATE_DEFINITION_DELETE(ISceneObject)
SEED_CREATE_DEFINITION(Sprite)
SEED_CREATE_DEFINITION(Image)
SEED_CREATE_DEFINITION(Camera)
SEED_CREATE_DEFINITION(Frame)
SEED_CREATE_DEFINITION(ParticleEmitter)
SEED_CREATE_DEFINITION(MetadataObject)
SEED_CREATE_DEFINITION(MapLayerMetadata)
SEED_CREATE_DEFINITION(MapLayerMosaic)
SEED_CREATE_DEFINITION(MapLayerTiled)
SEED_CREATE_DEFINITION(TileSet)
SEED_CREATE_DEFINITION(GameMap)

#undef SEED_CREATE_DEFINITION_DELETE
#undef SEED_CREATE_DEFINITION_NEW
#undef SEED_CREATE_DEFINITION

#endif

}
