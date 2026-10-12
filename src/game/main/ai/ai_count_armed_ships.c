#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

int UnitType_GetAttackKind(int unitType);

/* Counts the current player's ships that have a weapon (slots 1-50). */
int Ai_CountArmedShips(void) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type != UNIT_NONE &&
        (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA)) {
      if (UnitType_GetAttackKind(unit->type) != ATTACK_KIND_NONE) {
        count++;
      }
    }
  }
  return count;
}
