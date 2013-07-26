#!/bin/bash
cd $(dirname $0)
gcc -Og -I ../../ -c -g ../SimpleLinkedList/SimpleLinkedList.c -o ../objectFiles/SimpleLinkedList.og &&
gcc -O3 -I ../../ -c  ../SimpleLinkedList/SimpleLinkedList.c -o ../objectFiles/SimpleLinkedList.o &&
gcc -Og -I ../../ -g  ../SimpleLinkedList/testSimpleLinkedList.c ../objectFiles/SimpleLinkedList.og -o ../testPrograms/testSimpleLinkedList.og &&
gcc -O3 -I ../../ ../SimpleLinkedList/testSimpleLinkedList.c ../objectFiles/SimpleLinkedList.o -o ../testPrograms/testSimpleLinkedList