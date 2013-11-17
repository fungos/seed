TARGET = seed
TEMPLATE = lib

INCLUDEPATH += include/ contrib/
DEFINES += SEED_BUILD SEED_ENABLE_PROFILER

CONFIG += glfw
CONFIG += staticlib
#CONFIG += check

CONFIG(debug, debug|release) {
	DESTDIR =../seed/lib/debug
} else {
	DESTDIR = ../seed/lib/release
}


include(compiler.pri)
include(platform.pri)

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
	source/ParticleEmitter.cpp \
	source/Particle.cpp \
	source/Movie.cpp \
	source/Manager.cpp \
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
	source/interface/ITransformable.cpp \
	source/interface/IThread.cpp \
	source/interface/ITexture.cpp \
	source/interface/ISystem.cpp \
	source/interface/ISoundSystem.cpp \
	source/interface/ISoundSource.cpp \
	source/interface/IScreen.cpp \
	source/interface/ISceneObject.cpp \
	source/interface/IResource.cpp \
	source/interface/IRendererDevice.cpp \
	source/interface/IRenderable.cpp \
	source/interface/IReader.cpp \
	source/interface/IMusic.cpp \
	source/interface/IInputPointer.cpp \
	source/interface/IInputMotion.cpp \
	source/interface/IInputKeyboard.cpp \
	source/interface/IInputJoystick.cpp \
	source/interface/IGameApp.cpp \
	source/interface/ICartridge.cpp \
	source/api/oal/vorbis_util.cpp \
	source/api/oal/oalSoundSystem.cpp \
	source/api/oal/oalSoundSource.cpp \
	source/api/oal/oalSound.cpp \
	source/api/oal/oalMusic.cpp \
	source/api/nullal/nalSoundSystem.cpp \
	source/api/nullal/nalSoundSource.cpp \
	source/api/nullal/nalSound.cpp \
	source/api/nullal/nalMusic.cpp \
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
	source/platform/pc/pcRendererDevice.cpp \
	source/platform/pc/pcLog.cpp \
	source/platform/pc/pcCartridge.cpp \
	source/platform/qt/qtTimer.cpp \
	source/platform/qt/qtThread.cpp \
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
	source/platform/sdl/sdlInput.cpp \
	source/api/yajl/JsonWriter.cpp \
	source/api/net/Address.cpp \
	source/api/net/Socket.cpp \
	source/Writer.cpp \
	source/interface/IWriter.cpp \
	source/SceneObjectFactory.cpp \
	source/Camera.cpp \
	source/platform/glfw/glfwTimer.cpp \
	source/platform/glfw/glfwThread.cpp \
	source/platform/glfw/glfwSystem.cpp \
	source/platform/glfw/glfwScreen.cpp \
	source/platform/glfw/glfwMutex.cpp \
	source/platform/glfw/glfwInput.cpp \
	source/api/soil/soilTexture.cpp \
	source/platform/pc/seed_win32.cpp \
	source/platform/pc/seed_posix.cpp \
	source/JobManager.cpp \
	source/Job.cpp \
	source/ThreadManager.cpp \
	source/api/rocket/RocketInterface.cpp \
	source/Presentation.cpp \
	source/EventPresentation.cpp \
	source/map/GameMap.cpp \
	source/map/IMapLayer.cpp \
	source/map/MapLayerMetadata.cpp \
	source/map/MapLayerMosaic.cpp \
	source/map/MapLayerTiled.cpp \
	source/map/TileSet.cpp \
	source/interface/IManager.cpp \
    source/PrefabManager.cpp \
    source/map/MetadataObject.cpp \
    source/Memory.cpp

OTHER_FILES += \
	source/platform/ios/iosView.mm

HEADERS += include/*.h \
	include/platform/sdl/*.h \
	include/platform/qt/qtSurface.h \
	include/platform/qt/qtScene.h \
	include/platform/qt/qtDefines.h \
	include/platform/qt/qtInput.h \
	include/platform/qt/qtMutex.h \
	include/platform/qt/qtPch.h \
	include/platform/qt/qtScreen.h \
	include/platform/qt/qtSystem.h \
	include/platform/qt/qtThread.h \
	include/platform/qt/qtTimer.h \
	include/platform/pc/*.h \
	include/platform/ios/*.h \
	include/interface/*.h \
	include/api/directx/*.h \
	include/api/oal/*.h \
	include/api/nullal/*.h \
	include/api/ogl/*.h \
	include/api/theora/*.h \
	include/api/yajl/*.h \
	include/api/net/*.h \
	include/Writer.h \
	include/interface/IWriter.h \
	include/api/yajl/JsonWriter.h \
	include/interface/IDataObject.h \
	include/SceneObjectFactory.h \
	include/Camera.h \
	include/platform/glfw/glfwTimer.h \
	include/platform/glfw/glfwThread.h \
	include/platform/glfw/glfwSystem.h \
	include/platform/glfw/glfwScreen.h \
	include/platform/glfw/glfwMutex.h \
	include/platform/glfw/glfwInput.h \
	include/platform/glfw/glfwDefines.h \
	include/api/soil/soilTexture.h \
	include/JobManager.h \
	include/Job.h \
	include/ThreadManager.h \
	include/api/rocket/RocketInterface.h \
	include/interface/IHardwareBuffer.h \
	include/Presentation.h \
	include/EventPresentation.h \
	include/map/GameMap.h \
	include/map/IMapLayer.h \
	include/map/MapLayerMetadata.h \
	include/map/MapLayerMosaic.h \
	include/map/MapLayerTiled.h \
	include/map/TileSet.h \
    include/PrefabManager.h \
    include/map/MetadataObject.h \
    include/Memory.h
