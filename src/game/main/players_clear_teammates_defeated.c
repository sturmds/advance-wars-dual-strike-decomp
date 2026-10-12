#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);

/* For each player still in the battle, clears the defeated flag of every
 * other player on the same team who has a controller. */
void Players_ClearTeammatesDefeated(void) {
  int i;
  int j;

  for (i = 1; i < 5; i++) {
    if (Player_IsInGame(i)) {
      for (j = 1; j < 5; j++) {
        if (j != i && STATE->players[j].controller != 0 &&
            STATE->players[j].team == STATE->players[i].team) {
          STATE->players[j].defeated = 0;
        }
      }
    }
  }
}
