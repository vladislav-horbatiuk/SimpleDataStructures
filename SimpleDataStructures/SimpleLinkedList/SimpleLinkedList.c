//
//  SimpleLinkedList.c
//  
//
//  Created by Vladyslav Gorbatiuk on 10.11.12.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "SimpleDataStructures/SimpleLinkedList/SimpleLinkedList.h"


static inline void _setEverythingTo0(SimpleLinkedList *oList)
{
    oList->head = oList->tail = NULL;
    oList->count = 0;
}

void InitLinkedList(SimpleLinkedList *oList)
{
    _setEverythingTo0(oList);
}

int AddElementToLinkedList(SimpleLinkedList *oList, void *iElement)
{
    tLinkedListElement *newElement = malloc(sizeof(tLinkedListElement));
    if (!newElement) return -1;
    newElement->element = iElement;
    newElement->next = NULL;
    if (oList->tail) oList->tail->next = newElement;
    oList->tail = newElement;
    if (!oList->head) {
        oList->head = newElement;
    }
    oList->count++;
    return 0;
}

void* GetElementAtIndexInLinkedList(SimpleLinkedList *iList, unsigned long iIndex)
{
    int i;
    tLinkedListElement *current = iList->head;
    for (i = 0; i < iIndex; ++i) {
        if (!current) return NULL;
        current = current->next;
    }
    if (current) return current->element;
    else return NULL;
}

void TraverseLinkedListElementsUsingFunc(SimpleLinkedList *iList, void (*iFunc)(void*))
{
    tLinkedListElement *current = iList->head;
    while (current) {
        iFunc(current->element);
        current = current->next;
    }
}

void DisposeLinkedListAndElements(SimpleLinkedList *oList)
{
    tLinkedListElement *current = oList->head;
    tLinkedListElement *tmp;
    while (current) {
        free(current->element);
        current = (tmp=current)->next;
        free(tmp);
    }
    _setEverythingTo0(oList);
}

void TraverseAndDisposeLinkedList(SimpleLinkedList *oList, void (*iFunc)(void*))
{
    tLinkedListElement *current = oList->head;
    tLinkedListElement *tmp;
    while (current) {
        iFunc(current->element);
        current = (tmp=current)->next;
        free(tmp);
    }
    _setEverythingTo0(oList);
}