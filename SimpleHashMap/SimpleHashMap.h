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
    float loadFactor;
    unsigned long count;
    unsigned long numberOfBuckets;
} SimpleHashMap;

/*
 */
void InitHashMap(SimpleHashMap *oHashMap, unsigned long iNumOfElements, unsigned int iKeyLength);

/*
 */
int AddElementToHashMap(SimpleHashMap *iHashMap, const char *iElementID, void *iElement);

/*
 */
void* GetElementInHashMapByID(SimpleHashMap *iHashMap, const char *iElementID);

/*
 */
void DisposeHashMap(SimpleHashMap *iHashMap);
#endif
