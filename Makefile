CC_ARGS_SCRIPT = $(HOME)/.vim/bundle/dein.vim/repos/github.com/xavierd/clang_complete/bin/cc_args.py

ifneq ("$(wildcard $(CC_ARGS_SCRIPT))","")
	CXX_PREFIX = $(CC_ARGS_SCRIPT)
else
	CXX_PREFIX =
endif

CXX = $(CXX_PREFIX) clang++
CXX_TIDY = clang-tidy

DISTRO=$(shell sh -c "cat /etc/*-release | grep DISTRIB_ID | sed 's/.*=//'")
CXXFLAGS = -Wall -Werror -g
LDFLAGS += -std=c++2a
LDFLAGS += $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)

BIN_NAME = core

default: run

clean:
	rm -f ./$(BIN_NAME)

build: clean
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_NAME) src/*.cpp src/**/*.cpp
	chmod +x ./$(BIN_NAME)

build-nix:
	nix-shell shell.nix --run 'make build'

run: build
	./$(BIN_NAME)

debug: build
	gdb ./$(BIN_NAME)

run-nix: build-nix
	./$(BIN_NAME)

debug-nix: build-nix
	gdb ./$(BIN_NAME)

shell:
	nix-shell shell.nix

tidy:
	$(CXX_TIDY) src/*.cpp src/**/*.cpp -- $(LDFLAGS)
