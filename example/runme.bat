@setlocal
path=%WES%\pilot\pilrc\debug;c:\bin\;c:\gcc\palm\bin;.;%PATH%
mkdir \tmp
mkdir \temp
set GCC_EXEC_PREFIX=//c/gcc/palm/lib/gcc-lib/
echo "Now type make"
nmake -f makefile
erase *.res
erase resource.h
