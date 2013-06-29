//
//  SimpleGraph.h
//  
//
//  Created by Vladyslav Gorbatiuk on 11.01.13.
//
//

#ifndef _SimpleGraph_h
#define _SimpleGraph_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SimpleDataStructures/SimpleList/SimpleList.h"
#include "SimpleDataStructures/SimpleHeap/SimpleHeap.h"
#include "SimpleDataStructures/SimpleHashMap/SimpleHashMap.h"
#include "SimpleDataStructures/common_defines.h"

#define VERTEX_PTR(x) ((tVertex*) x)
#define EDGE_PTR(x) ((tEdge*) x)
#define UNSET_VALUE -1
#define DEFAULT_EDGES_NUMBER 10

typedef struct
{
	void *data;
	SimpleList edges;
	long vertexIndex;
	long _heapIndex;
	long _sourceVertexIndex;
} tVertex;

typedef struct
{
	tVertex *source;
	tVertex *dest;
	double cost;
} tEdge;

typedef struct
{
	SimpleList vertices;
	SimpleList edges;
} SimpleGraph;

int InitGraph(SimpleGraph*, long, long);

int AddVertexWithData(SimpleGraph*, void*, long);

int AddEdge(SimpleGraph*, long, long, double);

double MinimumSpanningTree(SimpleGraph*, SimpleGraph*);
/*
	USAGE NOTES:
	Reads graph from a file in the special format.
	The format goes as follows:
	"""
	N M
	N11 N12 C1
	N21 N22 C2
	...
	NM1 NM2 CM
	"""
	where N - number of vertices, M - number of edges,
	Ni1, Ni2 - source and dest vertices of an edge i,
	Ci - its cost. Vertices indices should start from 1.
	INPUTS:
	-iFileName - name of the file, containing graph.
	-oGraph - pointer to the SimpleGraph structure.
	RETURNS:
	- 0, if everything went well, error code otherwise.
*/
int ReadGraphFromFile(char *iFileName, SimpleGraph *oGraph);

/*
	USAGE NOTES:
	Solves the single source shortest path problem for
	a given graph with non-negative edge length - i.e.
	finds shortest distances to every vertex from a 
	given source vertex. Uses Dijkstra algorithm with
	binary heaps, thus reaching O(M*logN) complexity.
	For a graph with at least one negative edge length
	there is no guarantee that the function will return
	correct shortest paths.
*/
double* ShortestPathUsingDijkstra(SimpleGraph *iGraph, long iSourceVertexIndex);

/*
	USAGE NOTES:
	Solves the single source shortest path problem for
	a given graph without negative-cost cycles - i.e.
	finds shortest distances to every vertex from a 
	given source vertex. Uses Bellman-Ford algorithm,
	thus reaching O(M*N) complexity. If graph has at
	least one negative-cost cycle the returned values
	aren't guaranteed to be correct, and the variable
	oFoundNegativeCycle will be set to 1 (0 otherwise).  
*/
double* ShortestPathUsingBellmanFord(SimpleGraph *iGraph, long iSourceVertexIndex, int *oFoundNegativeCycle);

/*
	USAGE NOTES:
	Solves the all-pairs shortest path problem for a
	given graph using Johnson algorithm.'Solving' means:
	a)for a graph with no negative-cost cycles the function
	will return an NxN matrix of distances (N - number
	of vertices in the source graph),  where number at
	indices i,j will correspond to the shortest distance 
	between vertices i and j in the original graph. Matrix
	is given in a row-major way - i.e. first row (N numbers)
	followed by a second row (N numbers) and so on.
	b) for a graph with at least one negative-cost cycle
	an empty list will be returned (list head pointer==NULL).
*/
double* AllPairsShortestPath(SimpleGraph *iGraph);

int DisposeGraphWithVerticesData(SimpleGraph*);

int DisposeGraphMemoryOnly(SimpleGraph*);

inline void PrintEdgeInfo(tEdge* iEdge);

#endif