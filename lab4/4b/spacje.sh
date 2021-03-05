#!/bin/sh

ls | while read name
do
	newname=`echo "$name" | sed 's/ * /_/g'`
	echo "$name" "$newname"
done
