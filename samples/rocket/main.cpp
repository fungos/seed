<<<<<<< HEAD
#include "rocket_sample.h"

int main(int argc, const char **argv)
{
	return SeedRun<RocketSample>(argc, argv);
}
=======
#include "rocket_sample.h"

int main(int argc, char **argv)
{
	return SeedRun<RocketSample>(argc, argv, "rocket_app.config");
}
>>>>>>> a9580b74bf80685c754dd1f7b36da15e51452b9b
