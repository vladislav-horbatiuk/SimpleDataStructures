//
//  testSimpleHeap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 03.01.13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleHeap.h"

int main()
{
    SimpleHeap aHeap;
    SimpleList elements;
    long input, i;
    double key;
    long *element;
    char strInp[256];
    InitHeap(&aHeap, 10);
    while (1) {
        printf("1. Add element.\n");
        printf("2. Extract min element.\n");
        printf("3. Exit.\n");
        scanf("%ld", &input);
        if (input == 3) {
            break;
        }
        if (1 == input)
        {
            printf("Enter element key:\n");
            scanf("%lf", &key);
            printf("Enter element itself (long number):\n");
            scanf("%ld", &input);
            element = malloc(sizeof(long));
            if (!element) printf("element error\n");
            *element = input;
            if (!AddElementToHeap(&aHeap, key, element))
                printf("Added new element successfully\n");
            else { printf("Failed to add element, probably memory problems...\n"); break; }
                
        }
        else if (2 == input)
        {
            printf("Extracting min element...\n");
            tHeapElement *minElement = ExtractMinElement(&aHeap);
            if (minElement)
                printf("Element key:%lf, element itself:%ld.\n", minElement->key, *(long*) minElement->element);
            else
                printf("No elements in heap.\n");
        }
        printf("Current heap keys:\n");
        elements = aHeap.elements;
        for (i = 0; i < elements.currentNum; ++i)
        	printf("%lf ", ((tHeapElement*)GetElementAt(&elements, i))->key);
        printf("\n");
    }
    DisposeHeap(&aHeap);
    return 0;
}