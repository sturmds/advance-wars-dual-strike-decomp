#include "types.h"
#include "game/link_list.h"

/* Returns the object before the given one, or the last object when the
 * given one is NULL. */
void* LinkList_GetPrev(LinkList* list, void* object) {
  if (object == NULL) {
    return list->tail;
  }
  return ((ListLink*)((u8*)object + list->linkOffset))->prev;
}
