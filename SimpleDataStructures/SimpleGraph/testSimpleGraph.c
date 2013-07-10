//
//  testSimpleGraph.c
//  
//
//  Created by Vladyslav Gorbatiuk on 13.01.13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleDataStructures/SimpleGraph/SimpleGraph.h"

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
        printf("5. Find the shortest paths using Dijkstra algorithm\
 (works only for graphs with non-negative weights).\n");
        printf("6. Exit.\n");
        scanf("%ld", &input);
        if (input == 6) {
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
                    PrintEdgeInfo(currEdge);
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
            if (ReadGraphFromFile(strInp, &fileGraph, UNDIRECTIONAL))
            { printf("Incorrect file name.\n"); continue; }

            SimpleGraph MST;
            double result = MinimumSpanningTree(&fileGraph, &MST);
            printf("MST EDGES:\n");
            elements = fileGraph.edges;
            for (i = 0; i < elements.currentNum; ++i)
            {   
                currEdge = EDGE_PTR(GetElementAt(&elements, i));
                PrintEdgeInfo(currEdge);
            }
            printf("The total sum of edges cost in MST:%lg\n", result);
            DisposeGraphWithVerticesData(&fileGraph);
            DisposeGraphMemoryOnly(&MST);
            continue;
        }
        else if (5 == input)
        {
            printf("Enter starting vertex index:\n");
            scanf("%ld", &input);
            if (input < 0 || input > (aGraph.vertices.currentNum - 1))
            {
                 printf("Index out of bounds..\n");
                 continue;
            }
            double *shortestPaths = ShortestPathUsingDijkstra(&aGraph, input);
            if (!shortestPaths)
            {
                printf("Something went wrong during Dijkstra algorithm..\n");
                continue;
            }
            printf("Shortest paths starting from vertex at index %ld \
(%lg means infinity - no path):\n", input, DBL_MAX);
            double minPath = DBL_MAX;
            for (i = 0; i < aGraph.vertices.currentNum; ++i)
            {
                printf("%lg ", shortestPaths[i]);
            }
            printf("\n");
            free(shortestPaths);
        }
        printf("Current graph vertices and edges:\n");
        printf("VERTICES number:%ld.\n", aGraph.vertices.currentNum);
        printf("EDGES:\n");
        elements = aGraph.edges;
        for (i = 0; i < elements.currentNum; ++i)
        {   
            currEdge = EDGE_PTR(GetElementAt(&elements, i));
            PrintEdgeInfo(currEdge);
        }
    }
    DisposeGraphWithVerticesData(&aGraph);
    return 0;
}