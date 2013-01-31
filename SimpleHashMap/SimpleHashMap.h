//
//  SimpleHashMap.h
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//

#ifndef _SimpleHashMap_h
#define _SimpleHashMap_h

#include "SimpleLinkedList.h"

typedef struct
{
    const char *elementID;
    void *element;
} tHashMapElement;

typedef struct
{
    SimpleLinkedList  *elements;
    unsigned long count, numberOfBuckets;
    int *_coeffs, _coeffsCount;
    SimpleLinkedList IDs;
} SimpleHashMap;

/*
 */
int InitHashMap(SimpleHashMap *oHashMap, unsigned long iNumOfElements, unsigned int iKeyLength);

/*
 */
int AddElementToHashMapWithoutChecks(SimpleHashMap *oHashMap, const char *iElementID, void *iElement);

/*
 */
void* GetElementInHashMapByID(SimpleHashMap *iHashMap, const char *iElementID);

/*
 */
int AddOrReplaceElementInHashMap(SimpleHashMap *oHashMap, const char *iElementID, void *iElement);

/*
 */
int DisposeHashMapWithIDs(SimpleHashMap *);
#endif
