#!/bin/sh

stripcr_inplace ()
{
    mv $1 $1.dos
    cat $1.dos | tr -d \\r > $1
    chmod --reference=$1.dos $1
}

for file in *.c *.h; do
    stripcr_inplace $file
done;
 
