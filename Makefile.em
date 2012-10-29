include contrib.in
include seed.in
BUILDDIR = embuild
OBJECTS = $(CONTRIB) $(SEED)
DEFS = -DSEED_BUILD -DSEED_ENABLE_PROFILER -DBUILD_SDL -DDEBUG -DEMSCRIPTEN
CFLAGS += -Icontrib/ -Iinclude/
CC=emcc
CXX=em++

.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

.cpp.o:
	$(CXX) $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

all: $(OBJECTS)
	
clean:
	rm -f *.o *.bc
