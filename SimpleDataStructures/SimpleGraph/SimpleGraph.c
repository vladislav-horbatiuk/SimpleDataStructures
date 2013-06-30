//
//  SimpleGraph.c
//  
//
//  Created by Vladyslav Gorbatiuk on 12.01.13.
//
//

#include "SimpleDataStructures/SimpleGraph/SimpleGraph.h"

inline void _updateVertex(void*, long);

/*
	USAGE NOTES:
	Performs a search through the graph iGraph using the following algorithm:
	1) Start with a frontier, that has only one vertex at index iStartInd.
	2) Find the criterion for an every edge, that goes outside the frontier
	(i.e. its source vertex is IN the frontier, while its destination vertex
	is outside).
	3) Add the edge with minimum criterion value together with its destination
	vertex to the frontier. Beforehand, call updateFunc on the minimum edge.
	4) If there are left vertices (not in the frontier) - go to step 1.
	The function returns the total sum of criteria for all added edges. It
	also fills in the passed in pointer to the graph structure oGraph with
	a "backbone" - all edges and vertices that were added to the frontier.
	For instance, if the criteria is the edge's cost, then the filled in
	"backbone" is a graph's MST and a returned value is a sum of MST's edges
	costs.
 */
static inline double _GreedyCriterionSearch(SimpleGraph *iGraph,long iStartInd,
	double (*criterion) (tEdge* iEdge), void (*updateFunc) (tEdge* iEdge),
	SimpleGraph* oGraph);

static inline void _updateAdjacentVerticesInHeap(tVertex*, long, SimpleHeap*,
	double (*) (tEdge*));

static inline void _fillHeapWithVertices(SimpleList*, SimpleHeap*, long);

static inline double _addNewVertexToFrontier(SimpleHeap*,double (*) (tEdge*),
	void (*) (tEdge*), SimpleGraph*);

static inline long _addVertexAndEdgeToFrontier(SimpleGraph*, tVertex*,
	void (*) (tEdge*));

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

static double _edgeCostCriterion(tEdge *iEdge)
{
	return iEdge->cost;
}

static double _pathLengthCriterion(tEdge *iEdge)
{
	return iEdge->cost + iEdge->source->_pathLength;
}

static void _DijkstraUpdateFunc(tEdge* iEdge)
{
	iEdge->dest->_pathLength = iEdge->source->_pathLength + iEdge->cost;
}

int InitGraph(SimpleGraph *oGraph, long iVerticesNum, long iEdgesNum)
{
	int res = InitList(&oGraph->vertices, iVerticesNum);
	res = res || InitList(&oGraph->edges, iEdgesNum);
	if (res)
		return MALLOC_ERROR;
	return 0;
}

int AddVertexWithData(SimpleGraph *oGraph,void *iData, long iEdgesNum)
{
	tVertex *newVertex = malloc(1 * sizeof(tVertex));
	if (!newVertex)
		return MALLOC_ERROR;
	newVertex->data = iData;
	newVertex->_heapIndex = newVertex->_prevVertexInd = UNSET_VALUE;
	newVertex->_edgeCost = newVertex->_pathLength = 0;

	if (InitList(&newVertex->edges, iEdgesNum))
		return MALLOC_ERROR;
	newVertex->vertexIndex = oGraph->vertices.currentNum;
	if (AddElement(&oGraph->vertices, newVertex))
		return MALLOC_ERROR;
	return 0;
}

int AddEdge(SimpleGraph *oGraph, long iSourceVertexInd, long iDestVertexInd, double iCost)
{
	long errorCode = 0;
	tEdge *newEdge = malloc(1 * sizeof(tEdge));
	if (!newEdge)
		return MALLOC_ERROR;
	tVertex *source = VERTEX_PTR(GetElementAt(&oGraph->vertices, iSourceVertexInd));
	tVertex *dest = VERTEX_PTR(GetElementAt(&oGraph->vertices, iDestVertexInd));
	if (!source || !dest)
	{
		errorCode = INDEX_OUT_OF_BOUNDS;
		goto END;
	}
	newEdge->source = source; newEdge->dest = dest; newEdge->cost = iCost;
	if (AddElement(&oGraph->edges, newEdge))
	{
		errorCode = MALLOC_ERROR;
		goto END;
	}
	if (AddElement(&source->edges, newEdge))
	{
		PopAt(&oGraph->edges, oGraph->edges.currentNum - 1);
		errorCode = MALLOC_ERROR;
		goto END;
	}
	if (AddElement(&dest->edges, newEdge))
	{
		PopAt(&oGraph->edges, oGraph->edges.currentNum - 1);
		PopAt(&source->edges, source->edges.currentNum - 1);
		errorCode = MALLOC_ERROR;
	}
	END:
	if (errorCode != 0)
		free(newEdge);
	return errorCode;
}

