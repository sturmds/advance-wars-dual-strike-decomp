#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

/* For each player still in the battle, clears the defeated flag of every
 * other player on the same team who has a controller. */
void Players_ClearTeammatesDefeated(void) {
  int i;
  int j;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (Player_IsInGame(i)) {
      for (j = 1; j < ARMY_SLOT_COUNT; j++) {
        if (j != i && MAP_STATE->players[j].controller != 0 &&
            MAP_STATE->players[j].team == MAP_STATE->players[i].team) {
          MAP_STATE->players[j].defeated = 0;
        }
      }
    }
  }
}
