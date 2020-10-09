let
   pkgs = import <nixpkgs> {};
in pkgs.stdenv.mkDerivation rec {
  name = "makefile-guy-dev";
  LIBCLANG_PATH="${pkgs.llvmPackages.libclang}/lib";
  buildInputs = with pkgs; [
    stdenv
    glib
    pkgconfig
    libtcod
    clang
  ];
}
