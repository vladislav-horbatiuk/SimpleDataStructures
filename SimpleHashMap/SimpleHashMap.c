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
#include "SimpleHashMap.h"

unsigned long hashFunc(const char *iElementID);

void InitHashMap(SimpleHashMap *iHashMap, unsigned long iNumOfElements)
{
    srand( time(NULL) );
    InitializeList(&iHashMap->elements, iNumOfElements * 2 - 1);
    iHashMap->loadFactor = iHashMap->count = 0;
}

int AddElementToHashMap(SimpleHashMap *oHashMap, const char *iElementID, void *iElement)
{
    unsigned long index = hashFunc(iElementID);
    if (GetElementAt(&oHashMap->elements, index) == NULL)
    {
    }
        
}


