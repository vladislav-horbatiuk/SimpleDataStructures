//
//  SimpleLinkedList.h
//  
//
//  Created by Vladyslav Gorbatiuk on 10.11.12.
//
//

#ifndef _SimpleLinkedList_h
#define _SimpleLinkedList_h

struct linkedListElement
{
    void *element;
    struct linkedListElement *next;
};
typedef struct linkedListElement tLinkedListElement;
typedef struct
{
    tLinkedListElement *head;
    tLinkedListElement *tail;
    unsigned long count;
} SimpleLinkedList;

/*
 USAGE: must be called before usage of a linked list.
 INPUTS:
 -oList - pointer to the SimpleLinkedList structure to initialize.
 */
void InitLinkedList(SimpleLinkedList *oList);

/*
 USAGE: adds another element to the linked list (at the end).
 INPUTS:
 -oList - pointer to the SimpleLinkedList structure to add new element to.
 -iElement - pointer to the added element.
 RETURNS: 0 if element was added succesfully, -1 otherwise.
 */
int AddElementToLinkedList(SimpleLinkedList *oList, void *iElement);

/*
 USAGE: returns pointer to the element at index iIndex in linked list iList.
 INPUTS:
 -iList - pointer to the SimpleLinkedList structure in which the element will be searched.
 -iIndex - an index of the desired element.
 RETURNS: pointer to the found element; NULL if iIndex is incorrect.
 */
void* GetElementAtIndexInLinkedList(SimpleLinkedList *iList, unsigned long iIndex);

/*
 USAGE: loops through all linked list's elements and calls iFunc for each element passing element
 itself as an argument.
 INPUTS:
 -iList - pointer to the SimpleLinkedList structure which elements should be traversed.
 -iFunc - pointer to the function, that will be called on each element.
 */
void TraverseLinkedListElementsUsingFunc(SimpleLinkedList *iList, void (*iFunc)(void*));

/*
 USAGE: frees all elements of the linked list.
 INPUTS:
 -oList - pointer to the SimpleLinkedList structure to be disposed.
 */
void DisposeLinkedListAndElements(SimpleLinkedList *oList);

/*
*/
void TraverseAndDisposeLinkedList(SimpleLinkedList *oList, void (*iFunc)(void*));

#endif
