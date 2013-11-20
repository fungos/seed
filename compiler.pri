#
# Detect best compiler options for current platform
#

CCACHE=""
COMPXX="g++"
COMP="gcc"
FLAGSXX=""

USE_CCACHE=FALSE
USE_CLANG=FALSE

unix {
		FLAGSXX += -std=c++11

		system(which ccache):USE_CCACHE=TRUE
		system(which clang++):USE_CLANG=TRUE
}

contains(USE_CCACHE, TRUE) {
		CCACHE="ccache"
}

contains(USE_CLANG, TRUE) {
		COMPXX="clang++"
		COMP="clang"

		contains(USE_CCACHE, TRUE) {
				FLAGSXX += -Qunused-arguments # so clang+ccache works fine without spitting tons of warnings
		}
}

#TARGET_EXT = .bc
#QMAKE_EXT_OBJ = .bc
QMAKE_CXXFLAGS += $$FLAGSXX #-emit-llvm
QMAKE_CXX="$$CCACHE $$COMPXX"
QMAKE_CC="$$CCACHE $$COMP"
QMAKE_LIB=llvm-ld -link-as-library -o
#QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj
