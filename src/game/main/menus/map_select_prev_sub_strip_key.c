#include "types.h"

void MapSelect_PrevSubStrip(const void* src, void* dst, u32 size);

void MapSelect_PrevSubStripKey(const void* src, void* dst, u32 size) {
  MapSelect_PrevSubStrip(src, dst, size);
}
