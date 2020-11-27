let
   pkgs = import <nixpkgs> {};
in pkgs.llvmPackages.libcxxStdenv.mkDerivation rec {
  name = "makefile-guy-dev";
  LIBCLANG_PATH="${pkgs.llvmPackages.libclang}/lib";
  nativeBuildInputs = [ pkgs.pkg-config ];
  buildInputs = with pkgs; [
    llvmPackages.libcxxStdenv
    llvmPackages.libclang

    clang

    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer

    glm
    glew
    libGLU
  ];
}
