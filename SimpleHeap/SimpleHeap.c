//
//  SimpleHeap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 02.01.13.
//
//
#include "SimpleDataStructures/SimpleHeap/SimpleHeap.h"
#include <stdlib.h>

static inline void GoUpFromIndex(SimpleList*, long, void (*) (void*, long));
static inline void GoDownFromIndex(SimpleList*, long, void (*) (void*, long));


int InitHeap(SimpleHeap *oHeap, long iCount)
{
	InitList(&oHeap->elements, iCount);
}


int AddElementToHeap(SimpleHeap *oHeap, double iKey, void *iElement, void (*updateFunc) (void*, long))
{
	SimpleList *elements = &oHeap->elements;
	tHeapElement *newElement = malloc(1 * sizeof(tHeapElement));
	if (!newElement)
		return -1;
	newElement->key = iKey; newElement->element = iElement;
	if (AddElement(elements, newElement))
		return -1;
	GoUpFromIndex(elements, elements->currentNum - 1, updateFunc);
	return 0;
}


tHeapElement* ExtractMinElement(SimpleHeap *oHeap, void (*updateFunc) (void*, long))
{
	return PopHeapElementAtIndex(oHeap, 0, updateFunc);
}


tHeapElement* LookupMinElement(SimpleHeap *iHeap)
{
	return HEAP_ELEMENT(GetElementAt(&iHeap->elements, 0));
}


tHeapElement* PopHeapElementAtIndex(SimpleHeap *oHeap, long iIndex, void (*updateFunc) (void*, long))
{
	SimpleList *elements = &oHeap->elements;
	long elementsCount = elements->currentNum;
	if (iIndex < 0 || iIndex >= elementsCount)
		return NULL;
	tHeapElement *returnValue = HEAP_ELEMENT(GetElementAt(elements, iIndex));
	tHeapElement *last = HEAP_ELEMENT(PopAt(elements, elementsCount - 1));
	elementsCount--;
	if (elementsCount > iIndex)
	{
		SetElementAt(elements, iIndex, last);
		long parent = PARENT(iIndex);
		if (parent < iIndex && HEAP_ELEMENT(GetElementAt(elements, parent))->key > last->key)
			GoUpFromIndex(elements, iIndex, updateFunc);
		else
			GoDownFromIndex(elements, iIndex, updateFunc);
	}
	return returnValue;
}


int DisposeHeapWithElements(SimpleHeap *oHeap)
{
	long i = 0;
	tHeapElement *current = HEAP_ELEMENT(GetElementAt(&oHeap->elements, i));
	for (; current!=NULL; current=GetElementAt(&oHeap->elements, ++i))
		free(current->element);
	DisposeListWithElements(&oHeap->elements);
	return 0;
}

int DisposeOnlyHeapMemory(SimpleHeap *oHeap)
{
	DisposeListWithElements(&oHeap->elements);
	return 0;
}

static inline void GoUpFromIndex(SimpleList *oHeapElements, long iIndex, void (*updateFunc) (void*, long))
{
	tHeapElement *current = HEAP_ELEMENT(GetElementAt(oHeapElements, iIndex));
	if (updateFunc)
	{
		updateFunc(current->element, iIndex);
	}
	double currentKey = current->key;
	while (iIndex > 0)
	{
		long parentIndex = PARENT(iIndex);
		tHeapElement *parent = HEAP_ELEMENT(GetElementAt(oHeapElements, parentIndex));
		if (parent->key < currentKey)
			break;
		SetElementAt(oHeapElements, iIndex, parent);
		SetElementAt(oHeapElements, parentIndex, current);
		if (updateFunc)
		{
			updateFunc(current->element, parentIndex);
			updateFunc(parent->element, iIndex);
		}
		iIndex = parentIndex;
	}
}


static inline void GoDownFromIndex(SimpleList *oHeapElements, long iIndex, void (*updateFunc) (void*, long))
{
		int stop;
		long leftChildInd, rightChildInd, swapInd, elementsCount = oHeapElements->currentNum;
		tHeapElement *toSwap, *current = HEAP_ELEMENT(GetElementAt(oHeapElements, iIndex));
		double rightChildKey, leftChildKey, currentKey = current->key;

		if (updateFunc)
		{
			updateFunc(current->element, iIndex);
		}

		while ((leftChildInd = LEFT_CHILD(iIndex)) < elementsCount)
		{
			stop = 1;
			leftChildKey = HEAP_ELEMENT(GetElementAt(oHeapElements, leftChildInd))->key;
			rightChildKey = DBL_MAX;
			if ((rightChildInd = RIGHT_CHILD(iIndex)) < elementsCount)
				stop = (rightChildKey = HEAP_ELEMENT(GetElementAt(oHeapElements, rightChildInd))->key) > currentKey;
			stop = stop && (leftChildKey > currentKey);
			if (stop)
				break;
			swapInd = (leftChildKey < rightChildKey || rightChildInd >= elementsCount) ? leftChildInd : rightChildInd;
			toSwap = HEAP_ELEMENT(GetElementAt(oHeapElements, swapInd));
			SetElementAt(oHeapElements, iIndex, toSwap);
			SetElementAt(oHeapElements, swapInd, current);
			if (updateFunc)
			{
				updateFunc(current->element, swapInd);
				updateFunc(toSwap->element, iIndex);
			}
			iIndex = swapInd;
		}
}