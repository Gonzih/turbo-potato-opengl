CXX = clang++
LDFLAGS += -lfmt $(shell pkg-config --cflags fmt)
LDFLAGS += -lncurses $(shell pkg-config --cflags ncurses)
LDFLAGS += -lspdlog $(shell pkg-config --cflags spdlog)

default: run

clean:
	rm -f ./core

build: clean
	$(CXX) $(LDFLAGS) -o core core.cpp
	chmod +x ./core

build-nix:
	nix-shell shell.nix --run 'make build'

run: build
	./core

run-nix: build-nix
	./core

shell:
	nix-shell shell.nix
