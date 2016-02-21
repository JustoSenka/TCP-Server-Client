
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef int (*listIterator)(void *);

typedef struct _listNode {
  void* data;
  struct _listNode *next;
} ListNode;

typedef struct _list {
  int logicalLength;
  int elementSize;
  ListNode *head;
  ListNode *tail;
} List;

void list_new(List *list, int elementSize);
void list_destroy(List *list);

void list_prepend(List *list, void *element);
void list_append(List *list, void *element);
int list_size(List *list);

void* list_head(List *list);
void  list_remove(void* element);

void list_for_each(List *list, listIterator iterator);

#endif // LIST_H_INCLUDED
