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

static inline double _pathLength(tEdge *iEdge)
{
	return iEdge->cost + iEdge->source->_pathLength;
}

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
	return _pathLength(iEdge);
}

static void _DijkstraUpdateFunc(tEdge* iEdge)
{
	iEdge->dest->_pathLength = _pathLength(iEdge);
}

static inline void _resetPrivateFields(tVertex *oVertex)
{
	oVertex->_heapIndex = oVertex->_prevVertexInd = UNSET_VALUE;
	oVertex->_edgeCost = oVertex->_pathLength = DBL_MAX;
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
	_resetPrivateFields(newVertex);

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
	double *shortestPaths = malloc(iGraph->vertices.currentNum * sizeof(double));
	if (shortestPaths == NULL)
		return NULL;

	ShortestPathUsingDijkstra2(iGraph, iSourceVertexIndex, shortestPaths);
	return shortestPaths;
}

int ShortestPathUsingDijkstra2(SimpleGraph *iGraph, 
	long iSourceVertexIndex, double *oDistances)
{
	SimpleGraph shortestPathGraph;
	_GreedyCriterionSearch(iGraph, iSourceVertexIndex, &_pathLengthCriterion,
						&_DijkstraUpdateFunc, &shortestPathGraph);

	long i;
	for (i = 0; i < shortestPathGraph.vertices.currentNum; ++i)
	{
		tVertex *currentVertex = GetElementAt(&shortestPathGraph.vertices, i);
		oDistances[currentVertex->vertexIndex] = currentVertex->_pathLength;
		currentVertex->vertexIndex = i;
	}
    /* TODO: For now we just destroy it - consider somehow returning this backbone. */
    DisposeGraphMemoryOnly(&shortestPathGraph);
	return 0;
}

double* ShortestPathUsingBellmanFord(SimpleGraph *iGraph,
		long iSourceVertexIndex, int *oFoundNegativeCycle)
{
	SimpleList *vertices = &iGraph->vertices;
	SimpleList *edges = &iGraph->edges;
	long N = vertices->currentNum;
	long M = edges->currentNum;
	long i,j;
	double *result = malloc(sizeof(double) * N);
	if (!result)
		return NULL;

	*oFoundNegativeCycle = 0;
	int anyImpromevent;

	for (i = 0; i < N; ++i)
		result[i] = DBL_MAX;
	result[iSourceVertexIndex] = 0;
	for (i = 0; i < N; ++i)
	{
		anyImpromevent = 0;
		for (j = 0; j < M; ++j)
		{
			tEdge *currEdge = GetElementAt(edges, j);
			long sourceIndex = currEdge->source->vertexIndex;
			long destIndex = currEdge->dest->vertexIndex;
			double newPathCost = result[sourceIndex] + currEdge->cost;
			if (result[destIndex] > newPathCost)
			{
				result[destIndex] = newPathCost;
				anyImpromevent = 1;
			}
		}
		if (!anyImpromevent)
			break;
	}
	if (i == N)
	{
		*oFoundNegativeCycle = 1;
		free(result);
		result = NULL;
	}
	return result;
}

