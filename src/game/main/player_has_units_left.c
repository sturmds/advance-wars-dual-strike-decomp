#include "types.h"

BOOL Player_IsInGame(u8 player);
int Player_CountUnitsBuilt(int player);
BOOL Player_HasAnyUnit(int player);

/* Whether the player is still in the battle by unit count: FALSE when out
 * of the game, TRUE when no unit has been built yet, else Player_HasAnyUnit
 * (a unit in slots 1 to 50). */
BOOL Player_HasUnitsLeft(int player) {
  if (!Player_IsInGame(player)) {
    return FALSE;
  }
  if (Player_CountUnitsBuilt(player) == 0) {
    return TRUE;
  }
  return Player_HasAnyUnit(player);
}
