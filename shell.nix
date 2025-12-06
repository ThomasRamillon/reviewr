{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  # other packages
  packages = with pkgs; [];
  # tools
  nativeBuildInputs = with pkgs; [];
  # libs
  buildInputs = with pkgs; [
    gcc
    gnumake
    nlohmann_json
  ];
  
  shellHook = ''echo 'shell on'
  '';
}