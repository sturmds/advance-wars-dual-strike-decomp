#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];

/* Looks at the other live units (unk_17_6 clear) on the tile of unit. Returns
 * 2 as soon as one has a different row; otherwise 1 if unit->unk_04
 * is below that of any of them, else 0. Needed to match: the declarations are
 * split from their assignments. */
int Ov003Unit_CompareOnTile(Ov003Unit* unit) {
  Ov003Unit* other;
  int i;
  int tileX;
  int tileY;
  int result;
  tileX = unit->x >> 7;
  tileY = unit->y >> 7;
  result = 0;
  other = data_ov003_02369538;
  for (i = 0; i < 16; i++, other++) {
    if (unit != other && other->hp != 0 && !other->unk_17_6 &&
        (other->x >> 7) == tileX && (other->y >> 7) == tileY) {
      if (other->row != unit->row) {
        return 2;
      }
      if (unit->unk_04 < other->unk_04) {
        result = 1;
      }
    }
  }
  return result;
}
