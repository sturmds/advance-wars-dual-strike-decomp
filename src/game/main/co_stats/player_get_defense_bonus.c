#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int Player_GetCoDefense(int playerIndex, int slot, int powerState,
                        int unitType);
int DefenseIncreaseDirectIndirect(int player, int defenderType,
                                  int attackerType, int arg3, int arg4);

/* Defense bonus in percent of a player's unit against an attacker type:
 * the front CO's bonus in its power state plus skill and Force Rank
 * bonuses. 0 with coEffectsOn clear, and for Oozium (UNIT_CLASS_ALL).
 * x and y are the defender's position, passed on unused. */
int Player_GetDefenseBonus(int playerIndex, int defenderType, int attackerType,
                           int x, int y) {
  int bonus = 0;

  if (!data_027c027c->unk_48[data_027c0284]->coEffectsOn) {
    return bonus;
  }
  if (Ov000_gUnitStats[defenderType].unitClass == UNIT_CLASS_ALL) {
    return bonus;
  }
  bonus += Player_GetCoDefense(playerIndex, 0,
                               data_027c027c->unk_48[data_027c0284]
                                   ->players[playerIndex]
                                   .front.powerState,
                               defenderType);
  return bonus + DefenseIncreaseDirectIndirect(playerIndex, defenderType,
                                               attackerType, x, y);
}
