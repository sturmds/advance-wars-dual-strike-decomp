#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the current player's units whose AI class (AI_CLASS_*) is aiClass.
 * Like Ai_CountUnitsOfType, it skips the first unit slot and covers 50. */
int Ai_CountUnitsOfAiClass(int aiClass) {
  int count = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE &&
        aiClass == Ov000_gUnitStats[unit->type].aiClass) {
      count++;
    }
  }
  return count;
}
