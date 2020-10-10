default: run

clean:
	rm -f ./hello

build: clean
	clang++ -lncurses $(shell pkg-config --cflags ncurses) -o hello hello.cpp
	chmod +x ./hello

build-nix:
	nix-shell shell.nix --run 'make build'

run: build
	./hello

run-nix: build-nix
	./hello

shell:
	nix-shell shell.nix
