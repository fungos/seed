#include "particle_sample.h"

int main(int argc, char **argv)
{
	return SeedRun<ParticleSample>(argc, argv, "particle_app.config");
}
