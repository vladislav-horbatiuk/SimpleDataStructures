//
//  SimpleList.c
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#include <stdlib.h>

#include "SimpleDataStructures/SimpleList/SimpleList.h"

int InitList(SimpleList *oList, long iMaxNum)
{
    oList->maxNum = iMaxNum;
    oList->currentNum = 0;
    if ((oList->listHead = (void**) calloc(iMaxNum, sizeof(void*))))
        return 0;
    return -1;

}

void* GetElementAt(SimpleList *iList, long iIndex)
{
    if (iIndex < iList->currentNum) {
        return iList->listHead[iIndex];
    }
    return NULL;
}

void SetElementAt(SimpleList *oList, long iIndex, void *iElement)
{
    if (iIndex < oList->currentNum) {
        oList->listHead[iIndex] = iElement;
    }
}

int AddElement(SimpleList *oList, void *iElement)
{
    if (oList->currentNum < oList->maxNum) {
        oList->listHead[oList->currentNum] = iElement;
        ++oList->currentNum;
    }
    else 
    {
        void **temporaryHead = (void**) realloc(oList->listHead, oList->maxNum * SCALE_FACTOR * sizeof(void*));
        if (!temporaryHead)
            return -1;
        oList->maxNum *= SCALE_FACTOR;
        oList->listHead = temporaryHead;
        AddElement(oList, iElement);
    }
    return 0;
}

void* PopAt(SimpleList *oList, long iIndex)
{
    if (iIndex < oList->currentNum)
    {
        void *toReturn = oList->listHead[iIndex];
        --oList->currentNum;

        while (iIndex < oList->currentNum)
        {
            oList->listHead[iIndex] = oList->listHead[iIndex+1];
            iIndex++;
        }
        return toReturn;
    }
    return NULL;
}

int DisposeListWithElements(SimpleList *oList)
{
    int i;
    for (i = 0; i < oList->currentNum; ++i) 
    {
        free(oList->listHead[i]);
    }
    free(oList->listHead);
    return 0;
}

int DisposeListMemoryOnly(SimpleList *oList)
{
    free(oList->listHead);
    return 0;
}

void TraverseAndDisposeList(SimpleList *oList, void (*iFunc) (void*))
{
    int i;
    for (i = 0;i < oList->currentNum; ++i)
    {
        iFunc(oList->listHead[i]);
        free(oList->listHead[i]);
    }
    free(oList->listHead);
}

