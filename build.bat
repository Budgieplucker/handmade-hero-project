@echo off

mkdir build
pushd build
cl -Zi ..\src\handmade.c user32.lib
popd