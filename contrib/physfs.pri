INCLUDEPATH += physfs/

SOURCES += \
	physfs/physfs_unicode.c \
	physfs/physfs_byteorder.c \
	physfs/physfs.c \
	physfs/zlib123/zutil.c \
	physfs/zlib123/trees.c \
	physfs/zlib123/uncompr.c \
	physfs/zlib123/inftrees.c \
	physfs/zlib123/inflate.c \
	physfs/zlib123/deflate.c \
	physfs/zlib123/inffast.c \
	physfs/zlib123/infback.c \
	physfs/zlib123/gzio.c \
	physfs/zlib123/crc32.c \
	physfs/zlib123/compress.c \
	physfs/zlib123/adler32.c \
	physfs/platform/windows.c \
	physfs/platform/unix.c \
	physfs/platform/posix.c \
	physfs/platform/pocketpc.c \
	physfs/platform/os2.c \
	physfs/platform/macosx.c \
#	physfs/platform/beos.cpp \
	physfs/archivers/zip.c \
	physfs/archivers/wad.c \
	physfs/archivers/qpak.c \
	physfs/archivers/mvl.c \
	physfs/archivers/lzma.c \
	physfs/archivers/hog.c \
	physfs/archivers/grp.c \
	physfs/archivers/dir.c

	
HEADERS += \
	physfs/physfs_platforms.h \
	physfs/physfs_internal.h \
	physfs/physfs_casefolding.h \
	physfs/physfs.h \
	physfs/zlib123/zutil.h \
	physfs/zlib123/trees.h \
	physfs/zlib123/zlib.h \
	physfs/zlib123/zconf.h \
	physfs/zlib123/inftrees.h \
	physfs/zlib123/inflate.h \
	physfs/zlib123/inffixed.h \
	physfs/zlib123/inffast.h \
	physfs/zlib123/deflate.h \
	physfs/zlib123/crc32.h
