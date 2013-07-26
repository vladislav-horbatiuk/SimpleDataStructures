#!/bin/bash
cd $(dirname $0)
gcc -Og -I ../../ -c -g ../SimpleHashMap/SimpleHashMap.c -o ../objectFiles/SimpleHashMap.og &&
gcc -O3 -I ../../ -c ../SimpleHashMap/SimpleHashMap.c -o ../objectFiles/SimpleHashMap.o &&
gcc -Og -I ../../ -g  ../SimpleHashMap/testSimpleHashMap.c ../objectFiles/SimpleHashMap.og ../objectFiles/SimpleLinkedList.og -o ../testPrograms/testSimpleHashMap.og &&
gcc -O3 -I ../../ ../SimpleHashMap/testSimpleHashMap.c ../objectFiles/SimpleHashMap.o ../objectFiles/SimpleLinkedList.o -o ../testPrograms/testSimpleHashMap