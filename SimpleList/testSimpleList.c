//
//  testSimpleList.c
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleList.c"

int main()
{
    SimpleList aList;
    long input;
    int *element, i;
    InitList(&aList, 10);
    while (1) {
        printf("Enter new element to add, 0 to exit, -index to delete element at index:\n");
        scanf("%ld", &input);
        if (!input) {
            break;
        }
        if (input < 0)
        {
            void *removedElement = PopAt(&aList, -input));
            if (removedElement) free(removedElement);
        }
        else
        {
            element = (int*)malloc(sizeof(int));
            *element = input;
            AddElement(&aList, element);
        }
        printf("Current list elements are:\n");
        for (i = 0; i < aList.currentNum; ++i) {
            printf("%d ", *((int*)GetElementAt(&aList, i)));
        }
        printf("\n");
    }
    DisposeList(&aList);
    return 0;
}