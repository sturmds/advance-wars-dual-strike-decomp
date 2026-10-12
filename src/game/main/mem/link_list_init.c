#include "types.h"
#include "game/link_list.h"

/* Empties a list. linkOffset is where each object keeps its ListLink. */
void LinkList_Init(LinkList* list, u16 linkOffset) {
  list->head = NULL;
  list->tail = NULL;
  list->count = 0;
  list->linkOffset = linkOffset;
}
