#include "types.h"
#include "game/Unk_02080440.h"

/* propertyGoal (from the map header) is a property count goal (guess: it ends the battle); 0: no
 * such goal. Returns the first player slot that owns that many, or 0. */
int Players_FindPropertyGoalWinner(void) {
  u8 player;

  if (MAP_STATE->propertyGoal == 0) {
    return 0;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (MAP_STATE->players[player].propertyCount >= MAP_STATE->propertyGoal) {
      return player;
    }
  }
  return 0;
}
