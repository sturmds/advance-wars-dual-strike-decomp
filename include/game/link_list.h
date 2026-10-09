#ifndef GAME_LINK_LIST_H
#define GAME_LINK_LIST_H

#include "types.h"

/* Doubly linked list of objects that carry their own links. Each object
 * holds a ListLink at linkOffset bytes from its start. The heap code keeps
 * its heaps in these lists; other systems use them too. */
typedef struct ListLink {
    void *prev;
    void *next;
} ListLink;

typedef struct LinkList {
    void *head;
    void *tail;
    u16 count;
    u16 linkOffset;
} LinkList;

void LinkList_Init(LinkList *list, u16 linkOffset);
void *LinkList_GetNext(LinkList *list, void *object);

#endif
