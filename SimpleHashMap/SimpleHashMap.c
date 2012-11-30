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


unsigned long hashFunc(SimpleHashMap *ioHashMap, const char *iElementID, unsigned long iCount);

inline void _initCoeffs(SimpleHashMap *oHashMap, unsigned int iCoeffsCount)
{
    oHashMap->_coeffsCount = iCoeffsCount;
    oHashMap->_coeffs = malloc(iCoeffsCount * sizeof(int));
    if (!oHashMap->_coeffs) printf("Coeffs malloc error..\n");
    int i;
    for (i = 0; i < iCoeffsCount; ++i) oHashMap->_coeffs[i] = rand() % MAX_RAND;
}

int InitHashMap(SimpleHashMap *oHashMap, unsigned long iNumOfElements, unsigned int iKeyLength)
{
    srand( time(NULL) );
    oHashMap->numberOfBuckets = iNumOfElements * 2 - 1;
    oHashMap->elements = calloc(sizeof(SimpleLinkedList), oHashMap->numberOfBuckets);
    if (!oHashMap->elements) { printf("Calloc of hashmap elements array failed..\n"); return -1; }
    oHashMap->count = 0;
    _initCoeffs(oHashMap, iKeyLength);
    return 0;
}

int AddElementToHashMapWithoutChecks(SimpleHashMap *oHashMap, const char *iElementID, void *iElement)
{
    tHashMapElement *newElement = malloc(sizeof(tHashMapElement));
    if (!newElement) return -1;
    newElement->elementID = iElementID; newElement->element = iElement;
    unsigned long index = hashFunc(oHashMap, iElementID, oHashMap->numberOfBuckets);
    printf("Element will be added at index:%ld\n", index);
    /* Currently not needed
    if (oHashMap->elements[index].head == NULL) InitLinkedList(&oHashMap->elements[index]);
     */
    if (AddElementToLinkedList(&oHashMap->elements[index], newElement) == -1) return -1;
    oHashMap->count++;
    return 0;
}

void* GetElementInHashMapByID(SimpleHashMap *iHashMap, const char *iElementID)
{
    /* The length of iElementID is greater than the length  of any key in iHashMap - hence, definitely no element will match - return NULL. */
    if (strlen(iElementID) > iHashMap->_coeffsCount) return NULL;
    /*Since if we are here, length of searched key is definetely <= max length of key in the iHashMap, the 'hashFunc' function won't change iHashMap.*/
    unsigned long index = hashFunc(iHashMap, iElementID, iHashMap->numberOfBuckets);
    
    SimpleLinkedList elementsAtIndex = iHashMap->elements[index];
    
    tLinkedListElement* current = elementsAtIndex.head;
    while (current && strcmp((char*)((tHashMapElement*)current->element)->elementID, iElementID)) {
        current = current->next;
    }
    
    if (current) return ((tHashMapElement*)current->element)->element;
    else return NULL;
}

unsigned long hashFunc(SimpleHashMap *ioHashMap, const char *iElementID, unsigned long iCount)
{
    int i;
    unsigned long result = 0;
    unsigned int keyLength = strlen(iElementID);
    if (ioHashMap->_coeffsCount < keyLength) {
        ioHashMap->_coeffs = realloc(ioHashMap->_coeffs, keyLength * sizeof(int));
        if (!ioHashMap->_coeffs) printf("Error during memory allocation for hash function coefficients..\n");
        while (ioHashMap->_coeffsCount < keyLength) {
            ioHashMap->_coeffs[ioHashMap->_coeffsCount] = rand() % MAX_RAND;
            ioHashMap->_coeffsCount++;
        }
    }
    for (i = 0; i < keyLength; ++i) {
        result += iElementID[i] * ioHashMap->_coeffs[i];
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
            if (!AddElementToHashMapWithoutChecks(&aMap, (const char*) elementID, element))
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
    DisposeHashMap(&aMap);
    return 0;
}




















