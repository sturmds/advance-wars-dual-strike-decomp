#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int DefenseIncreaseTagTransport(int player, int slot, int unitType);
int GetUnitCombatClass(int unitType);
/* Com Tower defense: the CO's towerDefense times the towers owned. */
int Player_GetComTowerDefense(int playerIndex, int slot, int powerState);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Defense bonus in percent of one CO of a player (slot 0 front, 1 back)
 * in a power state, for a unit type: the player's defenseBonus (10 while a
 * power is on), the front CO's skills, Com Towers and the CO's stat rows. 0 when
 * coEffectsOn is clear, and 0 for Oozium (UNIT_CLASS_ALL). */
int Player_GetCoDefense(int playerIndex, int slot, int powerState,
                        int unitType) {
  const CoStatRow* classRow;
  const CoStatRow* combatRow;
  int co =
      (&data_027c027c->unk_48[data_027c0284]->players[playerIndex].front)[slot]
          .id &
      PLAYER_CO_ID_MASK;
  int bonus = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return bonus;
  }
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_ALL) {
    return bonus;
  }
  bonus +=
      data_027c027c->unk_48[data_027c0284]->players[playerIndex].defenseBonus;
  /* The original passes 0, not slot: skills always come from the front
     * CO's list. Player_GetCoFirepower passes slot. */
  bonus += DefenseIncreaseTagTransport(playerIndex, 0, unitType);
  bonus += Player_GetComTowerDefense(playerIndex, slot, powerState);
  classRow = gCoTable[co]
                 .blocks[powerState]
                 .statRows[Ov000_gUnitStats[unitType].unitClass];
  combatRow =
      gCoTable[co].blocks[powerState].statRows[GetUnitCombatClass(unitType)];
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_FOOT) {
    bonus += classRow->defense;
  } else {
    bonus += classRow->defense + combatRow->defense;
  }
  return bonus;
}
