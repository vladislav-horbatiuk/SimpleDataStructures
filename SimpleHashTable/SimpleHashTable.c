//
//  SimpleHashTable.c
//  
//
//  Created by Vladyslav Gorbatiuk on 09.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SimpleHashTable.h"

unsigned long hashFunc(const char *iElementID);

void Init(SimpleHashTable *iHashTable, unsigned long iNumOfElements)
{
    srand( time(NULL) );
    InitializeList(&iHashTable->elements, iNumOfElements * 2 - 1);
    iHashTable->loadFactor = iHashTable->count = 0;
}

void AddElement(SimpleHashTable *oHashTable, const char *iElementID, void *iElement)
{
    unsigned long index = hashFunc(iElementID);
    if (GetElementAt(oHashTable, index) == NULL) {
        <#statements#>
    }
}


