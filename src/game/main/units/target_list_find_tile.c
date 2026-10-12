#include "types.h"
#include "game/unit_target_entry.h"

UnitTargetEntry* func_020dc524(int index);

/* Index of the target list entry on tile (tileX, tileY), or -1. */
int TargetList_FindTile(int count, int tileX, int tileY) {
  int i;

  for (i = 0; i < count; i++) {
    UnitTargetEntry* target = func_020dc524(i);
    if (tileX == target->x && tileY == target->y) {
      return i;
    }
  }
  return -1;
}
