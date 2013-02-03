//
//  SimpleGraph.c
//  
//
//  Created by Vladyslav Gorbatiuk on 12.01.13.
//
//

#include "SimpleDataStructures/SimpleGraph/SimpleGraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline void _updateVertex(void*, long);
static inline void _updateAdjacentVerticesInHeap(tVertex*, long, SimpleHashMap*, SimpleHeap*);
static inline void _fillHeapWithVertices(SimpleList*, SimpleHeap*);
static inline double _addNewVertexToMST(SimpleHeap*, SimpleHashMap*, SimpleGraph*);
static inline long _addVertexAndCorrespondingEdgeToMST(SimpleGraph*, tVertex*, double);

static inline void _freeVertexData(void *iVertex)
{
	tVertex *vertexToFree = VERTEX_PTR(iVertex);
	free(vertexToFree->data);
	free(vertexToFree->edges.listHead);
}

static inline void _freeVertexOnly(void *iVertex)
{
	tVertex *vertexToFree = VERTEX_PTR(iVertex);
	free(vertexToFree->edges.listHead);
}

int InitGraph(SimpleGraph *oGraph, long iVerticesNum, long iEdgesNum)
{
	int res = InitList(&oGraph->vertices, iVerticesNum);
	res = res || InitList(&oGraph->edges, iEdgesNum);
	if (res)
		return -1;
	return 0;
}

int AddVertexWithData(SimpleGraph *oGraph,void *iData, long iEdgesNum)
{
	tVertex *newVertex = malloc(1 * sizeof(tVertex));
	if (!newVertex)
		return -1;
	newVertex->data = iData;
	newVertex->_heapIndex = newVertex->_sourceVertexIndex = UNSET_VALUE;
	if (InitList(&newVertex->edges, iEdgesNum))
		return -1;
	if (AddElement(&oGraph->vertices, newVertex))
		return -1;
	return 0;
}

int AddEdge(SimpleGraph *oGraph, long iSourceVertexInd, long iDestVertexInd, double iCost)
{
	tEdge *newEdge = malloc(1 * sizeof(tEdge));
	if (!newEdge)
		return -1;
	tVertex *source = VERTEX_PTR(GetElementAt(&oGraph->vertices, iSourceVertexInd));
	tVertex *dest = VERTEX_PTR(GetElementAt(&oGraph->vertices, iDestVertexInd));
	if (!source || !dest)
		return INDEX_OUT_OF_BOUNDS;
	newEdge->source = source; newEdge->dest = dest; newEdge->cost = iCost;
	if (AddElement(&oGraph->edges, newEdge))
		return -1;
	if (AddElement(&source->edges, newEdge))
	{
		PopAt(&oGraph->edges, oGraph->edges.currentNum - 1);
		return -1;
	}
	if (AddElement(&dest->edges, newEdge))
	{
		PopAt(&oGraph->edges, oGraph->edges.currentNum - 1);
		PopAt(&source->edges, source->edges.currentNum - 1);
		return -1;
	}
	return 0;
}

double MinimumSpanningTree(SimpleGraph *iGraph, SimpleGraph *oMST)
{
	double result = 0;
	SimpleList *vertices = &iGraph->vertices;
	long i, vertNum = vertices->currentNum;

	InitGraph(oMST, vertNum, vertNum);

	SimpleHashMap verticesSoFar;
	InitHashMap(&verticesSoFar, vertNum, MAX_POINTER_LENGTH);

	SimpleHeap verticesRest;
	_fillHeapWithVertices(vertices, &verticesRest);

	do
	{
		result += _addNewVertexToMST(&verticesRest, &verticesSoFar, oMST);
	} while (verticesRest.elements.currentNum);
	DisposeHeapWithElements(&verticesRest);
	DisposeHashMapWithIDs(&verticesSoFar);
	return result;
}

