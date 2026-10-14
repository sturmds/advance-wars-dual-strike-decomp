#include "types.h"
#include "game/rect.h"

#pragma explicit_zero_data on
/* 0x4 bytes no code names, before gRectHitBits */
u32 data_02158f24 = {
#include "data/main/data_02158f24.inc.c"
};
static u32 gRectHitBits[] = {
#include "data/main/gRectHitBits.inc.c"
};
#pragma explicit_zero_data reset

/* Returns the gRectHitBits entry (1, 2, 4, 8) of the first of count rectangles
 * that holds the point, or 0 when none does. */
u32 Rects_FindHitBit(int x, int y, const Rect* rects, int count) {
  int i;

  for (i = 0; i < count; i++) {
    if (Rect_ContainsPoint(x, y, &rects[i])) {
      return gRectHitBits[i];
    }
  }
  return 0;
}