double MinimumSpanningTree(SimpleGraph *iGraph, SimpleGraph *oMST)
{
	return _GreedyCriterionSearch(iGraph, 0, &_edgeCostCriterion, NULL, oMST);
}

int ReadGraphFromFile(char *iFileName, SimpleGraph *oGraph, int iDirect)
{
    long i, errorCode = 0;
    FILE *aFile = fopen(iFileName, "r");
    if (!aFile)
        return FILE_OPEN_ERROR;

    long vertNum, edgeNum, currSource, currDest, *element;
    double currCost;

    /* Read number of vertices and edges from file */
    if (fscanf(aFile, "%ld %ld", &vertNum, &edgeNum) != 2)
    {
    	errorCode = INCONSISTENT_FILE;
    	goto CLOSE;
    }
    if (InitGraph(oGraph, vertNum, edgeNum) != 0)
    {
    	errorCode = MALLOC_ERROR;
    	goto CLOSE;
    }
    /* Add vertices to graph */
    for (i = 0; i < vertNum; ++i)
    {
        if (AddVertexWithData(oGraph, NULL, DEFAULT_EDGES_NUMBER) != 0)
        {
        	errorCode = MALLOC_ERROR;
        	goto DISPOSE_AND_CLOSE;
	    }
    }
    for (i = 0; i < edgeNum; ++i)
    {
        if (fscanf(aFile, "%ld %ld %lf", &currSource, &currDest, &currCost) != 3)
        {
        	errorCode = INCONSISTENT_FILE;
        	goto DISPOSE_AND_CLOSE;
        }
        AddEdge(oGraph, currSource - 1, currDest - 1, currCost);
        if (!iDirect)
        	AddEdge(oGraph, currDest - 1, currSource - 1, currCost);
    }
    goto CLOSE;

    DISPOSE_AND_CLOSE:
    DisposeGraphMemoryOnly(oGraph);

    CLOSE:
    fclose(aFile);

    return errorCode;
}

double* ShortestPathUsingDijkstra(SimpleGraph *iGraph, long iSourceVertexIndex)
{
	return NULL;
}

double* ShortestPathUsingBellmanFord(SimpleGraph *iGraph, long iSourceVertexIndex, int *oFoundNegativeCycle)
{
	return NULL;
}

double* AllPairsShortestPath(SimpleGraph *iGraph)
{
	return NULL;
}

static inline double _GreedyCriterionSearch(SimpleGraph *iGraph,long iStartInd,
	double (*criterion) (tEdge* iEdge), void (*updateFunc) (tEdge* iEdge),
	SimpleGraph *oGraph)
{
	double result = 0;
	SimpleList *vertices = &iGraph->vertices;
	long i, vertNum = vertices->currentNum;

	InitGraph(oGraph, vertNum, vertNum);

	SimpleHeap verticesRest;
	_fillHeapWithVertices(vertices, &verticesRest, iStartInd);

	do
	{
		result += 
		_addNewVertexToFrontier(&verticesRest, criterion, updateFunc, oGraph);
	} while (verticesRest.elements.currentNum);
	DisposeHeapWithElements(&verticesRest);
	return result;
}

static inline void _fillHeapWithVertices(SimpleList *iVertices, 
	SimpleHeap *oHeap, long iStartInd)
{
	long i, vertNum = iVertices->currentNum;
	InitHeap(oHeap, vertNum);
	/* The vertex at index iStartInd is added with weight=0, since it will
	 * serve as a source vertex to start building MST.
	 */
	AddElementToHeap(oHeap, 0, GetElementAt(iVertices, iStartInd),
					&_updateVertex);
	/* Add the rest of vertices with 'infinite' key, since 
	 * they aren't reachable at all in the beginning of an algorithm.
	 */
	for (i = 0; i < vertNum; ++i)
	{
		if (i != iStartInd)
			AddElementToHeap(oHeap, DBL_MAX, GetElementAt(iVertices, i),
							&_updateVertex);
	}
}

