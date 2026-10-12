#include "types.h"
#include "game/link_list.h"

/* Adds an object at the end of the list. */
void LinkList_Append(LinkList* list, void* object) {
  ListLink* link;

  if (list->head == NULL) {
    LinkList_SetFirst(list, object);
    return;
  }
  link = (ListLink*)((u8*)object + list->linkOffset);
  link->prev = list->tail;
  link->next = NULL;
  ((ListLink*)((u8*)list->tail + list->linkOffset))->next = object;
  list->tail = object;
  list->count++;
}