double* AllPairsShortestPath(SimpleGraph *iGraph, int *oFoundNegativeCycle)
{
	SimpleList *vertices = &iGraph->vertices;
	SimpleList *edges = &iGraph->edges;
	long N = vertices->currentNum;
	long M = edges->currentNum;
	*oFoundNegativeCycle = 0;
	double *result = malloc(sizeof(double) * N * N);
	if (!result)
		return NULL;

	long i,j;
	/* Step 1: Add fictitious vertex s to the graph. */
	AddVertexWithData(iGraph, NULL, N);
	/* Step 2: Connect it with every other vertex using an edge with 0 cost.*/
	for (i = 0; i < N; ++i)
	{
		AddEdge(iGraph, N, i, 0);
	}
	/* Step 3: Run Bellman-Ford algorithm to get shortest-paths distances from
	 * new fictitious vertex to every other vertex.
	 */
	double *distances = ShortestPathUsingBellmanFord(iGraph, 
	 									N, oFoundNegativeCycle);
	if (*oFoundNegativeCycle)
	{
		free(result);
		result = NULL;
		goto END;
	}
	/* Step 4: Update a cost of every edge in the source graph based on
	 * distances, computed during the previous step.
	 */
	for (i = 0; i < M; ++i)
	{
		tEdge* currEdge = GetElementAt(edges, i);
		long sourceIndex = currEdge->source->vertexIndex;
		long destIndex = currEdge->dest->vertexIndex;
		currEdge->cost += distances[sourceIndex] - distances[destIndex];
	}
	/* Step 5: Run Dijkstra's algo N times, in order to get shortest paths
	 * distances between every pair of vertices. Previously, we need to delete
	 * fictitious vertex and its edges as well.
	 */
END:
	;
	tVertex *fictitiousVertex = PopAt(vertices, N);
	_freeVertexOnly(fictitiousVertex);

	for (i = M + N - 1; i >= M; --i)
	{
		tEdge *addedEdge = PopAt(edges, i);
		free(addedEdge);
	}
	if (!result)
		/* We are here because of some error - so just return NULL. */
		return result;

	for (i = 0; i < N; ++i)
	{
		ShortestPathUsingDijkstra2(iGraph, i, result + i * N);
	}
	/* Restore original cost for every edge. */
	for (i = 0; i < M; ++i)
	{
		tEdge *currEdge = GetElementAt(edges, i);
		long sourceIndex = currEdge->source->vertexIndex;
		long destIndex = currEdge->dest->vertexIndex;
		currEdge->cost -= distances[sourceIndex] - distances[destIndex];
	}
	/* Restore the actual shortest paths values. */
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			result[i*N+j] -= distances[i] - distances[j];

	return result;
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
	/* Hack, since this routine is supposed to be general, while _pathLength
	 * internal field is used only by Dijkstra algorithm. But this is the
	 * easiest way to make it work for now.
	 */
	VERTEX_PTR(GetElementAt(vertices, iStartInd))->_pathLength = 0;

	do
	{
		result += 
		_addNewVertexToFrontier(&verticesRest, criterion, updateFunc, oGraph);
	} while (verticesRest.elements.currentNum);
	DisposeHeapWithElements(&verticesRest);
	/* Reset private fields (to defaults) for every vertex. */
	for (i = 0; i < vertNum; ++i)
	{
		_resetPrivateFields(GetElementAt(vertices, i));
	}

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
	if (newVertexInd > 0 && (iNewVertex->_prevVertexInd != UNSET_VALUE))
	{
		/* Add a corresponding edge (if it isn't the first added vertex, i.e. its
		 * index is > 0).
		 */
		AddEdge(oGraph, iNewVertex->_prevVertexInd, newVertexInd, 
			iNewVertex->_edgeCost);
		if (updateFunc != NULL)
		{
			/* Update vertex info needed for particular greedy search algorithm,
			 * using passed in function pointer updateFunc (for instance, Dijkstra
			 * algorithm would remember a shortest path length to iNewVertex).
			 */
			tEdge fictitiousEdge;
			fictitiousEdge.source = 
				GetElementAt(&oGraph->vertices, iNewVertex->_prevVertexInd);
			fictitiousEdge.dest = iNewVertex;
			fictitiousEdge.cost = iNewVertex->_edgeCost;
			updateFunc(&fictitiousEdge);
		}
	}

	tVertex *insertedVertex = GetElementAt(&oGraph->vertices, newVertexInd);
	/* Set the vertex index of newly inserted vertex to the old one, so that
	 * we can find a correspondence between vertices in new "backbone" graph
	 * and vertices in the input graph.
	 */
	insertedVertex->vertexIndex = iNewVertex->vertexIndex;
	insertedVertex->_pathLength = iNewVertex->_pathLength;

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
		adjacentVertex = currentEdge->dest;
		if (adjacentVertex->_heapIndex == UNSET_VALUE)
			/* Vertex not in the heap - no need to update */
			continue;
		heapElementToUpdate = 
			PopHeapElementAtIndex(oVerticesRest, adjacentVertex->_heapIndex,
								&_updateVertex);
		
		double criterionValue = criterion(currentEdge);
		if (heapElementToUpdate->key > criterionValue)
		{
			VERTEX_PTR(heapElementToUpdate->element)->_prevVertexInd = iNewInd;
			VERTEX_PTR(heapElementToUpdate->element)->_edgeCost = currentEdge->cost;
			AddElementToHeap(oVerticesRest, criterionValue,
						heapElementToUpdate->element, &_updateVertex);
		}
		else
			AddElementToHeap(oVerticesRest, heapElementToUpdate->key,
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
