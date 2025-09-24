{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Build tools
    cmake
    ninja
    pkg-config
    
    # Compiler
    gcc
    gdb
    
    # Libraries  
    raylib
    nlohmann_json
    
    # Development tools
    clang-tools # per clangd se lo usi
  ];
  
  shellHook = ''
    echo "Development environment loaded"
    echo "Available libraries:"
    echo "- raylib: $(pkg-config --modversion raylib 2>/dev/null || echo "not found via pkg-config")"
    echo "- nlohmann_json: $(pkg-config --modversion nlohmann_json 2>/dev/null || echo "not found via pkg-config")"
  '';
}