#include "types.h"

void func_02104494(void *heap, void *block);

extern void *gGameHeap;

/* Frees a block from the game heap. */
void Mem_Free(void *block)
{
    func_02104494(gGameHeap, block);
}
