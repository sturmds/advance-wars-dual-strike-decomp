#include "types.h"
#include "game/gfx_queue.h"

void Mem_Free(void* ptr);

/* Frees the heap copies held by queued kind 3 and 4 entries and empties
 * those entries. */
void GfxQueue_FreeOwnedCopies(void) {
  int i;

  for (i = 0; i < GFX_QUEUE_LENGTH; i++) {
    if (GFX_QUEUE_OWNS_SRC(data_027c0520[i].kind)) {
      Mem_Free((void*)data_027c0520[i].src);
      data_027c0520[i].src = NULL;
      data_027c0520[i].dst = NULL;
      data_027c0520[i].size = 0;
      data_027c0520[i].kind = 0;
    }
  }
}
