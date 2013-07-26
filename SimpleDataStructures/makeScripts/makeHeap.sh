#!/bin/bash
cd $(dirname $0)
gcc -Og -I ../../ -c -g ../SimpleHeap/SimpleHeap.c -o ../objectFiles/SimpleHeap.og &&
gcc -O3 -I ../../ -c ../SimpleHeap/SimpleHeap.c -o ../objectFiles/SimpleHeap.o &&
gcc -Og -I ../../ -g  ../SimpleHeap/testSimpleHeap.c ../objectFiles/SimpleHeap.og ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleHeap.og &&
gcc -O3 -I ../../ ../SimpleHeap/testSimpleHeap.c ../objectFiles/SimpleHeap.o ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleHeap