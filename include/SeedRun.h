#ifndef _SEEDRUN_H_
#define _SEEDRUN_H_

#if defined(EMSCRIPTEN)

#include <emscripten/emscripten.h>
template <class T>
int SeedRun(int argc, const char **argv)
{
	T app;
	Seed::SetGameApp(&app, argc, argv);
	if (Seed::Initialize())
		emscripten_set_main_loop(Seed::Update, 60, true);

	return EXIT_SUCCESS;
}

#elif defined(BUILD_IOS)
#include <platform/ios/iosView.h>

template <class T>
int SeedRun(int argc, const char **argv)
{
	T app;
	@autoreleasepool
	{
		return UIApplicationMain(argc, const_cast<char **>(argv), nil, NSStringFromClass([AppDelegate class]));
	}
}

#else

template <class T>
int SeedRun(int argc, const char **argv)
{
	T app;
	Seed::SetGameApp(&app, argc, argv);
	if (Seed::Initialize())
	{
		while (!pSystem->IsShuttingDown())
		{
			Seed::Update();
			pTimer->Sleep(1);
		}
	}
	Seed::Shutdown();

	return EXIT_SUCCESS;
}

#endif // EMSCRIPTEN

#endif // _SEEDRUN_H_
