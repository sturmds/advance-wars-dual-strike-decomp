#include "types.h"
#include "game/link_list.h"

/* Returns the object after the given one, or the first object when the
 * given one is NULL. */
void* LinkList_GetNext(LinkList* list, void* object) {
  if (object == NULL) {
    return list->head;
  }
  return ((ListLink*)((u8*)object + list->linkOffset))->next;
}
