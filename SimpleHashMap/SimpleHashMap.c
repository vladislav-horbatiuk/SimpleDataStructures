//
//  SimpleHashMap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//


#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SimpleHashMap.h"

#define MAX_RAND 256


unsigned long hashFunc(SimpleHashMap *ioHashMap, const char *iElementID, unsigned long iCount);

static inline void _initCoeffs(SimpleHashMap *oHashMap, unsigned int iCoeffsCount)
{
    oHashMap->_coeffsCount = iCoeffsCount;
    oHashMap->_coeffs = malloc(iCoeffsCount * sizeof(int));
    int i;
    for (i = 0; i < iCoeffsCount; ++i) oHashMap->_coeffs[i] = rand() % MAX_RAND;
}

inline int _createNewElement(tHashMapElement **oNewElement, const char *iElementID, void *iElement)
{
    *oNewElement = malloc(sizeof(tHashMapElement));
    if (!*oNewElement)
        return -1;
    (*oNewElement)->elementID = iElementID; (*oNewElement)->element = iElement;
    return 0;
}

int InitHashMap(SimpleHashMap *oHashMap, unsigned long iNumOfElements, unsigned int iKeyLength)
{
    srand( time(NULL) );
    oHashMap->numberOfBuckets = iNumOfElements * 2 - 1;
    oHashMap->elements = calloc(sizeof(SimpleLinkedList), oHashMap->numberOfBuckets);
    if (!oHashMap->elements) return -1;
    oHashMap->count = 0;
    _initCoeffs(oHashMap, iKeyLength);
    InitLinkedList(&oHashMap->IDs);
    return 0;
}

int AddElementToHashMapWithoutChecks(SimpleHashMap *oHashMap, const char *iElementID, void *iElement)
{
    if (AddElementToLinkedList(&oHashMap->IDs, (void*) iElementID) == -1) return -1;
    tHashMapElement *newElement;
    if (_createNewElement(&newElement, iElementID, iElement)) return -1;
    unsigned long index = hashFunc(oHashMap, iElementID, oHashMap->numberOfBuckets);
    if (AddElementToLinkedList(&oHashMap->elements[index], newElement) == -1) return -1;
    oHashMap->count++;
    return 0;
}

int AddOrReplaceElementInHashMap(SimpleHashMap *oHashMap, const char *iElementID, void *iElement)
{
    tHashMapElement *newElement;
    if (_createNewElement(&newElement, iElementID, iElement)) return -1;
    unsigned long index = hashFunc(oHashMap, iElementID, oHashMap->numberOfBuckets);
    tLinkedListElement *current = oHashMap->elements[index].head;
    /*
     Advance iterator while we haven't reached end of the list and the current element ID differs from the inserted one
     */
    while (current && strcmp(((tHashMapElement*) current->element)->elementID, iElementID))
        current = current->next;
    
    /*
     If no element with similar ID was found - insert new element in list.
     */
    if (!current) {
        if (AddElementToLinkedList(&oHashMap->IDs, (void*)iElementID) == -1) return -1;

        if (AddElementToLinkedList(&oHashMap->elements[index], newElement))
            return -1;
        oHashMap->count++;
    }
    else
    {
        /*
         Replace existing element with the inserted one.
         */
        current->element = newElement;
    }
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

int DisposeHashMapWithIDs(SimpleHashMap *oHashMap)
{
    tLinkedListElement *current = oHashMap->IDs.head;
    tLinkedListElement *tmp;
    SimpleLinkedList elementsAtIndex;
    unsigned long index;
    while (current) {
        index = hashFunc(oHashMap, current->element, oHashMap->numberOfBuckets);
        DisposeLinkedListAndElements(&oHashMap->elements[index]);
        free(current->element);
        current = (tmp=current)->next;
        free(tmp);
    }
    free(oHashMap->_coeffs);
    free(oHashMap->elements);
    return 0;
}



















