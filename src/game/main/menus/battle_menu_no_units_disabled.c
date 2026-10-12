#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Battle menu entry test: 2 greys the entry out (its handler returns at once
 * when bit 1 of the flags is set), 0 shows it. The entry is greyed out when
 * the first 50 unit slots after the current player's block start are all
 * empty. */
int BattleMenu_NoUnitsDisabled(void) {
  s16 i;
  int slot;
  MapUnit* units;

  i = 1;
  slot = STATE->currentUnitBase + 1;
  units = STATE->units;
  for (; i < 51; i++, slot++) {
    /* byte access: needed to match; units[slot].type makes mwcc walk a pointer */
    if (((u8*)units)[slot * sizeof(MapUnit)] != UNIT_NONE) {
      break;
    }
  }
  return i == 51 ? 2 : 0;
}
