#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

int GetUnitCombatClass(int unitType);
int MovementBoost(int player, int slot, int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Change to a unit type's movement from one CO of a player (coSlot 0:
 * front, 1: back) in the given power state: the CO's stat rows plus
 * skills. Foot units take only their class row; other units add the
 * class row and the combat row. No change when coEffectsOn is clear, and
 * none for Oozium (UNIT_CLASS_ALL). */
int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType) {
  /* needed to match: declared before co and bonus */
  const CoStatRow* classRow;
  const CoStatRow* combatRow;
  int co =
      (&data_027c027c->unk_48[data_027c0284]->players[player].front)[coSlot]
          .id &
      PLAYER_CO_ID_MASK;
  int bonus = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return bonus;
  }
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_ALL) {
    return bonus;
  }
  classRow = gCoTable[co]
                 .blocks[powerState]
                 .statRows[Ov000_gUnitStats[unitType].unitClass];
  combatRow =
      gCoTable[co].blocks[powerState].statRows[GetUnitCombatClass(unitType)];
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_FOOT) {
    bonus += classRow->movement;
  } else {
    bonus += classRow->movement + combatRow->movement;
  }
  return bonus + MovementBoost(player, coSlot, unitType);
}
