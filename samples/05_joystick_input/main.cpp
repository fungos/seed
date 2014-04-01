#include "joystick_sample.h"

int main(int argc, char **argv)
{
	return SeedRun<JoystickSample>(argc, argv, "joystick_app.config");
}
