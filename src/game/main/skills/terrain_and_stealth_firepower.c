#include "types.h"

int TerrainFirepower(int playerIndex, int unitType, int x, int y);
int StealthFirepower(int playerIndex, BOOL dived);

/* Firepower a unit gets from its tile plus the Backstab bonus.
 * `column` (the damage chart column) is not used. */
int TerrainAndStealthFirepower(int playerIndex, int unitType, int column, int x,
                               int y, BOOL dived) {
  int firepower = 0; /* needed to match: start at 0, then add */

  firepower += TerrainFirepower(playerIndex, unitType, x, y);
  return firepower + StealthFirepower(playerIndex, dived);
}
