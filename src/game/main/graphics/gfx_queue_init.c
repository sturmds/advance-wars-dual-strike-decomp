#include "types.h"
#include "game/gfx_queue.h"

/* Empties the whole queue (called once at startup). */
void GfxQueue_Init(void) {
  int i;

  data_027c051c = 0;
  data_027c0518 = 0;
  for (i = 0; i < GFX_QUEUE_LENGTH; i++) {
    data_027c0520[i].src = NULL;
    data_027c0520[i].dst = NULL;
    data_027c0520[i].size = 0;
    data_027c0520[i].kind = 0;
  }
}
