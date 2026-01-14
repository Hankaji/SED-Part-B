project := "EMCS"

native_build := "build"
linux_build  := "build-linux"
win_build    := "build-windows"

toolchain    := "toolchain-mingw64.cmake"

default: build

build:
	cmake -S . -B {{native_build}}
	cmake --build {{native_build}}

build-all: build-linux build-windows
	@echo "Built Linux and Windows binaries"

build-linux:
	cmake -S . -B {{linux_build}}
	cmake --build {{linux_build}}

build-windows:
	cmake -S . -B {{win_build}} -DCMAKE_TOOLCHAIN_FILE={{toolchain}}
	cmake --build {{win_build}}

clean:
	rm -rf {{native_build}} {{linux_build}} {{win_build}}

list:
	just --list
