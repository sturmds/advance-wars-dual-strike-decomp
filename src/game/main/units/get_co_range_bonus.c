#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

int UnitType_GetMaxRange(int unitType);
int GetUnitCombatClass(int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Change to a unit type's firing range from one CO of a player (coSlot 0:
 * front, 1: back) in the given power state. Foot units take only their
 * class row; other units add the class row and the combat row. Sandstorm
 * takes 1 off indirect units, but their range never drops below 2.
 * Oozium (UNIT_CLASS_ALL) gets no change. */
int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType) {
  int co =
      (&data_027c027c->unk_48[data_027c0284]->players[player].front)[coSlot]
          .id &
      PLAYER_CO_ID_MASK;
  int baseRange = UnitType_GetMaxRange(unitType);
  int bonus = 0;

  if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_ALL) {
    return 0;
  }

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn) {
    const CoStatRow* classRow =
        gCoTable[co]
            .blocks[powerState]
            .statRows[Ov000_gUnitStats[unitType].unitClass];
    const CoStatRow* combatRow =
        gCoTable[co].blocks[powerState].statRows[GetUnitCombatClass(unitType)];

    if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_FOOT) {
      bonus += classRow->range;
    } else {
      bonus += classRow->range + combatRow->range;
    }
  }

  if (baseRange >= 2) {
    if (data_027c027c->unk_48[data_027c0284]->unk_1c4->weather ==
        WEATHER_SAND) {
      bonus -= 1;
    }
    if (baseRange + bonus < 2) {
      bonus = 2 - baseRange;
    }
  }
  return bonus;
}
