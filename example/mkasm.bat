@echo off
setlocal
path=%WES%\pilot\pilrc\debug;%WES%\pilot\asdk\bin;%path%
pilrc -H resource.h -L TRANSTEST pilrctst.rcp
pila -s pilrctst.asm
rem erase *.bin
