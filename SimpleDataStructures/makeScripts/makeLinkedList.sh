#!/bin/bash
cd $(dirname $0)
gcc -I ../../ -c -g ../SimpleLinkedList/SimpleLinkedList.c -o ../objectFiles/SimpleLinkedList.og &&
gcc -I ../../ -c  ../SimpleLinkedList/SimpleLinkedList.c -o ../objectFiles/SimpleLinkedList.o &&
gcc -I ../../ -g  ../SimpleLinkedList/testSimpleLinkedList.c ../objectFiles/SimpleLinkedList.og -o ../testPrograms/testSimpleLinkedList.og &&
gcc -I ../../ ../SimpleLinkedList/testSimpleLinkedList.c ../objectFiles/SimpleLinkedList.o -o ../testPrograms/testSimpleLinkedList