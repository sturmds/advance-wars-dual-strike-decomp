#include "types.h"

int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamFootUnitsInRange(void);
int Ai_TeamFootOrEnemyVehicleInRange(void);

/* Score of a drop tile for a transport carrying foot units or an APC. Bits
 * 0-6: enemy property value reached plus 2, minus the team's foot units
 * reached (127 when that is negative or above 127). Bit 7: Ai_TeamFootOrEnemyVehicleInRange
 * (name not known yet) returns 0. Whether a higher score is better is not
 * known. */
int AiSeaTransport_ScoreFootUnitsVsProperties(void) {
  int properties = Ai_ScoreReachableEnemyProperties();
  int flag;
  int score;

  if (Ai_TeamFootOrEnemyVehicleInRange() == 0) {
    flag = 0x80;
  } else {
    flag = 0;
  }
  score = properties + 2 - Ai_CountTeamFootUnitsInRange();
  if (score & 0x80) {
    score = 0x7f;
  }
  return score | flag;
}
