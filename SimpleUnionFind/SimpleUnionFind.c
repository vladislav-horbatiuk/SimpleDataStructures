//
//  SimpleUnionFind.c
//  
//
//  Created by Vladyslav Gorbatiuk on 08.02.13.
//
//

#include <stdlib.h>

#include "SimpleDataStructures/SimpleUnionFind/SimpleUnionFind.h"
#include "SimpleDataStructures/SimpleList/SimpleList.h"

int InitUnionFind(SimpleUnionFind *oUnionFind, long iElementsNum)
{
	oUnionFind->elements = malloc(iElementsNum * sizeof(tUnionFindElement));
	if (!oUnionFind->elements)
		return -1;
	oUnionFind->maxNum = iElementsNum;
	oUnionFind->currentNum = 0;
	return 0;
}

int DisposeUnionFindMemoryOnly(SimpleUnionFind *oUnionFind)
{
	free(oUnionFind->elements);
	return 0;
}

int DisposeUnionFindWithElements(SimpleUnionFind *oUnionFind)
{
	long i;
	for (i = 0; i < oUnionFind->currentNum; ++i)
	{
		free(oUnionFind->elements[i].element);
	}
	free(oUnionFind->elements);
	return 0;
}

long AddElementToUnionFind(SimpleUnionFind *oUnionFind, void *iElement)
{
	if (oUnionFind->currentNum < oUnionFind->maxNum)
	{
		oUnionFind->elements[oUnionFind->currentNum].element = iElement;
		oUnionFind->elements[oUnionFind->currentNum].leaderIndex = oUnionFind->currentNum;
		oUnionFind->elements[oUnionFind->currentNum++].rank = 0;
	}
	else
	{
        tUnionFindElement *temporaryArray = realloc(oUnionFind->elements,\
         oUnionFind->maxNum * UNION_SCALE_FACTOR * sizeof(tUnionFindElement));
        if (!temporaryArray)
            return -1;
        oUnionFind->maxNum *= UNION_SCALE_FACTOR;
        oUnionFind->elements = temporaryArray;
        AddElementToUnionFind(oUnionFind, iElement);
    }
    return oUnionFind->currentNum - 1;
}

tUnionFindElement FindLeaderOfElementAtIndex(SimpleUnionFind *oUnionFind, long iIndex)
{
	tUnionFindElement returnElement, *elementOnPath;
	SimpleList elementsOnPath;
	returnElement.leaderIndex = -1;
	if (InitList(&elementsOnPath, 10))
		return returnElement;
	while (oUnionFind->elements[iIndex].leaderIndex != iIndex)
	{
		AddElement(&elementsOnPath, &oUnionFind->elements[iIndex]);
		iIndex = oUnionFind->elements[iIndex].leaderIndex;
	}
	long i;
	for (i = 0; i < elementsOnPath.currentNum; ++i)
	{
		elementOnPath = GetElementAt(&elementsOnPath, i);
		elementOnPath->leaderIndex = iIndex;
	}
	DisposeListMemoryOnly(&elementsOnPath);
	return oUnionFind->elements[iIndex];
}

int UnionClustersForElementsAtIndices(SimpleUnionFind *oUnionFind, long iFirstInd, long iSecondInd)
{
	tUnionFindElement firstElement = FindLeaderOfElementAtIndex(oUnionFind, iFirstInd);
	tUnionFindElement secondElement = FindLeaderOfElementAtIndex(oUnionFind, iSecondInd);
	if (firstElement.leaderIndex == secondElement.leaderIndex)
		return 0;
	long minRankInd, maxRankInd;
	if (firstElement.rank < secondElement.rank)
	{
		minRankInd = firstElement.leaderIndex;
		maxRankInd = secondElement.leaderIndex;
	}
	else
	{
		minRankInd = secondElement.leaderIndex;
		maxRankInd = firstElement.leaderIndex;
	}
	oUnionFind->elements[minRankInd].leaderIndex = oUnionFind->elements[maxRankInd].leaderIndex;
	if (firstElement.rank == secondElement.rank)
		++oUnionFind->elements[maxRankInd].rank;
	return 1;
}
