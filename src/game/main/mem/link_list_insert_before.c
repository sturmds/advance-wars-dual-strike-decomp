#include "types.h"
#include "game/link_list.h"

/* Puts an object into the list in front of another one. A NULL position
 * appends. */
void LinkList_InsertBefore(LinkList* list, void* position, void* object) {
  ListLink* link;
  ListLink* positionLink;
  ListLink* prevLink;

  if (position == NULL) {
    LinkList_Append(list, object);
    return;
  }
  if (position == list->head) {
    LinkList_Prepend(list, object);
    return;
  }
  link = (ListLink*)((u8*)object + list->linkOffset);
  positionLink = (ListLink*)((u8*)position + list->linkOffset);
  prevLink = (ListLink*)((u8*)positionLink->prev + list->linkOffset);
  link->prev = positionLink->prev;
  link->next = position;
  prevLink->next = object;
  ((ListLink*)((u8*)position + list->linkOffset))->prev = object;
  list->count++;
}
