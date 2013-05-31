#include "testbase.h"

int main(int argc, char **argv)
{
	return SeedRun<TestBase>(argc, argv, "configs/app.config");
}