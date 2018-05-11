#!/bin/bash
# This is a comment!
for number in {10..20..5}
do
	for i in {1..5}
	do
	./WaTor $number
	done
done
exit 0
