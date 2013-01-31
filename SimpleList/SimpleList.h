//
//  SimpleList.h
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#ifndef _SimpleList_h
#define _SimpleList_h

#define SCALE_FACTOR 2

typedef struct {
    void **listHead;
    long currentNum;
    long maxNum;
} SimpleList;

int InitList(SimpleList*, long);

void* GetElementAt(SimpleList*, long);

void SetElementAt(SimpleList*, long, void*);

int AddElement(SimpleList*, void*);

void* PopAt(SimpleList*, long);

int DisposeListWithElements(SimpleList*);

void TraverseAndDisposeList(SimpleList*, void (*) (void*));
#endif
