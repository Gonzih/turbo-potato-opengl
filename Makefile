default: run

clean:
	rm -f ./hello

build:
	clang++ -Lcurses -o hello hello.cpp
	chmod +x ./hello

build-nix:
	nix-shell shell.nix --run 'make build'

run: clean build
	./hello

run-nix: clean build-nix
	./hello

shell:
	nix-shell shell.nix
