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
    long input, i, source, dest, j;
    double cost;
    long *element;
    char strInp[256];
    tVertex *currVertex;
    tEdge *currEdge;
    InitGraph(&aGraph, 10, 10);
    while (1) {
        printf("0. Start new graph (clear existing one).\n");
        printf("1. Add vertex.\n");
        printf("2. Add edge.\n");
        printf("3. Get incident edges for vertex.\n");
        printf("4. Read graph from file.\n");
        printf("5. Find the MST of a graph.\n");
        printf("6. Find the shortest paths using Dijkstra algorithm\
 (works only for graphs with non-negative weights).\n");
        printf("7. Find the shortest paths using Bellman-Ford algorithm\
(works only for graphs without negative-cost cycles).\n");
        printf("8. Find the all-pair shortest paths using Johnson's algorithm\
(works only for graphs without negative-cost cycles).\n");
        printf("9. Exit.\n");
        scanf("%ld", &input);
        if (input == 9) {
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
            printf("Directional (1) or undirectional (0):\n");
            scanf("%ld", &input);
            SimpleGraph tmp;
            if (ReadGraphFromFile(strInp, &tmp, 
                (input == 1 ? DIRECTIONAL : UNDIRECTIONAL)))
            { printf("Incorrect file name.\n"); continue; }
            DisposeGraphWithVerticesData(&aGraph);
            aGraph = tmp;
        }
        else if (5 == input)
        {
            SimpleGraph MST;
            double result = MinimumSpanningTree(&aGraph, &MST);
            printf("MST EDGES:\n");
            elements = aGraph.edges;
            for (i = 0; i < elements.currentNum; ++i)
            {   
                currEdge = EDGE_PTR(GetElementAt(&elements, i));
                PrintEdgeInfo(currEdge);
            }
            printf("The total sum of edges cost in MST:%lf\n", result);
            DisposeGraphMemoryOnly(&MST);
            continue;
        }
        else if (6 == input)
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
            for (i = 0; i < aGraph.vertices.currentNum; ++i)
            {
                printf("%lg ", shortestPaths[i]);
            }
            printf("\n");
            free(shortestPaths);
            continue;
        }
        else if (7 == input)
        {
            printf("Enter starting vertex index:\n");
            scanf("%ld", &input);
            if (input < 0 || input > (aGraph.vertices.currentNum - 1))
            {
                 printf("Index out of bounds..\n");
                 continue;
            }
            int negativeCycle;
            double *shortestPaths = ShortestPathUsingBellmanFord(&aGraph, input, &negativeCycle);
            if (!shortestPaths)
            {
                printf("Something went wrong during Dijkstra algorithm..\n");
                continue;
            }
            if (negativeCycle)
                printf("Found negative cycle, you can't now trust the paths.\n");

            printf("Shortest paths starting from vertex at index %ld \
(%lg means infinity - no path):\n", input, DBL_MAX);
            for (i = 0; i < aGraph.vertices.currentNum; ++i)
            {
                printf("%lg ", shortestPaths[i]);
            }
            printf("\n");
            free(shortestPaths);
            continue;
        }
        else if (8 == input)
        {
            int negativeCycle;
            double *shortestPaths = AllPairsShortestPath(&aGraph, &negativeCycle);
            if (!shortestPaths)
            {
                if (negativeCycle)
                    printf("Found negative cycle.\n");
                else
                    printf("Something went wrong during algorithm's execution..\n");
                continue;
            }
            double minDistance = DBL_MAX, currDistance;
            for (i = 0; i < aGraph.vertices.currentNum; ++i)
            {
                printf("Shortest distances from vertex %ld\
 (%lg means infinity - no path):\n", i, DBL_MAX);
                for (j = 0; j < aGraph.vertices.currentNum; ++j)
                {
                    currDistance = shortestPaths[i*aGraph.vertices.currentNum+j];
                    if (currDistance < minDistance)
                     minDistance = currDistance;
                    printf("%lg ", currDistance);
                }
                printf("\n");
            }
            printf("SHORTEST DISTANCE IN WHOLE GRAPH:%lf.\n", minDistance);
            free(shortestPaths);
            continue;
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