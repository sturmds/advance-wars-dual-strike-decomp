#include "types.h"

void MapSelect_NextSubStrip(const void* src, void* dst, u32 size);

void MapSelect_NextSubStripKey(const void* src, void* dst, u32 size) {
  MapSelect_NextSubStrip(src, dst, size);
}
