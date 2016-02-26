
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef short (*Equals)(void* a, void* b);

typedef struct _El {
    void* data;
    struct _El* next;
} El;

typedef struct _List {
    int size;
    int elementSize;
    Equals equals;
    El* pointed;
    El* head;
} List;


List* ListCreate(int elementSize, Equals equals);
short ListInsert(List* list, void* data);
void* ListGet(List* list, int index);

short ListGetEnumerator(List* list);
void* ListPointed(List* list);

short ListRemove(List* list, void* data);
short ListRemoveByValue(List* list, void* value);

void ListDestroy(List* list);

#endif // LIST_H_INCLUDED
