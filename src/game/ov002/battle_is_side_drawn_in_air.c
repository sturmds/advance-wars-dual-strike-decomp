#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

BOOL Battle_IsSideAir(int side);

/* TRUE for an air side, except a B Copter that fights a ground unit and hits
 * back. The name is a guess from that rule. Scene offsets 3 and 10 (plus 12
 * per side) are the unit type and BattleSide.counters. */
BOOL Battle_IsSideDrawnInAir(int side) {
  int offset;

  if (Battle_IsSideAir(side) == 1) {
    offset = side * sizeof(BattleSide);
    if (((u8*)Ov002_gBattleScene + offset)[3] != UNIT_B_COPTER) {
      return TRUE;
    }
    if (Battle_IsSideAir((side + 1) & 1) == 1) {
      return TRUE;
    }
    if (((u8*)Ov002_gBattleScene + offset)[10] == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
