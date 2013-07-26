#!/bin/bash
cd $(dirname $0)
gcc -Og -I ../../ -c -g ../SimpleUnionFind/SimpleUnionFind.c -o ../objectFiles/SimpleUnionFind.og &&
gcc -O3 -I ../../ -c  ../SimpleUnionFind/SimpleUnionFind.c -o ../objectFiles/SimpleUnionFind.o &&
gcc -Og -I ../../ -g  ../SimpleUnionFind/testSimpleUnionFind.c ../objectFiles/SimpleUnionFind.og ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleUnionFind.og &&
gcc -O3 -I ../../ ../SimpleUnionFind/testSimpleUnionFind.c ../objectFiles/SimpleUnionFind.o ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleUnionFind