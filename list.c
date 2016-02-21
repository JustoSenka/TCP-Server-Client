
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

void list_new(List *list, int elementSize)
{
  assert(elementSize > 0);
  list->logicalLength = 0;
  list->elementSize = elementSize;
  list->head = list->tail = NULL;
}

void list_destroy(List *list)
{
  ListNode *current;
  while(list->head != NULL) {
    current = list->head;
    list->head = current->next;

    free(current->data);
    free(current);
  }
}

void list_prepend(List *list, void *element)
{
  ListNode *node = malloc(sizeof(ListNode));
  node->data = malloc(list->elementSize);
  memcpy(node->data, element, list->elementSize);

  node->next = list->head;
  list->head = node;

  // first node?
  if(!list->tail) {
    list->tail = list->head;
  }

  list->logicalLength++;
}

void list_append(List *list, void *element)
{
  ListNode *node = malloc(sizeof(ListNode));
  node->data = malloc(list->elementSize);
  node->next = NULL;

  memcpy(node->data, element, list->elementSize);

  if(list->logicalLength == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  list->logicalLength++;
}

void* list_head(List *list)
{

}

void  list_remove(void* element)
{

}

void list_for_each(List *list, listIterator iterator)
{
  assert(iterator != NULL);

  ListNode *node = list->head;
  int result = 1;
  while(node != NULL && result) {
    result = iterator(node->data);
    node = node->next;
  }
}


int list_size(List *list)
{
  return list->logicalLength;
}
