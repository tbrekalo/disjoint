.PHONY: \
	all \
	clean \
	clean-debug \
	clean-relwithdebinfo \
	clean-release \
	debug \
	relwithdebinfo \
	release

GEN:="Unix Makefiles"
NINJA_PATH:=$(shell command -v ninja 2> /dev/null)
ifdef NINJA_PATH
	GEN:="Ninja"
endif

venv:
	@python -m venv ./venv
	@bash -c 'source venv/bin/activate; pip install -e .';

venv-dev: venv
	@bash -c 'source venv/bin/activate; pip install -e .[dev]';

clean-venv:
	rm -rf ./venv

build-debug:
	cmake -B ./build-debug -DCMAKE_BUILD_TYPE=Debug -G $(GEN)

debug: build-debug
	cmake --build build-debug

clean-debug:
	rm -rf ./build-debug

build-relwithdebinfo:
	cmake -B ./build-relwithdebinfo -DCMAKE_BUILD_TYPE=RelWithDebInfo -G $(GEN)

relwithdebinfo: build-relwithdebinfo
	cmake --build build-relwithdebinfo

clean-relwithdebinfo:
	rm -rf ./build-relwithdebinfo

build:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G $(GEN)

release: build
	cmake --build build

clean-release:
	rm -rf ./build

all: debug relwithdebinfo release venv-dev
	@:

clean: clean-debug clean-relwithdebinfo clean-release clean-venv
	@:
