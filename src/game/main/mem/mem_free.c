#include "types.h"

void Heap_Free(void *heap, void *block);

extern void *gGameHeap;

/* Frees a block from the game heap. */
void Mem_Free(void *block)
{
    Heap_Free(gGameHeap, block);
}
