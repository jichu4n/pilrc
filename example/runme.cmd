@setlocal
PATH=c:\wes\pilot\pilrc\debug;\bin\;c:\cygnus\H-i386-cygwin32\bin;.;%PATH%
mkdir \tmp
mkdir \temp
set GCC_EXEC_PREFIX=//c/cygnus/H-i386-cygwin32/lib/gcc-lib/

echo "Now type make"
bash
