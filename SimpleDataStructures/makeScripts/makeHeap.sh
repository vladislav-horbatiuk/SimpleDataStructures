#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleHeap/SimpleHeap.c -o ../objectFiles/SimpleHeap.og &&
gcc -I ../../ -c ../SimpleHeap/SimpleHeap.c -o ../objectFiles/SimpleHeap.o &&
gcc -I ../../ -g  ../SimpleHeap/testSimpleHeap.c ../objectFiles/SimpleHeap.og ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleHeap.og &&
gcc -I ../../ ../SimpleHeap/testSimpleHeap.c ../objectFiles/SimpleHeap.o ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleHeap