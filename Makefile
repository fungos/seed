#-Wno-pointer-sign
BASECFLAGS = -D_THREAD_SAFE -DSDL `"$(call unixpath,$(FLASCC)/usr/bin/sdl-config)" --cflags` $(EXTRACFLAGS)
BUILDDIR = build/fla
DEBUG = TRUE

#------- GENERIC CONFIG
include contrib.in
include seed.in
include tests.in
OBJECTS = $(CONTRIB) $(SEED) $(TESTS)

ifeq ("$(DEBUG)","FALSE")
	CFLAGS += -O4 -fno-stack-protector $(EXTRACFLAGS)
	DEFS += -DSEED_ENABLE_PROFILER -DDEBUG
else
	CFLAGS += -O0 -g $(EXTRACFLAGS)
	DEFS += -DRELEASE
endif

LFLAGS = -flto-api=../../exports.txt
LIBS = -lSDL -lvgl -lGL -lm -lAS3++
CFLAGS += 
CXXFLAGS += $(CFLAGS) -fno-rtti -fno-exceptions
DEFS += -DUSE_API_NULL_OAL -DBUILD_SDL -DSEED_BUILD

$?CFLAGS += -Icontrib/ -Iinclude/ -Icontrib/flash/

default: all

#----------- FLASCC
LIBS += 
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
	
assets: check
	@echo "-------- Preparing data files --------"
	mkdir -p $(BUILDDIR)/vfs
	cd $(WORKDIR) && cp -f $(DATAFILES) $(PWD)/$(BUILDDIR)/vfs/
	chmod 777 $(BUILDDIR)/vfs/*
	cd $(BUILDDIR) && "$(FLASCC)/usr/bin/genfs" vfs vfs
	
build: check assets
	@echo "-------- Seed Framework --------"
	$(AS3COMPILERARGS) -abcfuture -AS3 \
		-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
		contrib/flash/Console.as -outdir $(BUILDDIR) -out Console

	cd $(BUILDDIR) && $(AS3COMPILERARGS) \
		-import $(call nativepath,$(FLASCC)/usr/lib/builtin.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/playerglobal.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/BinaryData.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/ISpecialFile.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/IBackingStore.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/IVFS.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/InMemoryBackingStore.abc) \
		-import $(call nativepath,$(FLASCC)/usr/lib/PlayerKernel.abc) \
		vfs*.as
		
	make -j2 link linkmt

link:
	cd $(BUILDDIR) && "$(FLASCC)/usr/bin/g++" $(CXXFLAGS) $(LFLAGS) `ls *.o` -o ../tests.swf $(LIBS) vfs*.abc $(FLASCC)/usr/lib/AlcVFSZip.abc -emit-swf -symbol-abc=Console.abc -swf-size=800x600

linkmt:
	cd $(BUILDDIR) && "$(FLASCC)/usr/bin/g++" $(CXXFLAGS) $(LFLAGS) `ls *.o` -o ../testsmt.swf $(LIBS) vfs*.abc $(FLASCC)/usr/lib/AlcVFSZip.abc -emit-swf -symbol-abc=Console.abc -swf-size=800x600 -pthread -swf-version=18
#------------- FLASCC
	
.c.o:
	"$(FLASCC)/usr/bin/gcc" $(CFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

.cpp.o:
	"$(FLASCC)/usr/bin/g++" $(CXXFLAGS) $(DEFS) $(BASECFLAGS) -c $< -o $(BUILDDIR)/$(@F)

all: contrib seed tests build
	
contrib: $(CONTRIB)

seed: $(SEED)

tests: $(TESTS)
	
clean:
	rm -f *.swf *.bc *.abc $(BUILDDIR)/*
