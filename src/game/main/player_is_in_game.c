#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* TRUE when player slot player takes part in the battle (has a controller)
 * and has not been defeated. */
BOOL Player_IsInGame(u8 player) {
  if (PLAYERS[player].controller == PLAYER_CONTROLLER_NONE) {
    return FALSE;
  }
  return PLAYERS[player].defeated == 0;
}
