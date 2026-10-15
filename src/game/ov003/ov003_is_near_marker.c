#include "types.h"
#include "game/ov003_marker.h"
#include "game/math.h"

/* x and y are in pixels. Kind 6 reaches 8 pixels, kind 7 reaches 24. */
int Ov003_IsNearMarker(Ov003Marker* marker, int x, int y) {
  u32 radius;

  switch (marker->kind) {
    case 6:
      radius = 8;
      break;
    case 7:
      radius = 24;
      break;
    default:
      return 0;
  }
  return Math_Distance(x, y, (marker->x * 128 + 64) >> 3,
                       (marker->y * 128 + 64) >> 3) <= radius;
}
