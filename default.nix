{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    nlohmann_json
    raylib
    pkg-config
  ];

 # shellHook = "nohup clion . >/dev/null 2>&1 &";
}
