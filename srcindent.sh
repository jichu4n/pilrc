#!/bin/sh

for file in *.c *.h; do
    indent $file
done;
rm *~
 
