#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include <stdlib.h>
#include "types.h"

// Actual structure of List is private. Use the provided api.
typedef struct List List;

List* ListAlloc(uint64_t itemSize);
void ListFree(List* this);
bool ListPush(List* this, const void* item);
bool ListPop(List* this, void* item);
bool ListPeek(const List* this, void* item);
bool ListGet(const List* this, uint64_t index, void* item);
bool ListSet(List* this, uint64_t index, const  void* item);
bool ListIsEmpty(const List* this);
uint64_t ListLength(const List* this);


#endif //GRAPH_LIST_H
