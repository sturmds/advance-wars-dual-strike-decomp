#include "types.h"
#include "game/ov003_unit.h"

/* Whether the unit may move over a tile with this terrain code (tile code & 31),
 * as the caller checks the tiles on a line. Terrain 0, 7, 9, 15, 19, 21, 25 and
 * 26 stop every unit. Terrain 2 lets through only units of tile 1 or 2; 3 also
 * those with unk_1c_1 set; 16 needs unk_1c_1. The rest is open. "+ 255" instead
 * of "- 1" is needed to match. */
int Ov003Unit_CanEnterTerrain(Ov003Unit* unit, int terrain) {
  switch (terrain) {
    case 0:
    case 7:
    case 9:
    case 15:
    case 19:
    case 21:
    case 25:
    case 26:
      return 0;
    case 3:
      if ((u8)(unit->kind + 255) <= 1) {
        return 1;
      }
    case 16:
      return unit->unk_1c_1 != 0;
    case 2:
      return (u8)(unit->kind + 255) <= 1;
    default:
      return 1;
  }
}
