CXX = clang++

DISTRO=$(shell sh -c "cat /etc/*-release | grep DISTRIB_ID | sed 's/.*=//'")
CXXFLAGS = -Wall -Werror -g
LDFLAGS += -std=c++2a
LDFLAGS += -lncursesw
LDFLAGS += $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)

BIN_NAME = core

default: run

clean: .clang_complete
	rm -f ./$(BIN_NAME)

build: clean
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BIN_NAME) src/*.cpp
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

.clang_complete:
	echo "$(CXXFLAGS) $(LDFLAGS)" > .clang_complete
