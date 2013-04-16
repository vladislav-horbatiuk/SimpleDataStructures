//
//  testSimpleHeap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 03.01.13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleDataStructures/SimpleHeap/SimpleHeap.h"

int main()
{
    SimpleHeap aHeap;
    SimpleList elements;
    long input, i;
    double key;
    long *element;
    tHeapElement *extractedElement;
    char strInp[256];
    InitHeap(&aHeap, 10);
    while (1) {
        printf("1. Add element.\n");
        printf("2. Extract min element.\n");
        printf("3. Extract element at index.\n");
        printf("4. Exit.\n");
        scanf("%ld", &input);
        if (input == 4) {
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
            if (!AddElementToHeap(&aHeap, key, element, NULL))
                printf("Added new element successfully\n");
            else { printf("Failed to add element, probably memory problems...\n"); break; }
                
        }
        else if (2 == input)
        {
            printf("Extracting min element...\n");
            extractedElement = ExtractMinElement(&aHeap, NULL);
            if (extractedElement)
            {
                printf("Element key:%lf, element itself:%ld.\n", extractedElement->key, *(long*) extractedElement->element);
                free(extractedElement->element);
                free(extractedElement);
            }
            else
                printf("No elements in heap.\n");
        }
        else if (3 == input)
        {
            printf("Enter element index:\n");
            scanf("%ld", &input);
            extractedElement = PopHeapElementAtIndex(&aHeap, input, NULL);
            if (!extractedElement)
                printf("Index out of bounds.\n");
            else
            {
                printf("Element key:%lf, element itself:%ld\n", extractedElement->key, *(long*) extractedElement->element);
                free(extractedElement->element);
                free(extractedElement);                
            }
        }

        printf("Current heap keys:\n");
        elements = aHeap.elements;
        for (i = 0; i < elements.currentNum; ++i)
        	printf("%lf ", ((tHeapElement*)GetElementAt(&elements, i))->key);
        printf("\n");
    }
    DisposeHeapWithElements(&aHeap);
    return 0;
}