EMSCRIPTEN = /Users/tquatro/Dev/emscripten
CRUNCH_QUALITY = 64

default: all

include contrib.in
include seed.in
include tests.in

BUILDDIR = embuild
DEFS = -DSEED_BUILD -DSEED_ENABLE_PROFILER -DBUILD_SDL -DDEBUG -DEMSCRIPTEN -DUSE_API_NULL_OAL
CFLAGS += -fno-rtti -fno-exceptions -Icontrib/ -Iinclude/
CC=emcc
CXX=em++

.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

.cpp.o:
	$(CXX) $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

all: contrib seed tests

contrib: $(CONTRIB)

seed: $(SEED)

tests: $(TESTS)
	cd tests/bin ; python $(EMSCRIPTEN)/tools/file_packager.py base.data --preload $(DATAFILES) --pre-run > preload.js
	$(CC) `find embuild -iname *.o` -o index.html --pre-js preload.js -s DISABLE_EXCEPTION_CATCHING=1 -s EXPORTED_FUNCTIONS="['_main']" # -O2 -s PROFILE_MAIN_LOOP=1

clean:
	rm -f embuild/*
