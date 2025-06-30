{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    raylib
    pkg-config
  ];

  #shellHook = "nohup clion >/dev/null 2>&1 &";
}
