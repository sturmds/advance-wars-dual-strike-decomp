#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/player_tag_power.h"
#include "game/unit_stats.h"

int Player_GetCoFirepower(int playerIndex, int slot, int powerState,
                          int unitType);
int Player_GetTagFirepower(int playerIndex, BOOL tagPower);

/* Player_GetFirepowerBonus without the flat bonus of a running power: the CO's
 * firepower percent plus the tag partner's during a tag power. 0 with coEffectsOn
 * clear, and for Oozium (UNIT_CLASS_ALL). */
int Player_GetSlotFirepower(int playerIndex, int slot, int powerState,
                            int unitType, int tagPower) {
  int firepower = 0;

  if (!data_027c027c->unk_48[data_027c0284]->coEffectsOn) {
    return firepower;
  }
  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_ALL) {
    return firepower;
  }
  firepower += Player_GetCoFirepower(playerIndex, slot, powerState, unitType);
  if (Player_TagPowerOn(playerIndex)) {
    firepower += Player_GetTagFirepower(playerIndex, tagPower);
  }
  return firepower;
}
