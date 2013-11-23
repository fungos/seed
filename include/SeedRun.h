#ifndef _SEEDRUN_H_
#define _SEEDRUN_H_

#if defined(EMSCRIPTEN)

#include <emscripten/emscripten.h>
template <class T>
int SeedRun(int argc, char **argv)
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
int SeedRun(int argc, char **argv)
{
	T app;
	Seed::SetGameApp(&app, argc, argv);
	@autoreleasepool
	{
		return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
	}
}

#else

template <class T>
int SeedRun(int argc, char **argv, const char *config = "app.config")
{
	T app;
	Seed::SetGameApp(&app, argc, argv, config);
	if (Seed::Initialize())
	{
		while (!pSystem->IsShuttingDown())
		{
			Seed::Update();
			pTimer->Sleep(Milliseconds(1));
		}
	}
	Seed::Shutdown();

	return EXIT_SUCCESS;
}

#endif // EMSCRIPTEN

#endif // _SEEDRUN_H_
