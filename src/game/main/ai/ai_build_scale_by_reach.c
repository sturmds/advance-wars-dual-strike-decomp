#include "types.h"
#include "game/unit_stats.h"

int UnitType_GetAttackKind(u8 unitType);
int CurrentPlayer_GetUnitMaxRange(int unitType, int powerState);
int CurrentPlayer_GetUnitMoveRange(int unitType, int powerState);

/* Scales a build score by how far the unit type reaches, in eighths: its
 * max range plus 4 less its min range for indirect units, its movement for
 * the rest (both with the current player's CO bonuses). Recon counts half. */
int AiBuild_ScaleByReach(int score, int unitType, int powerState) {
  if (UnitType_GetAttackKind(unitType) == ATTACK_KIND_INDIRECT) {
    score = score * (CurrentPlayer_GetUnitMaxRange(unitType, powerState) + 4 -
                     Ov000_gUnitStats[unitType].minRange) >>
            3;
  } else {
    score = score * CurrentPlayer_GetUnitMoveRange(unitType, powerState) >> 3;
  }
  if (unitType == UNIT_RECON) {
    score >>= 1;
  }
  return score;
}
