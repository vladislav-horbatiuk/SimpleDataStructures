#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleUnionFind/SimpleUnionFind.c -o ../objectFiles/SimpleUnionFind.og &&
gcc -I ../../ -c  ../SimpleUnionFind/SimpleUnionFind.c -o ../objectFiles/SimpleUnionFind.o &&
gcc -I ../../ -g  ../SimpleUnionFind/testSimpleUnionFind.c ../objectFiles/SimpleUnionFind.og ../objectFiles/SimpleList.og -o ../testPrograms/testSimpleUnionFind.og &&
gcc -I ../../ ../SimpleUnionFind/testSimpleUnionFind.c ../objectFiles/SimpleUnionFind.o ../objectFiles/SimpleList.o -o ../testPrograms/testSimpleUnionFind