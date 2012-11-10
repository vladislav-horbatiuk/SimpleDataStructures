//
//  SimpleHashMap.h
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//
#include "SimpleList.h"
#include "SimpleLinkedList.h"

#ifndef _SimpleHashMap_h
#define _SimpleHashMap_h

typedef struct
{
    const char *elementID;
    void *element;
} tHashMapElement;

typedef struct
{
    SimpleList elements;
    float loadFactor;
    unsigned long count;
} SimpleHashMap;

/*
 */
void InitHashMap(SimpleHashMap *iHashMap, unsigned long iNumOfElements);

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
