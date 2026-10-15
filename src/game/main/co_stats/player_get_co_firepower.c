#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int FirepowerIncrease(int player, int slot, int unitType);
int GetUnitCombatClass(int unitType);
/* Com Tower firepower: (CO value per tower, or 10 with coEffectsOn clear,
 * plus the skill bonus) times the towers owned. */
int Player_GetComTowerFirepower(int playerIndex, int slot, int powerState);

/* Firepower bonus in percent of one CO of a player (slot 0 front, 1 back)
 * in a power state, for a unit type: skills, Com Towers and the CO's stat
 * rows. Foot units get their class row only; other units also get the row
 * for their combat class (direct, indirect, transport or explodes). */
int Player_GetCoFirepower(int playerIndex, int slot, int powerState,
                          int unitType) {
  /* needed to match: declared before co and bonus */
  const CoStatRow* classRow;
  const CoStatRow* combatRow;
  int co =
      (&data_027c027c->unk_48[data_027c0284]->players[playerIndex].front)[slot]
          .id &
      PLAYER_CO_ID_MASK;
  int bonus = 0;

  bonus += FirepowerIncrease(playerIndex, slot, unitType);
  bonus += Player_GetComTowerFirepower(playerIndex, slot, powerState);
  classRow = gCoTable[co]
                 .blocks[powerState]
                 .statRows[Ov000_gUnitStats[unitType].unitClass];
  combatRow =
      gCoTable[co].blocks[powerState].statRows[GetUnitCombatClass(unitType)];
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_FOOT) {
    bonus += classRow->firepower;
  } else {
    bonus += classRow->firepower + combatRow->firepower;
  }
  return bonus;
}
