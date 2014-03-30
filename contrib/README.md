Some customizations are done to these libraries with intent to reduce warnings and to compile in all supported platforms without their respective build systems.

libRocket 1.2.1 bc8b334d2f477f250a885de68c9f621e297faf1c

- In the all Header.h added "#define STATIC_LIB";
- Added [#pragma GCC diagnostic ignored "-Wswitch"] in all Header.h;
- Removed Core/Python and Controls/Python as we doesn't use Python;
- Renamed Core/Stream.cpp to Core/RocketStream.cpp (qmake);
- Renamed Core/WidgetSlider.cpp to Core/RocketWidgetSlider.cpp (qmake);
- Renamed Debugger/Geometry.cpp to Debugger/DebugGeometry.cpp (qmake);
- Renamed Debugger/Plugin.cpp to Debugger/DebugPlugin.cpp (qmake);
- Renamed Debugger/SystemInterface.cpp to Debugger/DebugSystemInterface.cpp (qmake);
- BUGFIX: NULL pointer freeing on Core/FontFaceHandle.cpp:64 (~FontFaceHandle() -> delete[] i->second.bitmap_data;)

OtterUI 1.3.6.3 3de4464203a253cd5b89d36bd0bdefbe2f965fca

- (temporary) Renamed Log.cpp to otLog.cpp to avoid conflict with libRocket.
- (temporary) Renamed Vector2.cpp to otVector2.cpp to avoid conflict with libRocket.

FreeType 2.4.10

- Includes from <ft2build.h> to <freetype/ft2build.h>;
- Undefined FT_CONFIG_OPTION_USE_BZIP2 in ftoptions.h;
- Undefined FT_DEBUG_LEVEL_TRACE in ftoptions.h;
- Undefined FT_DEBUG_LEVEL_ERROR in ftoptions.h;
- Undefined FT_DEBUG_AUTOFIT in ftoptions.h;
- Undefined FT_DEBUG_MEMORY in ftoptions.h;

GLFW 2.7.7

- Include "internal.h" to "glfw/internal.h";
- Added platform.h to glfw/ root, with redirect to platform specific headers;
- Added "#define GLFW_NO_GLU 1" to glfw.h;
- Added parameter button to GLFWmousewheelfun in glfw.h and platform specific code;
- Do not use thread implementation;

SDL2

[Describe here all modifications on SDL2 needed to get working within contrib]

GLEW

PhysicsFS 2.0.3

SOIL

Box2D 2.2.1

Bullet Physics Library 2.81 r2613

- Not using OpenCL, MiniCL and DX11 related things;

vectormath

glm 0.9.5.2

yajl (custom version from https://github.com/fungos/yajl)

lua

libtheora

- Added [#pragma GCC diagnostic ignored "-Wparentheses"] to internal.h;

libogg

libvorbis

liboggz

liboggplay

- Added [#pragma GCC diagnostic ignored "-Wtype-limits"] to oggplay_private.h
