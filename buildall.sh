#!/bin/bash

echo Using compilers: $CXX, $CC

build()
{
	mkdir $1
	cd $1
	qmake ../../$1/$1.pro "CONFIG+=$BUILD QMAKE_CXX=$CXX QMAKE_CC=$CC" && make
	RET=$?
	cd ..
	return $RET
}

build_seed()
{
	pwd
	mkdir seed
	cd seed
	qmake ../../Seed.pro "CONFIG+=$BUILD" && make
	cp lib .. -rf
	RET=$?
	cd ..
	return $RET
}

build_samples()
{
	cd ../samples
	./buildall.sh
	RET=$?
	return $RET
}

build_games()
{
	cd ../games
	./buildall.sh
	RET=$?
	return $RET
}

if [ -d "build" ]
then
	rm build -rf
fi

git submodule update --init

mkdir build
cd build

build contrib && build_seed && build_samples && build_games
