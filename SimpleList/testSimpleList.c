//
//  testSimpleList.c
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleDataStructures/SimpleList/SimpleList.h"

int main()
{
    SimpleList aList;
    long input;
    long *element, i;
    InitList(&aList, 10);
    while (1) {
        printf("Enter new element to add, 0 to exit, -index to delete element at index:\n");
        scanf("%ld", &input);
        if (!input) {
            break;
        }
        if (input < 0)
        {
            void *removedElement = PopAt(&aList, -input);
            if (removedElement) free(removedElement);
        }
        else
        {
            element = (long*)malloc(sizeof(long));
            *element = input;
            AddElement(&aList, element);
        }
        printf("Current list elements are:\n");
        for (i = 0; i < aList.currentNum; ++i) {
            printf("%ld ", *((long*)GetElementAt(&aList, i)));
        }
        printf("\n");
    }
    DisposeListWithElements(&aList);
    return 0;
}