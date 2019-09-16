{ stdenv, pkgs, src } :
stdenv.mkDerivation {
  name = "scanner-button";
  version = "0.1";
  inherit src;
  preConfigure = "LD=$CC";
  makeFlags = [ "BINDIR=$(out)/bin" "CFLAGS=-Wall" ];
  nativeBuildInputs = with pkgs; [ sane-backends ];
}
