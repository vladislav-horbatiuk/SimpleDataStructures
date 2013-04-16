#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleList/SimpleList.c -o ../objectFiles/SimpleList.og &&
gcc -I ../../ -c  ../SimpleList/SimpleList.c -o ../objectFiles/SimpleList.o &&
gcc -I ../../ -g  ../SimpleList/testSimpleList.c ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleList.og &&
gcc -I ../../ ../SimpleList/testSimpleList.c ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleList