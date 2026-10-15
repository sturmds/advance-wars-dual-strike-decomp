#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_count_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* Counts the current player's units of the given type. The scan skips the
 * player's first unit slot and covers the next 50. */
int Ai_CountUnitsOfType(int type) {
  int count = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type == type) {
      count++;
    }
  }
  return count;
}

/* Counts the current player's units whose AI class (AI_CLASS_*) is aiClass.
 * Like Ai_CountUnitsOfType, it skips the first unit slot and covers 50. */
int Ai_CountUnitsOfAiClass(int aiClass) {
  int count = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE &&
        aiClass == Ov000_gUnitStats[unit->type].aiClass) {
      count++;
    }
  }
  return count;
}
