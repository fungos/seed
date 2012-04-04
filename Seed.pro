TARGET = seed
TEMPLATE = lib
INCLUDEPATH += include/ contrib/ bind/lua
win32:INCLUDEPATH += contrib/windows/

DEFINES += SEED_BUILD BUILD_SDL
unix:DEFINES += LINUX

CONFIG(debug, debug|release) {
	DESTDIR = $$(SEEDSDK)/seed/lib/debug
	DEFINES += DEBUG
} else {
	DESTDIR = $$(SEEDSDK)/seed/lib/release
	DEFINES += RELEASE
}

CONFIG += staticlib
CONFIG -= qt

SOURCES += source/Viewport.cpp \
	source/ViewManager.cpp \
	source/Updater.cpp \
	source/Timeline.cpp \
	source/Sprite.cpp \
	source/SeedInit.cpp \
	source/SceneNode.cpp \
	source/SceneManager.cpp \
	source/SaveSystem.cpp \
	source/ResourceManager.cpp \
	source/ResourceLoader.cpp \
	source/ResourceGroup.cpp \
	source/RendererManager.cpp \
	source/Renderer.cpp \
	source/Reader.cpp \
	source/Rand.cpp \
	source/Profiler.cpp \
	source/ParticleManager.cpp \
	source/ParticleEmitter.cpp \
	source/Particle.cpp \
	source/Movie.cpp \
	source/ModuleManager.cpp \
	source/LeakReport.cpp \
	source/Keyframe.cpp \
	source/Key.cpp \
	source/Image.cpp \
	source/Frame.cpp \
	source/FileSystem.cpp \
	source/File.cpp \
	source/EventSystem.cpp \
	source/EventResourceLoader.cpp \
	source/EventMovie.cpp \
	source/EventInputPointer.cpp \
	source/EventInputMotion.cpp \
	source/EventInputKeyboard.cpp \
	source/EventInputJoystick.cpp \
	source/EventFileSystem.cpp \
	source/Configuration.cpp \
	source/Checksum.cpp \
	source/Animation.cpp \
	source/api/directx/DirectXVersion.cpp \
	source/api/directx/D3D8RendererDevice.cpp \
	source/interface/IVideo.cpp \
	source/interface/IUpdatable.cpp \
	source/interface/ITransformable.cpp \
	source/interface/ITimer.cpp \
	source/interface/IThread.cpp \
	source/interface/ITexture.cpp \
	source/interface/ISystem.cpp \
	source/interface/ISoundSystem.cpp \
	source/interface/ISoundSource.cpp \
	source/interface/ISound.cpp \
	source/interface/IScreen.cpp \
	source/interface/ISceneObject.cpp \
	source/interface/IResource.cpp \
	source/interface/IRendererDevice.cpp \
	source/interface/IRenderable.cpp \
	source/interface/IReader.cpp \
	source/interface/IObject.cpp \
	source/interface/IMutex.cpp \
	source/interface/IMusic.cpp \
	source/interface/IModule.cpp \
	source/interface/IInputPointer.cpp \
	source/interface/IInputMotion.cpp \
	source/interface/IInputKeyboard.cpp \
	source/interface/IInputJoystick.cpp \
	source/interface/IInput.cpp \
	source/interface/IGameApp.cpp \
	source/interface/IEventSystemListener.cpp \
	source/interface/IEventResourceLoaderListener.cpp \
	source/interface/IEventMovieListener.cpp \
	source/interface/IEventListener.cpp \
	source/interface/IEventInputPointerListener.cpp \
	source/interface/IEventInputMotionListener.cpp \
	source/interface/IEventInputKeyboardListener.cpp \
	source/interface/IEventInputJoystickListener.cpp \
	source/interface/IEventInput.cpp \
	source/interface/IEventFileSystemListener.cpp \
	source/interface/IEvent.cpp \
	source/interface/ICartridge.cpp \
	source/interface/IBasicMesh.cpp \
	source/api/oal/vorbis_util.cpp \
	source/api/oal/oalSoundSystem.cpp \
	source/api/oal/oalSoundSource.cpp \
	source/api/oal/oalSound.cpp \
	source/api/oal/oalMusic.cpp \
	source/api/ogl/oglES1RendererDevice.cpp \
	source/api/theora/Theora.cpp \
	source/api/yajl/JsonReader.cpp \
	source/platform/ios/iosTimer.cpp \
	source/platform/ios/iosThread.cpp \
	source/platform/ios/iosTexture.cpp \
	source/platform/ios/iosSystem.cpp \
	source/platform/ios/iosSoundSystem.cpp \
	source/platform/ios/iosSoundSource.cpp \
	source/platform/ios/iosSound.cpp \
	source/platform/ios/iosScreen.cpp \
	source/platform/ios/iosMutex.cpp \
	source/platform/ios/iosMusic.cpp \
	source/platform/ios/iosInput.cpp \
	source/platform/pc/win32.cpp \
	source/platform/pc/posix.cpp \
	source/platform/pc/pcRendererDevice.cpp \
	source/platform/pc/pcLog.cpp \
	source/platform/pc/pcCartridge.cpp \
	source/platform/qt/qtTimer.cpp \
	source/platform/qt/qtThread.cpp \
	source/platform/qt/qtTexture.cpp \
	source/platform/qt/qtSystem.cpp \
	source/platform/qt/qtSurface.cpp \
	source/platform/qt/qtScreen.cpp \
	source/platform/qt/qtScene.cpp \
	source/platform/qt/qtMutex.cpp \
	source/platform/qt/qtInput.cpp \
	source/platform/sdl/sdlTimer.cpp \
	source/platform/sdl/sdlThread.cpp \
	source/platform/sdl/sdlTexture.cpp \
	source/platform/sdl/sdlSystem.cpp \
	source/platform/sdl/sdlScreen.cpp \
	source/platform/sdl/sdlMutex.cpp \
	source/platform/sdl/sdlInput.cpp

OTHER_FILES += \
	source/platform/ios/IphoneView.mm

HEADERS += include/*.h \
	include/platform/sdl/*.h \
	include/platform/qt/*.h \
	include/platform/pc/*.h \
	include/platform/ios/*.h \
	include/interface/*.h \
	include/api/directx/*.h \
	include/api/oal/*.h \
	include/api/ogl/*.h \
	include/api/theora/*.h \
	include/api/yajl/*.h
