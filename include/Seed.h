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

#ifndef __SEED_H__
#define __SEED_H__

#include <Defines.h>
#include <Enum.h>

#include <MathUtil.h>
#include <Log.h>
#include <Profiler.h>

#include <Array.h>

#include <Cartridge.h>
#include <Checksum.h>

#include <EventInputMotion.h>
#include <EventInputPointer.h>
#include <EventInputKeyboard.h>
#include <EventInputJoystick.h>
#include <EventFileSystem.h>
#include <EventResourceLoader.h>
#include <EventSystem.h>
#include <EventMovie.h>

#include <File.h>
#include <FileSystem.h>

#include <Image.h>
#include <Input.h>
#include <Texture.h>

#include <LeakReport.h>

#include <interface/IEventInputMotionListener.h>
#include <interface/IEventInputKeyboardListener.h>
#include <interface/IEventInputPointerListener.h>
#include <interface/IEventInputJoystickListener.h>
#include <interface/IEventResourceLoaderListener.h>
#include <interface/IEventSystemListener.h>
#include <interface/IEventFileSystemListener.h>
#include <interface/IEventMovieListener.h>
#include <interface/IObject.h>
#include <interface/ISceneObject.h>

#include <Music.h>
#include <Mutex.h>

#include <Number.h>

#include <ParticleEmitter.h>
#include <ParticleManager.h>
#include <Point.h>

#include <Rand.h>
#include <ResourceManager.h>
#include <ResourceLoader.h>
#include <ResourceGroup.h>
#include <Renderer.h>
#include <RendererDevice.h>

#include <SaveSystem.h>
#include <SceneNode.h>
#include <Screen.h>
#include <Sound.h>
#include <SoundSystem.h>
#include <SoundSource.h>
#include <StateMachine.h>
#include <Sprite.h>
#include <SceneManager.h>
#include <System.h>
#include <Updater.h>

#include <Thread.h>
#include <Timer.h>

#include <Timeline.h>
#include <Movie.h>

#include <Viewport.h>
#include <ViewManager.h>
#include <RendererManager.h>

#include <SeedInit.h>
#include <SeedRun.h>

#if SEED_USE_THEORA == 1
#include <api/theora/Theora.h>
#endif

#if defined(BUILD_SDL) && defined(WIN32)
#undef main
#endif

namespace Seed {}

#endif // __SEED_H__
