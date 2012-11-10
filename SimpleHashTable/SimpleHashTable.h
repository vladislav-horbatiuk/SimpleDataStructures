//
//  SimpleHashTable.h
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//
#include "SimpleList.h"

#ifndef _SimpleHashTable_h
#define _SimpleHashTable_h

typedef struct
{
    const char *elementID;
    void *element;
} tHashTableElement;

typedef struct
{
    SimpleList elements;
    float loadFactor;
    unsigned long count;
} SimpleHashTable;

void Init(SimpleHashTable *iHashTable, unsigned long iNumOfElements);
void AddElement(SimpleHashTable *iHashTable, const char *iElementID, void *iElement);
void* GetElementByID(SimpleHashTable *iHashTable, const char *iElementID);
void Dispose(SimpleHashTable *iHashTable);
#endif
