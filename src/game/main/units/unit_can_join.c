#include "types.h"
#include "game/Unk_02080440.h"

s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when unit can join target: same type, same owner, neither one
 * carrying cargo, and target below 10 shown hp. */
BOOL Unit_CanJoin(MapUnit* unit, MapUnit* target) {
  int shownHp;

  if (unit->type != target->type) {
    return FALSE;
  }
  /* bits 6-7 of the slot: the owner's block of MAP_UNITS_PER_PLAYER */
  if (((unit - STATE->units) & 0xc0) != ((target - STATE->units) & 0xc0)) {
    return FALSE;
  }
  if (unit->cargo[0] != 0 || target->cargo[0] != 0) {
    return FALSE;
  }
  if (target->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(target->hp - 1, 10) + 1;
  }
  if (shownHp == 10) {
    return FALSE;
  }
  return TRUE;
}
