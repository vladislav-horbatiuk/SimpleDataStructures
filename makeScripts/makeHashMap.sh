#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleHashMap/SimpleHashMap.c -o ../objectFiles/SimpleHashMap.og &&
gcc -I ../../ -c ../SimpleHashMap/SimpleHashMap.c -o ../objectFiles/SimpleHashMap.o &&
gcc -I ../../ -g  ../SimpleHashMap/testSimpleHashMap.c ../objectFiles/SimpleHashMap.og ../objectFiles/SimpleLinkedList.og -o ../testPrograms/testSimpleHashMap.og &&
gcc -I ../../ ../SimpleHashMap/testSimpleHashMap.c ../objectFiles/SimpleHashMap.o ../objectFiles/SimpleLinkedList.o -o ../testPrograms/testSimpleHashMap