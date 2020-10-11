default: run

clean:
	rm -f ./core

build: clean
	clang++ -lncurses $(shell pkg-config --cflags ncurses) -o core core.cpp
	chmod +x ./core

build-nix:
	nix-shell shell.nix --run 'make build'

run: build
	./core

run-nix: build-nix
	./core

shell:
	nix-shell shell.nix
