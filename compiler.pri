#
# Detect best compiler options for current platform
#
FLAGSXX += -std=c++11
USE_CCACHE=FALSE

USE_TRAVIS=$$(TRAVIS)
!isEmpty(USE_TRAVIS) {
	QMAKE_CXX=$$(CXX)
	QMAKE_CC=$$(CC)
	message(Running on travis-ci.org using $$(CXX) and $$(CC))
	unix:!macx {
		#FLAGSXX += -stdlib=libstdc++
	}

	macx {
		FLAGSXX += -stdlib=libc++
	}
} else {
	CCACHE=""
	COMPXX="g++"
	COMP="gcc"

	USE_CLANG=FALSE

	unix:!macx {
		FLAGSXX += -stdlib=libstdc++

		system(which ccache):USE_CCACHE=TRUE
		system(which clang++):USE_CLANG=TRUE
	}

	macx {
		FLAGSXX += -stdlib=libc++

		system(which ccache):USE_CCACHE=TRUE
		system(which clang++):USE_CLANG=TRUE
	}

	contains(USE_CCACHE, TRUE) {
		!check {
			CCACHE="ccache"
		}
	}

	contains(USE_CLANG, TRUE) {
		COMPXX=clang++
		COMP=clang
		contains(USE_CCACHE, TRUE) {
			FLAGSXX += -Qunused-arguments # so clang+ccache works fine without spitting tons of warnings
			check {
				FLAGSXX += -fsanitize=undefined
			}
		}
	}
	QMAKE_CXX="$$CCACHE $$COMPXX"
	QMAKE_CC="$$CCACHE $$COMP"
}

#TARGET_EXT = .bc
#QMAKE_EXT_OBJ = .bc
QMAKE_CXXFLAGS += $$FLAGSXX #-emit-llvm
QMAKE_CFLAGS_RELEASE += $$FLAGSXX
#QMAKE_LFLAGS += -stdlib=libc++
#QMAKE_LIB=llvm-ld -link-as-library -o
#QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj

#message($$FLAGSXX)
#message($$QMAKE_CXX)
