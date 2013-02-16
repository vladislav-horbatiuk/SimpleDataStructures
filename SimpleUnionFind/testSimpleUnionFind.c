//
//  testSimpleUnionFind.c
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleDataStructures/SimpleUnionFind/SimpleUnionFind.h"

int main()
{
    SimpleUnionFind testUnionFind;
    long input;
    long *element, i;
    tUnionFindElement leaderElement, requestedElement;
    InitUnionFind(&testUnionFind, 10);
    while (1) {
        printf("1. Add element.\n");
        printf("2. Lookup element at index.\n");
        printf("3. Union clusters.\n");
        printf("4. Exit.\n");
        scanf("%ld", &input);
        if (4 == input) {
            break;
        }
        if (1 == input)
        {
            printf("Enter element (long number):\n");
            scanf("%ld", &input);
            element = malloc(sizeof(long));

            if (!element) 
                { printf("Memory problems...\n"); continue; }

            *element = input;

            if (AddElementToUnionFind(&testUnionFind, element) == -1)
                { printf("Failed to add element, memory problems...\n"); continue; }

            printf("Added new element successfully\n");
        }
        else if (2 == input)
        {
            printf("Enter element index:\n");
            scanf("%ld", &input);
            if (input >= testUnionFind.currentNum)
                { printf("Index out of bounds.\n"); continue; }
            requestedElement = testUnionFind.elements[input];
            leaderElement = FindLeaderOfElementAtIndex(&testUnionFind, input);
            if (leaderElement.leaderIndex == -1)
                { printf("Memory problems...\n"); continue; }
            printf("Requested elements value:%ld, leader index:%ld.\n", *(long*) requestedElement.element, leaderElement.leaderIndex);
        }
        else if (3 == input)
        {
            printf("Enter the index of any element in first cluster:\n");
            scanf("%ld", &input);
            printf("Enter the index of any element in second cluster:\n");
            scanf("%ld", &i);
            UnionClustersForElementsAtIndices(&testUnionFind, input, i);
        }
        printf("Current union-find data structure elements:\n");
        for (i = 0; i < testUnionFind.currentNum; ++i)
        {
            leaderElement = FindLeaderOfElementAtIndex(&testUnionFind, i);
            printf("%ld. Value:%ld, leader index:%ld.\n", i, *(long*) testUnionFind.elements[i].element, leaderElement.leaderIndex);
        }        
    }
    DisposeUnionFindWithElements(&testUnionFind);
    return 0;
}