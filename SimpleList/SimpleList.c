//
//  SimpleList.c
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleList.h"

void InitList(SimpleList *oList, long iMaxNum)
{
    oList->maxNum = iMaxNum;
    oList->currentNum = 0;
    oList->listHead = (void**) calloc(iMaxNum, sizeof(void*));
}

void* GetElementAt(SimpleList *iList, long iIndex)
{
    if (iIndex < iList->currentNum) {
        return iList->listHead[iIndex];
    }
    return 0;
}

void SetElementAt(SimpleList *oList, long iIndex, void *iElement)
{
    if (iIndex < oList->currentNum) {
        oList->listHead[iIndex] = iElement;
    }
}

void AddElement(SimpleList *oList, void *iElement)
{
    if (oList->currentNum < oList->maxNum) {
        oList->listHead[oList->currentNum] = iElement;
        ++oList->currentNum;
    }
    else {
        oList->maxNum *= 2;
        void **temporaryHead = (void**) realloc(oList->listHead, oList->maxNum * sizeof(void*));
        if (temporaryHead) {
            oList->listHead = temporaryHead;
        }
        else
        {
            puts ("Error (re)allocating memory");
            exit(1);
        }
        AddElement(oList, iElement);
    }
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

void DisposeList(SimpleList *oList)
{
    int i;
    for (i = 0; i < oList->currentNum; ++i) {
        free(oList->listHead[i]);
    }
    free(oList->listHead);
}

