@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
cl.exe hello_world.c

echo --- *** ---

hello_world.exe

pause