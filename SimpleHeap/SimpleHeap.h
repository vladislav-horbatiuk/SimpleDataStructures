//
//  SimpleHeap.h
//  
//
//  Created by Vladyslav Gorbatiuk on 02.01.13.
//
//

#ifndef _SimpleHeap_h
#define _SimpleHeap_h

#define PARENT(x) ((x-1) / 2)
#define LEFT_CHILD(x) (2*x + 1)
#define RIGHT_CHILD(x) (2*x + 2)
#define HEAP_ELEMENT(x) ((tHeapElement*) x)

#include "SimpleList.h"
#include "float.h"

typedef struct 
{
	double key;
	void* element;
} tHeapElement;

typedef struct
{
	SimpleList elements;
} SimpleHeap;

int InitHeap(SimpleHeap*, long);

int AddElementToHeap(SimpleHeap*, double, void*, void (*) (void*, long));

tHeapElement* ExtractMinElement(SimpleHeap*, void (*) (void*, long));

tHeapElement* LookupMinElement(SimpleHeap*);

tHeapElement* PopHeapElementAtIndex(SimpleHeap*, long, void (*) (void*, long));

int DisposeHeapWithElements(SimpleHeap*);

int DisposeOnlyHeapMemory(SimpleHeap*);
#endif