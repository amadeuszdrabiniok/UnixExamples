#!/bin/sh

#$PATH=`echo $PATH | sed 's/[^a-zA-Z0-9]*\/usr\/games//g' | sed 's/^://g`

echo $PATH | sed 's/\:*\/usr\/games//g' | sed 's/^://g'
