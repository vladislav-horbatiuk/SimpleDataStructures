//
//  SimpleGraph.h
//  
//
//  Created by Vladyslav Gorbatiuk on 11.01.13.
//
//

#ifndef _SimpleGraph_h
#define _SimpleGraph_h
#include "SimpleDataStructures/SimpleList/SimpleList.h"
#include "SimpleDataStructures/SimpleHeap/SimpleHeap.h"
#include "SimpleDataStructures/SimpleHashMap/SimpleHashMap.h"

#define VERTEX_PTR(x) ((tVertex*) x)
#define EDGE_PTR(x) ((tEdge*) x)
#define INDEX_OUT_OF_BOUNDS 1
#define MAX_POINTER_LENGTH 15
#define MIN(x,y) (((x) < (y))? (x) : (y))
#define UNSET_VALUE -1

typedef struct
{
	void *data;
	SimpleList edges;
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
int DisposeGraphWithVerticesData(SimpleGraph*);
int DisposeGraphMemoryOnly(SimpleGraph*);
#endif