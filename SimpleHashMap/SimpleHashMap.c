//
//  SimpleHashMap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SimpleHashMap.h"

#define MAX_RAND 256

static float increment;
static int *coeffs;
static unsigned int coeffsCount;

unsigned long hashFunc(const char *iElementID, unsigned long iCount);

inline void _initCoeffs(unsigned int iCoeffsCount)
{
    coeffsCount = iCoeffsCount;
    coeffs = malloc(iCoeffsCount * sizeof(int));
    if (!coeffs) printf("Coeffs error..\n");
    int i;
    for (i = 0; i < iCoeffsCount; ++i) coeffs[i] = rand() % MAX_RAND;
}

void InitHashMap(SimpleHashMap *oHashMap, unsigned long iNumOfElements, unsigned int iKeyLength)
{
    srand( time(NULL) );
    oHashMap->numberOfBuckets = iNumOfElements * 2 - 1;
    oHashMap->elements = malloc(sizeof(SimpleLinkedList) * oHashMap->numberOfBuckets);
    if (!oHashMap->elements) printf("oHashMap->elements error..\n");
    oHashMap->loadFactor = oHashMap->count = 0;
    _initCoeffs(iKeyLength);
    increment = 1.0 / oHashMap->numberOfBuckets;
}

int AddElementToHashMap(SimpleHashMap *oHashMap, const char *iElementID, void *iElement)
{
    tHashMapElement *newElement = malloc(sizeof(tHashMapElement));
    if (!newElement) return -1;
    newElement->elementID = iElementID; newElement->element = iElement;
    unsigned long index = hashFunc(iElementID, oHashMap->numberOfBuckets);
    printf("Element will be added at index:%ld\n", index);
    if (oHashMap->elements[index].head == NULL) InitLinkedList(&oHashMap->elements[index]);
    if (AddElementToLinkedList(&oHashMap->elements[index], newElement) == -1) return -1;
    oHashMap->count++;
    oHashMap->loadFactor += increment;
    return 0;
}

unsigned long hashFunc(const char *iElementID, unsigned long iCount)
{
    int i;
    unsigned long result = 0;
    unsigned int keyLength = strlen(iElementID);
    if (coeffsCount < keyLength) {
        coeffs = realloc(coeffs, keyLength * sizeof(int));
        if (!coeffs) printf("coeffs2 error..\n");
        while (coeffsCount < keyLength) {
            coeffs[coeffsCount] = rand() % MAX_RAND;
            coeffsCount++;
        }
    }
    for (i = 0; i < keyLength; ++i) {
        result += iElementID[i] * coeffs[i];
        result %= iCount;
    }
    return result;
}

void DisposeHashMap(SimpleHashMap *oMap)
{
    
}

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
            if (!AddElementToHashMap(&aMap, (const char*) elementID, element))
                printf("Added new element successfully\n");
            else { printf("Failed to add element, probably memory problems...\n"); break; }
                
        }
        else if (2 == input)
        {
            printf("Enter element ID:\n");
            scanf("%s", strInp);
            unsigned long index = hashFunc((const char*) strInp, aMap.numberOfBuckets);
            /*
             TODO: This won't obviously work when two keys hash to the same bucket - so we will need to implement GetElementInHashMapAt function thoroughly.
             */
            tHashMapElement *HMelement = (tHashMapElement*) aMap.elements[index].head->element;
            element = (long*)HMelement->element;
            printf("Element is:%ld\n", *element);
        }

    }
    DisposeHashMap(&aMap);
    return 0;
}




















