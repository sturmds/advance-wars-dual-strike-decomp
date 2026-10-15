#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/unit_list_task.h"

/* Clears the 64-byte table at gUnitListRows, then counts the current
 * player's units. The scan skips the player's first unit slot and covers
 * the next 50. */
int UnitList_ClearAndCount(void) {
  int i;
  int j;
  int count;
  MapUnit* unit;

  for (i = 0; i < UNIT_LIST_SIZE; i++) {
    gUnitListRows[i] = 0;
  }
  count = 0;
  for (j = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       j < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; j++) {
    unit = &MAP_STATE->units[j];
    if (unit->type != UNIT_NONE) {
      count++;
    }
  }
  return count;
}
