#include "types.h"
#include "game/heap.h"

/* Frees a block from the game heap. */
void Mem_Free(void* block) { Heap_Free(gGameHeap, block); }
