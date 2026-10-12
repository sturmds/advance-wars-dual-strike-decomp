#include "types.h"

int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

/* Score of a drop tile for the order 0 and 1 routine. Bits 0-6: how far the
 * armed ground units reached are from Ai_CountEnemyGroundUnitsInRange's result
 * plus 1, capped at 127. Bit 7: they are within 5 of it (less, when
 * its result is below 5). Whether a higher score is better is not known. */
int AiSeaTransport_ScoreArmedCount(void) {
  int limit = Ai_CountEnemyGroundUnitsInRange();
  int armed = Ai_CountTeamArmedGroundUnitsInRange();
  int margin = limit > 5 ? 5 : limit;
  int flag;
  int dist;

  if (limit - margin < armed && limit + margin > armed) {
    flag = 0x80;
  } else {
    flag = 0;
  }
  if (limit > armed) {
    dist = limit - armed + 1;
  } else {
    dist = armed - limit + 1;
  }
  if (dist & 0x80) {
    dist = 0x7f;
  }
  return dist | flag;
}
