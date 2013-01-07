//
//  SimpleList.h
//  
//
//  Created by Vladyslav Gorbatiuk on 04.11.12.
//
//

#ifndef _SimpleList_h
#define _SimpleList_h

typedef struct {
    void **listHead;
    long currentNum;
    long maxNum;
} SimpleList;

void InitList(SimpleList*, long);

void* GetElementAt(SimpleList*, long);

void SetElementAt(SimpleList*, long, void*);

void AddElement(SimpleList*, void*);

void* PopAt(SimpleList*, long);

void DisposeList(SimpleList*);
#endif
