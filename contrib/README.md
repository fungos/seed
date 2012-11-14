Some customizations are done to these libraries with intent to reduce warning and compile in all supported platforms without their respective build systems.

libRocket 1.2.1 bc8b334d2f477f250a885de68c9f621e297faf1c

- In the all Header.h added "#define STATIC_LIB";
- Added [#pragma GCC diagnostic ignored "-Wswitch"] in all Header.h;
- Replace all "dynamic_cast" to "static_cast" because of -fno-rtti;
- Removed Core/Python and Controls/Python as we doesn't use Python;
- Removed use of typeid;
- Renamed Core/Stream.cpp to Core/RocketStream.cpp (qmake);
- Renamed Core/WidgetSlider.cpp to Core/RocketWidgetSlider.cpp (qmake);
- Renamed Debugger/Geometry.cpp to Debugger/DebugGeometry.cpp (qmake);
- Renamed Debugger/Plugin.cpp to Debugger/DebugPlugin.cpp (qmake);
- Renamed Debugger/SystemInterface.cpp to Debugger/DebugSystemInterface.cpp (qmake);

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

GLEW

PhysicsFS

SOIL

Box2D

vectormath

yajl

lua

libtheora

- Added [#pragma GCC diagnostic ignored "-Wparentheses"] to internal.h;

libogg

libvorbis

liboggz

liboggplay
