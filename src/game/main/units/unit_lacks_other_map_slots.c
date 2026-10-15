#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/screen_other_map.h"
#include "game/unit_stats.h"

#define OTHER_MAP_SLOTS 50

/* TRUE when the current player has fewer free unit slots on the other map
 * of a dual-front battle than the unit needs: one for itself and one for
 * each unit it carries, two levels deep. Only the first 50 slots after the
 * player's first slot are counted there. */
BOOL Unit_LacksOtherMapSlots(MapUnit* unit) {
  int firstSlot;
  u8 freeSlots;
  u8 needed;
  MapUnit* cargo;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int slot;
  int end;

  freeSlots = 0;
  firstSlot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer];
  needed = 1;
  if (unit->cargo[0] != 0) {
    cargo = &MAP_STATE->units[unit->cargo[0]];
    needed++;
    if (cargo->cargo[0] != 0) {
      needed++;
    }
    if (cargo->cargo[1] != 0) {
      needed++;
    }
  }
  if (unit->cargo[1] != 0) {
    cargo = &MAP_STATE->units[unit->cargo[1]];
    needed++;
    if (cargo->cargo[0] != 0) {
      needed++;
    }
    if (cargo->cargo[1] != 0) {
      needed++;
    }
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  for (slot = firstSlot + 1, end = firstSlot + 1 + OTHER_MAP_SLOTS; slot < end;
       slot++) {
    if (MAP_STATE->units[slot].type == UNIT_NONE) {
      freeSlots++;
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return needed > freeSlots;
}