static inline double _addNewVertexToFrontier(SimpleHeap *oVerticesRest,
	double (*criterion) (tEdge* edge), void (*updateFunc) (tEdge* iEdge),
	SimpleGraph *oGraph)
{
	tHeapElement *minHeapElement = 
		ExtractMinElement(oVerticesRest, &_updateVertex);
	double criterionValue = minHeapElement->key;
	tVertex *newVertex = VERTEX_PTR(minHeapElement->element);
	long iNewVertexInd = 
		_addVertexAndEdgeToFrontier(oGraph, newVertex, updateFunc);

	newVertex->_heapIndex = UNSET_VALUE;

	_updateAdjacentVerticesInHeap(newVertex, iNewVertexInd, 
								oVerticesRest, criterion);
	free(minHeapElement);
	return criterionValue;
}

static inline long _addVertexAndEdgeToFrontier(SimpleGraph *oGraph,
	tVertex *iNewVertex, void (*updateFunc) (tEdge* iEdge))
{
	AddVertexWithData(oGraph, iNewVertex->data, iNewVertex->edges.currentNum);
	long newVertexInd = oGraph->vertices.currentNum - 1;
	/* Set the vertex index of newly inserted vertex to the old one, so that
	 * we can find a correspondence between vertices in new "backbone" graph
	 * and vertices in the input graph.
	 */
	VERTEX_PTR(GetElementAt(&oGraph->vertices, newVertexInd))->vertexIndex = 
		iNewVertex->vertexIndex;
	/* Add a corresponding edge (if it isn't the first added vertex, i.e. its
	 * index is > 0).
	 */
	if (newVertexInd > 0 && (iNewVertex->_prevVertexInd != UNSET_VALUE))
	{
		AddEdge(oGraph, iNewVertex->_prevVertexInd, newVertexInd, 
			iNewVertex->_edgeCost);
		/* Update vertex info needed for particular greedy search algorithm,
		 * using passed in function pointer updateFunc (for instance, Dijkstra
		 * algorithm would remember a shortest path length to iNewVertex).
		 */
		if (updateFunc != NULL)
			updateFunc(EDGE_PTR(GetElementAt(&oGraph->edges,
				oGraph->edges.currentNum - 1)));
	}

	return newVertexInd;
}

static inline void _updateAdjacentVerticesInHeap(tVertex *iNewVertex, 
	long iNewInd, SimpleHeap *oVerticesRest, 
	double (*criterion) (tEdge *iEdge))
{
	tEdge *currentEdge;
	tHeapElement *heapElementToUpdate;
	tVertex *adjacentVertex;
	long i;
	for (i = 0; i < iNewVertex->edges.currentNum; ++i)
	{
		currentEdge = GetElementAt(&iNewVertex->edges, i);
		if (currentEdge->source != iNewVertex)
			/* Incoming edge, while we need outgoing */
			continue;
		adjacentVertex = currentEdge->dest;
		if (adjacentVertex->_heapIndex == UNSET_VALUE)
			/* Vertex not in the heap - no need to update */
			continue;
		heapElementToUpdate = 
			PopHeapElementAtIndex(oVerticesRest, adjacentVertex->_heapIndex,
								&_updateVertex);
		VERTEX_PTR(heapElementToUpdate->element)->_prevVertexInd = iNewInd;
		VERTEX_PTR(heapElementToUpdate->element)->_edgeCost = currentEdge->cost;
		AddElementToHeap(oVerticesRest, 
						MIN(heapElementToUpdate->key, criterion(currentEdge)),
						heapElementToUpdate->element, &_updateVertex);
		free(heapElementToUpdate);
	}	
}

inline void _updateVertex(void *oVertex, long iNewIndex)
{
	VERTEX_PTR(oVertex)->_heapIndex = iNewIndex;
}

inline void PrintEdgeInfo(tEdge* iEdge)
{
    printf("Source vertex index:%ld, dest vertex index:%ld, edge cost=%lf\n",
    iEdge->source->vertexIndex, iEdge->dest->vertexIndex, iEdge->cost);
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
