#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE when a player still in the game is on player 1's team. Player 1
 * counts itself, so it is also TRUE whenever player 1 is in the game. */
BOOL Player1Team_HasPlayerInGame(void) {
  u8 i;
  u8 count = 0;

  for (i = 1; i <= 4; i++) {
    if (Player_IsInGame(i)) {
      if (data_027c027c->unk_48[data_027c0284]->players[1].team ==
          data_027c027c->unk_48[data_027c0284]->players[i].team) {
        count++;
      }
    }
  }
  return count != 0;
}
