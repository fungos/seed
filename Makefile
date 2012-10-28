#-Wno-pointer-sign
$?BASECFLAGS=-D_THREAD_SAFE -DSDL `"$(call unixpath,$(FLASCC)/usr/bin/sdl-config)" --cflags` $(EXTRACFLAGS)
$?BUILDDIR=build
$?EXTRACFLAGS=
$?DEBUG=FALSE

include contrib.in
include seed.in
OBJECTS = $(SEED)

ifeq ("$(DEBUG)","FALSE")
	$?CFLAGS = -O4 -flto-api=exports.txt -fno-stack-protector $(EXTRACFLAGS)
	$?DEFS = -DSEED_BUILD -DSEED_ENABLE_PROFILER -DBUILD_SDL -DDEBUG
else
	$?CFLAGS = -O0 -g $(EXTRACFLAGS)
	$?DEFS = -DSEED_BUILD -DBUILD_SDL -DRELEASE
endif

$?CFLAGS += -Icontrib/ -Iinclude/ -Icontrib/flash/

T04: check $(OBJECTS)
	@echo "-------- Seed Framework --------"
	$(AS3COMPILERARGS) -abcfuture -AS3 \
		-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
		contrib/flash/Console.as -outdir . -out Console

#	"$(FLASCC)/usr/bin/g++" -O3 GameOfLife.cpp -symbol-abc=Console.abc -emit-swf -swf-size=800x600 -o seed.swf $(EXTRACFLAGS)

T04Debug: check $(OBJECTS)
	$(AS3COMPILERARGS) -abcfuture -AS3 \
	-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
	-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
	-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
	contrib/flash/Console.as -outdir . -out Console

#	"$(FLASCC)/usr/bin/g++" -O0 -g GameOfLife.cpp -symbol-abc=Console.abc -emit-swf -swf-size=800x600 -o seed.swf $(EXTRACFLAGS)

# Makefile.common
FLASCC:=X
FLEX:=X
AS3COMPILER:=asc2.jar

$?UNAME=$(shell uname -s)
ifneq (,$(findstring CYGWIN,$(UNAME)))
	$?nativepath=$(shell cygpath -at mixed $(1))
	$?unixpath=$(shell cygpath -at unix $(1))
else
	$?nativepath=$(abspath $(1))
	$?unixpath=$(abspath $(1))
endif

ifneq (,$(findstring "asc2.jar","$(AS3COMPILER)"))
	$?AS3COMPILERARGS=java $(JVMARGS) -jar $(call nativepath,$(FLASCC)/usr/lib/$(AS3COMPILER)) -merge -md 
else
	echo "ASC is no longer supported" ; exit 1 ;
endif

check:
	@if [ -d $(FLASCC)/usr/bin ] ; then true ; \
	else echo "Couldn't locate FLASCC sdk directory, please invoke make with \"make FLASCC=/path/to/FLASCC/sdk ...\"" ; exit 1 ; \
	fi

	@if [ -d "$(FLEX)/bin" ] ; then true ; \
	else echo "Couldn't locate Flex sdk directory, please invoke make with \"make FLEX=/path/to/flex  ...\"" ; exit 1 ; \
	fi

.c.o:
	"$(FLASCC)/usr/bin/gcc" $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

.cpp.o:
	"$(FLASCC)/usr/bin/g++" $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)
	
clean:
	rm -f *.swf *.bc *.abc