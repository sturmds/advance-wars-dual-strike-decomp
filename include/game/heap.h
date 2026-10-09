#ifndef GAME_HEAP_H
#define GAME_HEAP_H

#include "types.h"
#include "game/link_list.h"

/* Memory heaps, laid out like NitroSystem's expanded heap (FreeBlockHeap).
 * Every heap starts with a HeapHead. Heaps form a tree: each heap sits in
 * the child list of the heap whose range holds it, or in a root list. */
typedef struct HeapHead {
    u32 signature;
    ListLink link;              /* in the parent's children list */
    LinkList children;          /* heaps created inside this one */
    void *start;                /* first byte after the header */
    void *end;
    u32 options;                /* bits 0-7: option flags given at creation */
} HeapHead;

#define HEAP_LINK_OFFSET 4      /* offset of HeapHead.link, for LinkList_Init */

/* Header in front of every free or used block, 0x10 bytes. The caller's
 * pointer is the address just after it. */
typedef struct HeapBlock {
    u16 signature;
    u16 attribute;
    u32 size;                   /* bytes after the header */
    struct HeapBlock *prev;
    struct HeapBlock *next;
} HeapBlock;

/* HeapBlock.signature values */
#define HEAP_BLOCK_FREE 0x4652  /* "FR" */

/* Bits 8-14 of HeapBlock.attribute: bytes of alignment padding in front of the header. */
#define HEAP_BLOCK_PADDING(block) (((block)->attribute >> 8) & 0x7F)

/* First byte after a block's memory. Size first: needed to match. */
#define HEAP_BLOCK_END(block) ((void *)((block)->size + (u32)((block) + 1)))

typedef struct HeapBlockList {
    HeapBlock *head;
    HeapBlock *tail;
} HeapBlockList;

/* A range of free memory, [start, end). */
typedef struct HeapRegion {
    void *start;
    void *end;
} HeapRegion;

typedef struct ExpHeapData {
    HeapBlockList freeBlocks;
    HeapBlockList usedBlocks;
    u16 groupId;
    u16 allocMode;              /* bit 0 set: best fit; clear: first fit */
} ExpHeapData;

typedef struct ExpHeap {
    HeapHead head;
    ExpHeapData data;
} ExpHeap;

extern ExpHeap *gGameHeap;             /* Mem_Alloc and Mem_Free use it */

void *Heap_Alloc(ExpHeap *heap, u32 size, int align); /* negative align takes the top end */
void Heap_Free(ExpHeap *heap, void *memory);

#endif
