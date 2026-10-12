#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Co_GetTagStars(int co, int partner);

/* Tag stars (0-3) of a player's front and back CO pair; 0 without a back
 * CO. */
int Player_GetTagStars(int playerIndex) {
  /* Needed to match: the ternary keeps the 0/1 value in a register. */
  if ((data_027c027c->unk_48[data_027c0284]->players[playerIndex].back.id
           ? TRUE
           : FALSE) == FALSE) {
    return 0;
  }
  return Co_GetTagStars(
      data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
          PLAYER_CO_ID_MASK,
      data_027c027c->unk_48[data_027c0284]->players[playerIndex].back.id &
          PLAYER_CO_ID_MASK);
}
