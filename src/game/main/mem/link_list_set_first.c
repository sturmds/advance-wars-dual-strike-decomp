#include "types.h"
#include "game/link_list.h"

/* Puts the first object into an empty list. */
void LinkList_SetFirst(LinkList* list, void* object) {
  ListLink* link = (ListLink*)((u8*)object + list->linkOffset);

  link->next = NULL;
  link->prev = NULL;
  list->head = object;
  list->tail = object;
  list->count++;
}
