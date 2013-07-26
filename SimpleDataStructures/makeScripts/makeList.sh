#!/bin/bash
cd $(dirname $0)
gcc -Og -I ../../ -c -g ../SimpleList/SimpleList.c -o ../objectFiles/SimpleList.og &&
gcc -O3 -I ../../ -c  ../SimpleList/SimpleList.c -o ../objectFiles/SimpleList.o &&
gcc -Og -I ../../ -g  ../SimpleList/testSimpleList.c ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleList.og &&
gcc -O3 -I ../../ ../SimpleList/testSimpleList.c ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleList