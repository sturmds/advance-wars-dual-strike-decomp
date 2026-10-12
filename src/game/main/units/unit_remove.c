#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

s32 HwDiv32(s32 numerator, s32 denominator);
void Player_CountUnitLost(int player, int unitType);
void Player_DecUnitCount(int player);
void Unit_AddDamageTaken(MapUnit* unit, int shownHp);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Takes a unit slot out of play and its cargo with it. destroyed: the unit
 * counts as lost (Player_CountUnitLost and Unit_AddDamageTaken record it, with the shown
 * hp); otherwise only the owner's unit count goes down (Player_DecUnitCount). */
void Unit_Remove(int unitIndex, int destroyed) {
  MapUnit* unit = &STATE->units[unitIndex];
  int shownHp;

  if (destroyed) {
    Player_CountUnitLost((unitIndex >> 6) + 1, unit->type);
    if (unit->hp == 0) {
      shownHp = 0;
    } else {
      shownHp = HwDiv32(unit->hp - 1, 10) + 1;
    }
    Unit_AddDamageTaken(unit, shownHp);
  } else {
    Player_DecUnitCount((unitIndex >> 6) + 1);
  }
  if (unit->cargo[0] != 0) {
    Unit_Remove(unit->cargo[0], destroyed);
  }
  if (unit->cargo[1] != 0) {
    Unit_Remove(unit->cargo[1], destroyed);
  }
  unit->type = UNIT_NONE;
}
