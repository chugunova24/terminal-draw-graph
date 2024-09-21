#include "list.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

bool ListExpand(List *this);

// Private definition of List
typedef struct List {
    void* data; //Actual data
    uint64_t length; //Length of list
    uint64_t allocated; //Maximum possible length without reallocations
    uint64_t itemSize; //Size of item
} List;

/*
 * ListExpand - expand List capacity
 * This functions is called, when user tries to push an element into list, but there is no space in pointer
 * In that case, List->data reallocates with x1.5 more memory.
 * So this goes 1->2->3->4->6->9->14->21...
 * It is significantly faster than linear expansion, but may explode when there are too much elements
 */
bool ListExpand(List* this){
    uint64_t newAllocated = (uint64_t)round((double)this->allocated * 1.5);

    void* newData = realloc(this->data, newAllocated*this->itemSize);
    if (!newData) return False;

    this->data = newData;
    this->allocated = newAllocated;
    return True;
}


/*
 * ListAlloc - Simple List allocation
 * List is supposed to be multi-type. One can create List of ints, other can create List of struct sockaddr_in
 * List doesn't care.
 * List only cares about size of its element.
 * That's why List->data is void*. No one knows what can be there
 *
 */
List *ListAlloc(uint64_t itemSize) {
    List* stack = calloc(1, sizeof(List));

    if (!stack) return 0;

    stack->itemSize = itemSize;
    stack->length = 0;
    stack->allocated = 1;
    stack->data = calloc(itemSize, 1);
    if (!stack->data){
        free(stack);
        return 0;
    }
    return stack;
}

/*
 * ListFree - free the List
 * Both list object and it's data is dynamic memory located in heap.
 */

void ListFree(List *this) {
    // Firstly, free lists data
    free(this->data);
    // Then free itself
    free(this);
}


/*
 * ListPush - push an object to list
 * Works like list.append() in python
 * This can fail if there is not space left and expansion failed
 */
bool ListPush(List* this, const void* item){
    if (this->length >= this->allocated && !ListExpand(this)) return False;
    this->length++;
    ListSet(this, this->length-1, item); //Can't fail
    return True;
}

/*
 * ListPop - pop an object from list
 * Similar to list.pop(), although the result should be stored in the argument.
 * This is because we can't return *(void*).
 * This can fail if the list is empty
 */
bool ListPop(List *this, void *item) {
    if (!ListPeek(this, item)) return False;
    this->length--;
    return True;
}

/* ListPeek - like pop, but without deletion */
bool ListPeek(const List *this, void *item) {
    if (this->length == 0) return False;
    ListGet(this, this->length-1, item);
    return True;
}

/*
 * ListGet - get List[index]
 * This can fail if index is not in [0; length)
 */
bool ListGet(const List *this, uint64_t index, void *item) {
    if (index >= this->length) return False;
    const void* itemOffset = (const void*) ((uint64_t)this->data + this->itemSize*index);
    memcpy(item, itemOffset, this->itemSize);
    return True;
}

/*
 * ListSet - set List[index]
 * This can fail if index is not in [0; length)
 */

bool ListSet(List *this, uint64_t index, const void* item){
    if (index >= this->length) return False;
    void* itemOffset = (void*)((uint64_t)this->data + this->itemSize*index);
    memcpy(itemOffset, item, this->itemSize);
    return True;
}

/* Self-explanatory */
bool ListIsEmpty(const List *this) {
    return this->length == 0;
}

uint64_t ListLength(const List *this) {
    return this->length;
}
