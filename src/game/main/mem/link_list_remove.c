#include "types.h"
#include "game/link_list.h"

/* Takes an object out of the list. */
void LinkList_Remove(LinkList* list, void* object) {
  ListLink* link = (ListLink*)((u8*)object + list->linkOffset);
  void* prev = link->prev;

  if (prev == NULL) {
    list->head = link->next;
  } else {
    ((ListLink*)((u8*)prev + list->linkOffset))->next = link->next;
  }
  if (link->next == NULL) {
    list->tail = link->prev;
  } else {
    ((ListLink*)((u8*)link->next + list->linkOffset))->prev = link->prev;
  }
  link->prev = NULL;
  link->next = NULL;
  list->count--;
}
