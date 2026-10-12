#include "types.h"

int AiSite_PickBlackBombAirportMostEnemies(void);
int AiSite_PickBlackBombAirportNearestEnemy(void);

/* Picks the airport site to build a Black Bomb at: the one with the most
 * enemies near it, or when none qualifies, the one nearest an enemy.
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirport(void) {
  int index;

  index = AiSite_PickBlackBombAirportMostEnemies();
  if (index != -1) {
    return index;
  }
  return AiSite_PickBlackBombAirportNearestEnemy();
}
