//
//  SimpleUnionFind.h
//  
//
//  Created by Vladyslav Gorbatiuk on 07.02.13.
//
//

#ifndef _SimpleUnionFind_h
#define _SimpleUnionFind_h

#define UNION_SCALE_FACTOR (2)

typedef struct
{
	void *element;
	long leaderIndex, rank;
} tUnionFindElement;

typedef struct
{
	long currentNum, maxNum;
	tUnionFindElement *elements;
} SimpleUnionFind; 

/* 
*/
int InitUnionFind(SimpleUnionFind *oUnionFind, long iElementsNum);

/*
*/
long AddElementToUnionFind(SimpleUnionFind *oUnionFind, void *iElement);

/*
*/
tUnionFindElement FindLeaderOfElementAtIndex(SimpleUnionFind *oUnionFind, long iIndex);

/*
	RETURNS:
	1 if union actually occured (i.e. elements were in different clusters), 0 otherwise.
*/
int UnionClustersForElementsAtIndices(SimpleUnionFind *oUnionFind, long iFirstInd, long iSecondInd);

/*
*/
int DisposeUnionFindMemoryOnly(SimpleUnionFind *oUnionFind);

/*
*/
int DisposeUnionFindWithElements(SimpleUnionFind *oUnionFind);

#endif