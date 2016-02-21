
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef struct _El {
    void* data;
    struct _El* next;
} El;

typedef struct _List {
    int size;
    int elementSize;
    El* pointed;
    El* head;
} List;


List* ListCreate(int elementSize);
short ListInsert(List* list, void* data);
void* ListGet(List* list, int index);

short ListGetEnumerator(List* list);
void* ListPointed(List* list);

short ListRemove(List* list, void* data);

void ListDestroy(List* list);

#endif // LIST_H_INCLUDED
