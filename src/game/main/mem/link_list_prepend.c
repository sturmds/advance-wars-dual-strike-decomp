#include "types.h"
#include "game/link_list.h"

/* Adds an object at the start of the list. */
void LinkList_Prepend(LinkList* list, void* object) {
  ListLink* link;

  if (list->head == NULL) {
    LinkList_SetFirst(list, object);
    return;
  }
  link = (ListLink*)((u8*)object + list->linkOffset);
  link->prev = NULL;
  link->next = list->head;
  ((ListLink*)((u8*)list->head + list->linkOffset))->prev = object;
  list->head = object;
  list->count++;
}
