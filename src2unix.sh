#!/bin/sh

stripcr_inplace ()
{
    cat $1 | tr -d \\r > $1.unx_
    cat $1.unx_ > $1
    rm $1.unx_
}

for file in *.c *.h; do
    stripcr_inplace $file
done;
 
