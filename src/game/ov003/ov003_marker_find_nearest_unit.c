#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"
#include "game/math.h"

int func_ov003_023622b8(void);

/* Returns the live unit nearest to the marker's tile centre, or NULL if none
 * is within the marker's reach (Ov003TileInfo.reach + 2 tiles). For
 * unk_08_4 0 to 3 only units on one side count: 0 and 1 compare y, 2 and 3
 * compare x. Needed to match: the declaration order and case 1 before case 0. */
Ov003Unit* Ov003Marker_FindNearestUnit(Ov003Marker* marker) {
  Ov003Unit* unit;
  Ov003Unit* nearest;
  u32 nearestDistance;
  int i;
  u32 distance;
  nearest = NULL;
  i = 0;
  unit = data_ov003_02369538;
  nearestDistance = ((data_ov003_02364d88[marker->kind].reach + 2) << 7) >> 3;

  for (; i < func_ov003_023622b8(); i++, unit++) {
    if (unit->hp == 0 || unit->unk_17_6) {
      continue;
    }
    switch (marker->unk_08_4) {
      case 1:
        if (unit->y > (marker->y << 7) + 64) {
          continue;
        }
        break;
      case 0:
        if (unit->y < (marker->y << 7) + 64) {
          continue;
        }
        break;
      case 2:
        if (unit->x > (marker->x << 7) + 64) {
          continue;
        }
        break;
      case 3:
        if (unit->x < (marker->x << 7) + 64) {
          continue;
        }
        break;
    }
    distance =
        Math_Distance(unit->x >> 3, unit->y >> 3, ((marker->x << 7) + 64) >> 3,
                      ((marker->y << 7) + 64) >> 3);
    if (distance <= nearestDistance) {
      nearest = unit;
      nearestDistance = distance;
    }
  }
  return nearest;
}
