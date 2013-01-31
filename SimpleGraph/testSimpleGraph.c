//
//  testSimpleGraph.c
//  
//
//  Created by Vladyslav Gorbatiuk on 13.01.13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleGraph.h"

int readGraphFromFile(char *iFileName, SimpleGraph *oGraph)
{
    long i;
    FILE *aFile = fopen(iFileName, "r");
    if (!aFile)
        return -1;
    long vertNum, edgeNum, currSource, currDest, *element;
    double currCost;
    fscanf(aFile, "%ld %ld", &vertNum, &edgeNum);
    InitGraph(oGraph, vertNum, edgeNum);
    for (i = 0; i < vertNum; ++i)
    {
        element = malloc(1*sizeof(long));
        *element = i + 1;
        AddVertexWithData(oGraph, element, 10);
    }
    for (i = 0; i < edgeNum; ++i)
    {
        fscanf(aFile, "%ld %ld %lf", &currSource, &currDest, &currCost);
        AddEdge(oGraph, currSource - 1, currDest - 1, currCost);
    }
    return 0;
}

int main()
{
    SimpleGraph aGraph;
    SimpleList elements;
    long input, i, source, dest;
    double cost;
    long *element;
    char strInp[256];
    tVertex *currVertex;
    tEdge *currEdge;
    InitGraph(&aGraph, 10, 10);
    while (1) {
        printf("1. Add vertex.\n");
        printf("2. Add edge.\n");
        printf("3. Get incident edges for vertex.\n");
        printf("4. Find the MST of a graph from file.\n");
        printf("5. Exit.\n");
        scanf("%ld", &input);
        if (input == 5) {
            break;
        }
        if (1 == input)
        {
            printf("Enter vertex data (long number):\n");
            scanf("%ld", &input);
            element = malloc(sizeof(long));
            if (!element) { printf("element malloc error\n"); break; }
            *element = input;
            if (!AddVertexWithData(&aGraph, element, 10))
                printf("Added new element successfully\n");
            else { printf("Failed to add vertex, probably memory problems...\n"); break; }
                
        }
        else if (2 == input)
        {
            printf("Enter the index of source vertex:\n");
            scanf("%ld", &source);
            printf("Enter the index of destination vertex:\n");
            scanf("%ld", &dest);
            printf("Enter the edge's cost (double):\n");
            scanf("%lf", &cost);
            if ((i = AddEdge(&aGraph, source, dest, cost)) == 0)
                printf("Added edge successfully.\n");
            else if (i == -1)
                { printf("Failed to add edge because of the memory problems, so exiting...\n"); break; }
            else if (i == INDEX_OUT_OF_BOUNDS)
                printf("Failed to add edge, because one of the vertex indices was incorrect...\n");
        }
        else if (3 == input)
        {
            printf("Enter vertex, which incident edges you want to find:\n");
            scanf("%ld", &input);
            if ((currVertex = GetElementAt(&aGraph.vertices, input)))
            {
                elements = currVertex->edges;
                for (i = 0;i < elements.currentNum; ++i)
                {
                    currEdge = EDGE_PTR(GetElementAt(&elements, i));
                    printf("Source vertex data:%ld, dest vertex data:%ld, edge cost=%lf\n", *((long*) currEdge->source->data), *((long*) currEdge->dest->data), currEdge->cost);                    
                }
            }
            else
                printf("Incorrect vertex index...\n");
            continue;
        }
        else if (4 == input)
        {
            printf("Enter file name:\n");
            scanf("%s", strInp);
            SimpleGraph fileGraph;
            if (readGraphFromFile(strInp, &fileGraph))
            { printf("Incorrect file name.\n"); continue; }

            SimpleGraph MST;
            double result = MinimumSpanningTree(&fileGraph, &MST);
            printf("The total sum of edges cost in MST:%lf\n", result);
            printf("MST EDGES:\n");
            elements = fileGraph.edges;
            for (i = 0; i < elements.currentNum; ++i)
            {   
                currEdge = EDGE_PTR(GetElementAt(&elements, i));
                printf("Source vertex data:%ld, dest vertex data:%ld, edge cost=%lf\n", *(long*)currEdge->source->data, *(long*)currEdge->dest->data, currEdge->cost);
            }
            DisposeGraphWithVerticesData(&fileGraph);
            DisposeGraphMemoryOnly(&MST);
            continue;
        }

        printf("Current graph vertices and edges:\n");
        printf("VERTICES:\n");
        elements = aGraph.vertices;
        for (i = 0; i < elements.currentNum; ++i)
        {
            currVertex = VERTEX_PTR(GetElementAt(&elements, i));
        	printf("Index:%ld, data:%ld\n", i, *((long*)currVertex->data));
        }
        printf("EDGES:\n");
        elements = aGraph.edges;
        for (i = 0; i < elements.currentNum; ++i)
        {   
            currEdge = EDGE_PTR(GetElementAt(&elements, i));
            printf("Source vertex data:%ld, dest vertex data:%ld, edge cost=%lf\n", *((long*) currEdge->source->data), *((long*) currEdge->dest->data), currEdge->cost);
        }
    }
    DisposeGraphWithVerticesData(&aGraph);
    return 0;
}