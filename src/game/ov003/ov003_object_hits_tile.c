#include "types.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"

extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 owner, bits 0-4 terrain */

Ov003Marker* Ov003_FindMarkerAtTile(int x, int y);
int Ov003_IsNearMarker(Ov003Marker* entry, int x, int y);

/* Returns 1 when object hits the point (x, y), in 8-pixel units, judged by the
 * terrain of the tile there: terrain 15 and 16 always, terrain 3 unless the
 * object is kind 2, and terrain 9, 21 and 26 when Ov003_IsNearMarker accepts
 * the Ov003Marker entry on the tile (skipped for the entry named by
 * object->owner when bit 7 is set). */
int Ov003Object_HitsTerrainAt(int x, int y, Ov003Object* object) {
  s16 tileX = x >> 4;
  s16 tileY = y >> 4;
  u8 terrain;

  if (tileX < 0 || tileY < 0 || tileX >= data_ov003_02364a88->width ||
      tileY >= data_ov003_02364a88->height) {
    return 0;
  }
  terrain =
      data_ov003_02365d10[data_ov003_02364a88->cells
                              [tileY * data_ov003_02364a88->width + tileX]] &
      31;
  switch (terrain) {
    case 3:
      if (object->kind == 2) {
        break;
      }
      /* fall through */
    case 15:
    case 16:
      return 1;
    case 9:
    case 21:
    case 26:
      if (object->owner & 0x80) {
        Ov003Marker* entry = &data_ov003_02369bb8[object->owner & ~0x80];

        if (entry->x == tileX && entry->y == tileY) {
          return 0;
        }
      }
      return Ov003_IsNearMarker(Ov003_FindMarkerAtTile(tileX, tileY), x, y);
  }
  return 0;
}
