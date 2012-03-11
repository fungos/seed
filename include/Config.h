#ifndef __CONFIG_H__
#define __CONFIG_H__

#if defined(SEED_BUILD)
	#if !defined(SEED_USE_STATIC)
		#define SEED_BUILD_SHARED	1
		#define SEED_LICENSE		"Seed license"
	#else
		#define SEED_LICENSE		"Static"
	#endif // SEED_BUILD
#else
	#if !defined(SEED_USE_STATIC)
		#define SEED_BUILD_SHARED	1
		#define SEED_LICENSE		"Seed license"
	#else
		#define SEED_LICENSE		"Static"
	#endif
#endif

#define DANNY	"Danny Angelo Carminati Grein"
#define PATUTI	"Everton Fernando Patitucci da Silva"
#define RAPTOR	"Rafael Eduardo Gonchor"

#define SEED_TAG					"[Seed] "
#define SEED_VERSION_MINOR			0			// Developer version (features, fixes)
#define SEED_VERSION_MIDDLE			0			// Client version
#define SEED_VERSION_MAJOR			1			// Release version (final trunk)
#define SEED_VERSION_STRING			"%d.%d.%d"	//"1.1.1"
#define SEED_NAME					"Seed SDK"
#define SEED_COPYRIGHT				"Copyright (c) 2008-2009 Danny Angelo Carminati Grein\nCopyright (c) 2009 TechFront Studios"
#define SEED_AUTHORS				"Authors:\n\t" DANNY "\n\t" PATUTI "\n\t" RAPTOR

#define SEED_MESSAGE		SEED_NAME " " SEED_VERSION_STRING " [" SEED_PLATFORM_NAME " " SEED_TYPE " " SEED_LICENSE "]\n" SEED_COPYRIGHT "\n" SEED_AUTHORS

#if defined(DEBUG)
	#define SEED_TYPE "Debug"
#else
	#define SEED_TYPE "Release"
#endif // DEBUG

#if defined(BUILD_IOS)
	#define SEED_PLATFORM_NAME "iOS"
#elif defined(BUILD_SDL)
	#define SEED_PLATFORM_NAME "SDL"
#elif defined(BUILD_QT)
	#define SEED_PLATFORM_NAME "Qt"
#endif // BUILD_IOS

//================================================================================
// FEATURES CAPPING
//================================================================================
#define SEED_USE_JSON						1
#define SEED_USE_THEORA						0
#define SEED_ENABLE_DEPTH_TEST				0

/*
The size of the music buffer for streaming from hard disk on platforms that support it (SDL).
*/
#define SEED_MUSIC_STREAMING_BUFFER_SIZE	(1024 * 40)

/*
Toggle between instancing Singleton classes in the (0) Stack or in the (1) Heap
*/
#define SEED_SINGLETON_HEAP					0

/*
*/
#define SEED_MAX_PRIORITY					1000000

/*
Maximum size of a folder name string
*/
#define SEED_MAX_FOLDER_SIZE				4096

/*
Use wide char paths
*/
#if defined(WIN32)
#define SEED_PATH_WIDE						1 // For windows this is required
#else
#define SEED_PATH_WIDE						0
#endif


//================================================================================
// FEATURES SPECIFIC SETTINGS
//================================================================================


//================================================================================
// DEBUGGING DEFINES
//================================================================================
#if defined(DEBUG)
	#define SEED_LOG_RESOURCEMANAGER		1
#endif // DEBUG

#endif // __CONFIG_H__
