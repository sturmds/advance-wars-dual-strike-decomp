#include "types.h"
#include "game/ov003_marker.h"

/* Finds the busy marker covering a tile: the tile itself for kind 6, the 3 by 3
 * tiles around it for kind 7. */
Ov003Marker* Ov003_FindMarkerAtTile(int x, int y) {
  Ov003Marker* marker = data_ov003_02369bb8;
  int i;

  for (i = 0; i < OV003_MARKER_COUNT; i++, marker++) {
    if (!marker->busy) {
      continue;
    }
    switch (marker->kind) {
      case 6:
        if (x == marker->x && y == marker->y) {
          return marker;
        }
        break;
      case 7:
        if (x >= marker->x - 1 && y >= marker->y - 1 && x <= marker->x + 1 &&
            y <= marker->y + 1) {
          return marker;
        }
        break;
    }
  }
  return NULL;
}
