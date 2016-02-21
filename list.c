
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "List.h"


List* ListCreate(int elementSize)
{
    List* list = (List*) malloc(sizeof(List));
    list->head = NULL;
    list->pointed = NULL;
    list->size = 0;
    list->elementSize = elementSize;
    return list;
}

short ListInsert(List* list, void* data)
{
    if (list == NULL) return 1;

    El* temp;
    if((temp = (El*) malloc(sizeof(El))) == NULL) return 2;
    if((temp->data = (void*) malloc(list->elementSize)) == NULL) return 2;
    memcpy(temp->data, data, list->elementSize);

    temp->next = list->head;
    list->head = temp;

    list->pointed = list->head;
    list->size++;
    return 0;
}

void* ListGet(List* list, int index)
{
    if (list == NULL) return (void*) -1;
    if (list->size <= index) return (void*) -2;

    El* ret = list->head;

    int i;
    for (i = 0; i < index; i++)
    {
        ret = ret->next;
    }

    return ret->data;
}

short ListGetEnumerator(List* list)
{
    if (list == NULL) return 0;
    if (list->size == 0 || list->pointed == NULL)
    {
        list->pointed = list->head;
        return 0;
    }

    return 1;
}

void* ListPointed(List* list)
{
    El* ret = list->pointed;
    list->pointed = list->pointed->next;
    return ret->data;
}

short ListRemove(List* list, void* data)
{
    if (list == NULL) return 1;
    if (list->size == 0) return 2;

    // Removing head
    El* prev = list->head;
    if (list->head->data == data)
    {
        list->head = list->head->next;
        list->size--;
        free(prev->data);
        free(prev);
        return 0;
    }

    // Removing in midle
    while (prev->next != NULL)
    {
        if (prev->next->data == data)
        {
            El* temp = prev->next;
            prev->next = prev->next->next;
            list->size--;
            free(temp->data);
            free(temp);
            return 0;
        }
        else
        {
            prev = prev->next;
        }
    }

    return 3;
}

void ListDestroy(List* list)
{
    El* temp;
    list->size = 0;

    while (list->head != NULL){
        temp = list->head;
        list->head = list->head->next;
        free(temp->data);
        free(temp);
    }
}

/*
struct el* Remove(struct PriorQueue* pq){
    if (pq == NULL || pq->head == NULL) return NULL;

    struct el *temp = pq->head;

    pq->head = pq->head->next;
    pq->size--;

    return temp;
};
*/
