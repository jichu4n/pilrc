@echo off
setlocal
path=c:\wes\pilot\pilrc\debug;c:\wes\pilot\asdk\bin;%path%
pilrc -H resource.h pilrctst.rcp
pila -s pilrctst.asm
erase *.bin
