#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiBlackBoat_GetRepairPriority(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when unit is a better Black Boat repair target than best (NULL: no
 * best yet). It must have a higher repair priority (AiBlackBoat_GetRepairPriority); on a
 * tie, a higher repair cost per HP. */
BOOL AiBlackBoat_IsBetterRepairTarget(MapUnit* best, MapUnit* unit) {
  int bestPriority;
  int priority;

  if (best == NULL) {
    return TRUE;
  }
  bestPriority = AiBlackBoat_GetRepairPriority(best);
  priority = AiBlackBoat_GetRepairPriority(unit);
  if (bestPriority < priority) {
    return TRUE;
  }
  if (bestPriority == priority) {
    if (Unit_GetValuePerHp(STATE->currentPlayer, best->type) <
        Unit_GetValuePerHp(STATE->currentPlayer, unit->type)) {
      return TRUE;
    }
  }
  return FALSE;
}