static inline void _fillHeapWithVertices(SimpleList *iVertices, SimpleHeap *oHeap)
{
	long i, vertNum = iVertices->currentNum;
	InitHeap(oHeap, vertNum);
	/*The first vertex is added with weight=0, since it will serve as a source vertex to start building MST.*/
	AddElementToHeap(oHeap, 0, GetElementAt(iVertices, 0), &_updateVertex);
	/*Add the rest of vertices with 'infinite' key, since they aren't reachable at all in the beginning of an algorithm.*/
	for (i = 1; i < vertNum; ++i)
	{
		AddElementToHeap(oHeap, DBL_MAX, GetElementAt(iVertices, i), &_updateVertex);
	}
}

static inline double _addNewVertexToMST(SimpleHeap *oVerticesRest, SimpleHashMap *oVerticesInMST, SimpleGraph *oMST)
{
	char buffer[MAX_POINTER_LENGTH], *vertexID;
	tHeapElement *minHeapElement = ExtractMinElement(oVerticesRest, &_updateVertex);
	double minEdgeCost = minHeapElement->key;
	tVertex *newVertex = VERTEX_PTR(minHeapElement->element);
	long iSourceVertexInd = _addVertexAndCorrespondingEdgeToMST(oMST, newVertex, minEdgeCost);

	memset(buffer, 0, MAX_POINTER_LENGTH);
	sprintf(buffer, "%p", newVertex);
	vertexID = malloc(strlen(buffer) * sizeof(char));
	strcpy(vertexID, buffer);
	AddElementToHashMapWithoutChecks(oVerticesInMST, vertexID, (void *) 1);

	_updateAdjacentVerticesInHeap(newVertex, iSourceVertexInd, oVerticesInMST, oVerticesRest);
	free(minHeapElement);
	return minEdgeCost;
}
static inline long _addVertexAndCorrespondingEdgeToMST(SimpleGraph *oMST, tVertex *iNewVertex, double iEdgeCost)
{
	AddVertexWithData(oMST, iNewVertex->data, iNewVertex->edges.currentNum);
	long sourceVertexInd = oMST->vertices.currentNum - 1;
	if (sourceVertexInd)
	{
		if (iNewVertex->_sourceVertexIndex == UNSET_VALUE)
			perror("Disconnected graph passed into MinimumSpanningTree function - you need to check for graph connectivity before trying to find MST!");
		AddEdge(oMST, iNewVertex->_sourceVertexIndex, sourceVertexInd, iEdgeCost);
	}
	return sourceVertexInd;
}

static inline void _updateAdjacentVerticesInHeap(tVertex *iSourceVertex, long iSourceVertexInd, SimpleHashMap *iVerticesInMST, SimpleHeap *oVerticesRest)
{
	tEdge *currentEdge;
	tHeapElement *heapElementToUpdate;
	tVertex *adjacentVertex;
	char buffer[MAX_POINTER_LENGTH];
	long i;
	for (i = 0; i < iSourceVertex->edges.currentNum; ++i)
	{
		currentEdge = GetElementAt(&iSourceVertex->edges, i);
		if (currentEdge->source == iSourceVertex)
			adjacentVertex = currentEdge->dest;
		else
			adjacentVertex = currentEdge->source;
		sprintf(buffer, "%p", adjacentVertex);
		if (GetElementInHashMapByID(iVerticesInMST, buffer) == NULL)
		{
			heapElementToUpdate = PopHeapElementAtIndex(oVerticesRest, adjacentVertex->_heapIndex, &_updateVertex);
			VERTEX_PTR(heapElementToUpdate->element)->_sourceVertexIndex = iSourceVertexInd;
			AddElementToHeap(oVerticesRest, MIN(heapElementToUpdate->key, currentEdge->cost), heapElementToUpdate->element, &_updateVertex);
			free(heapElementToUpdate);
		}
	}	
}

inline void _updateVertex(void *oVertex, long iNewIndex)
{
	VERTEX_PTR(oVertex)->_heapIndex = iNewIndex;
}

int DisposeGraphWithVerticesData(SimpleGraph *oGraph)
{
	TraverseAndDisposeList(&oGraph->vertices, &_freeVertexData);
	DisposeListWithElements(&oGraph->edges);
	return 0;
}

int DisposeGraphMemoryOnly(SimpleGraph *oGraph)
{
	TraverseAndDisposeList(&oGraph->vertices, &_freeVertexOnly);
	DisposeListWithElements(&oGraph->edges);
	return 0;
}
