#!/bin/bash
# This is a comment!
for number in {10..200..5}
do
	for i in {1..20}
	do
	./WaTor $number
	done
done
exit 0
