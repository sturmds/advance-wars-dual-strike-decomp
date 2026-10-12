#include "types.h"
#include "game/gfx_queue.h"

void GfxQueue_AddKind2(const void* src, void* dst, u32 size) {
  data_027c0520[data_027c051c].src = src;
  data_027c0520[data_027c051c].dst = dst;
  data_027c0520[data_027c051c].size = size;
  data_027c0520[data_027c051c].kind = 2;
  data_027c0518 += size;
  data_027c051c++;
}
