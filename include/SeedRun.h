#ifndef _SEEDRUN_H_
#define _SEEDRUN_H_

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

#endif // _SEEDRUN_H_
