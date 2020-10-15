let
   pkgs = import <nixpkgs> {};
in pkgs.clangStdenv.mkDerivation rec {
  name = "makefile-guy-dev";
  LIBCLANG_PATH="${pkgs.llvmPackages.libclang}/lib";
  nativeBuildInputs = [ pkgs.pkg-config ];
  buildInputs = with pkgs; [
    clangStdenv
    clang
    ncurses
  ];
}
