#!/bin/bash

build()
{
	mkdir $1
	cd $1
	qmake ../../$1/$1.pro "CONFIG+=$BUILD" && make
	RET=$?
	cd ..
	return $RET
}

if [ -d "build" ]
then
	rm build -rf
fi

if [ -d "lib" ]
then
	rm lib -rf
fi

cp ../build/lib . -r
mkdir build
cd build

build 00_skel && build 01_presentation && build 02_image && build 03_pointer_input && build 04_keyboard_input && build 05_joystick_input && build 06_sprite && build 07_music_sfx && build 08_particles && build 09_tilemap && build 11_state_machine && build 12_savesystem && build 13_gui_rocket && build 14_renderer && build 15_presentation2 && build 19_box2d && build 23_net_udp_socket && build 25_prefabs && build 27_shader && build 28_simple3d

# && build 10_movie
# && build 16_renderer2
# && build 17_video
# && build 18_thread
# && build 20_box2d2
# && build 21_bullet && build 22_bullet2

