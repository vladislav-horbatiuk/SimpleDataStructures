//
//  SimpleHeap.c
//  
//
//  Created by Vladyslav Gorbatiuk on 02.01.13.
//
//
#include "SimpleHeap.h"
#include <stdlib.h>

int InitHeap(SimpleHeap *oHeap, long iCount)
{
	InitList(&oHeap->elements, iCount);
}

int AddElementToHeap(SimpleHeap *oHeap, double iKey, void *iElement)
{
	SimpleList *elements = &oHeap->elements;
	tHeapElement *newElement = malloc(1 * sizeof(tHeapElement));
	newElement->key = iKey; newElement->element = iElement;
	AddElement(elements, newElement);
	long currentIndex = elements->currentNum - 1;
	while (currentIndex > 0)
	{
		long parentIndex = PARENT(currentIndex);
		tHeapElement *parent = HEAP_ELEMENT(GetElementAt(elements, parentIndex));
		tHeapElement *current = HEAP_ELEMENT(GetElementAt(elements, currentIndex));
		if (parent->key < current->key)
			break;
		SetElementAt(elements, currentIndex, parent);
		SetElementAt(elements, parentIndex, current);
		currentIndex = parentIndex;
	}
	return 0;
}

void* ExtractMinElement(SimpleHeap *oHeap)
{
	long currentIndex = 0;
	SimpleList *elements = &oHeap->elements;
	long elementsCount = elements->currentNum;
	if (!elementsCount)
		return NULL;
	void *returnValue = GetElementAt(elements, currentIndex);
	tHeapElement *last = PopAt(elements, elementsCount - 1);
	elementsCount--;
	if (elementsCount)
	{
		SetElementAt(elements, currentIndex, last);

		int stop;
		long leftChildInd, rightChildInd, swapInd;
		double rightChildKey, leftChildKey, currentKey = last->key;

		while ((leftChildInd = LEFT_CHILD(currentIndex)) < elementsCount)
		{
			stop = 1;
			leftChildKey = HEAP_ELEMENT(GetElementAt(elements, leftChildInd))->key;
			rightChildKey = DBL_MAX;
			if ((rightChildInd = RIGHT_CHILD(currentIndex)) < elementsCount)
				stop = (rightChildKey = HEAP_ELEMENT(GetElementAt(elements, rightChildInd))->key) > currentKey;
			stop = stop && (leftChildKey > currentKey);
			if (stop)
				break;
			swapInd = (leftChildKey < rightChildKey) ? leftChildInd : rightChildInd;
			tHeapElement *toSwap = HEAP_ELEMENT(GetElementAt(elements, swapInd));
			SetElementAt(elements, currentIndex, toSwap);
			SetElementAt(elements, swapInd, last);
			currentIndex = swapInd;
		}
	}
	return returnValue;
}

void* LookupMinElement(SimpleHeap *iHeap)
{
	return GetElementAt(&iHeap->elements, 0);
}

int DisposeHeap(SimpleHeap *oHeap)
{
	long i = 0;
	tHeapElement *current = HEAP_ELEMENT(GetElementAt(&oHeap->elements, i));
	for (; current!=NULL; current=GetElementAt(&oHeap->elements, ++i))
		free(current->element);
	DisposeList(&oHeap->elements);
	return 0;
}