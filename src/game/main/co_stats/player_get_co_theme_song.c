#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Theme song id of a player's front CO. */
int Player_GetCoThemeSong(int playerIndex) {
  int co = data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
           PLAYER_CO_ID_MASK;

  return gCoTable[co].theme;
}
