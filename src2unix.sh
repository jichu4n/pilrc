#!/bin/sh

stripcr_inplace ()
{
#  cat $1 | tr -d '\r' > $1.unx_
#  cat $1.unx_ > $1
#  rm $1.unx_
   tr -d '\r' < "$1" > "$1.unx_"
   touch -r "$1" "$1.unx_"
   mv -f "$1.unx_" "$1"
}

for file in *.c *.h *.spec *.dsp doc/*.html; do
    stripcr_inplace $file
done;
 