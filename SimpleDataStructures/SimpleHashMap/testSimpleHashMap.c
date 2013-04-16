#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleDataStructures/SimpleHashMap/SimpleHashMap.h"

int main()
{
    SimpleHashMap aMap;
    long input;
    long *element;
    char strInp[256], *elementID;
    InitHashMap(&aMap, 10, 3);
    while (1) {
        printf("1. Add element.\n");
        printf("2. Get element.\n");
        printf("3. Exit.\n");
        scanf("%ld", &input);
        if (input == 3) {
            break;
        }
        if (1 == input)
        {
            printf("Enter element ID:\n");
            scanf("%s", strInp);
            elementID = malloc((strlen(strInp) + 1) * sizeof(char));
            if (!elementID) printf("elementID error..\n");
            strcpy(elementID, strInp);
            printf("Enter element itself (long number):\n");
            scanf("%ld", &input);
            element = malloc(sizeof(long));
            if (!element) printf("element error\n");
            *element = input;
            if (!AddOrReplaceElementInHashMap(&aMap, (const char*) elementID, element))
                printf("Added new element successfully\n");
            else { printf("Failed to add element, probably memory problems...\n"); break; }
                
        }
        else if (2 == input)
        {
            printf("Enter element ID:\n");
            scanf("%s", strInp);
            element = (long*) GetElementInHashMapByID(&aMap, strInp);
            if (element)
                printf("Element is:%ld\n", *element);
            else
                printf("No element with such key.\n");
        }

    }
    DisposeHashMapWithIDs(&aMap);
    return 0;
}
