#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleGraph/SimpleGraph.c -o ../objectFiles/SimpleGraph.og &&
gcc -I ../../ -c  ../SimpleGraph/SimpleGraph.c -o ../objectFiles/SimpleGraph.o &&
gcc -I ../../ -g  ../SimpleGraph/testSimpleGraph.c ../objectFiles/SimpleGraph.og ../objectFiles/SimpleHashMap.og ../objectFiles/SimpleList.og \
../objectFiles/SimpleLinkedList.og ../objectFiles/SimpleHeap.og -o ../testPrograms/testSimpleGraph.og &&
gcc -I ../../  ../SimpleGraph/testSimpleGraph.c ../objectFiles/SimpleGraph.o ../objectFiles/SimpleHashMap.o ../objectFiles/SimpleList.o \
../objectFiles/SimpleLinkedList.o ../objectFiles/SimpleHeap.o -o ../testPrograms/testSimpleGraph