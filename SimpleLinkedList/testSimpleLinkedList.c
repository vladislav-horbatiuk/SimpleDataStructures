//
//  testSimpleLinkedList.c
//
//
//  Created by Vladyslav Gorbatiuk on 10.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleLinkedList.h"

void printFunc(void *iElement)
{
    printf("%ld\n", *(long*)iElement);
}

int main()
{
    SimpleLinkedList linkedList;
    long input;
    long *element, i;
    InitLinkedList(&linkedList);
    while (1) {
        printf("Enter new element to add, 0 to exit:\n");
        scanf("%ld", &input);
        if (!input)
            break;
        element = (long*)malloc(sizeof(long));
        *element = input;
        if (!AddElementToLinkedList(&linkedList, element))
        {
            printf("Current list elements are:\n");
            TraverseLinkedListElementsUsingFunc(&linkedList, &printFunc);
        }
        else { printf("Failed to add element, probably memory problems..."); break; }
    }
    DisposeLinkedListAndElements(&linkedList);
}