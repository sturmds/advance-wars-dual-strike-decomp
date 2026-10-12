#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* Counts a kill for the player this turn (unk_1c) and keeps the best turn
 * total (unk_1e). Combat_ApplyResult passes two more arguments that this
 * routine does not read. */
void Player_AddKill(int player) {
  PLAYERS[player].turnKills++;
  if (PLAYERS[player].turnKills > PLAYERS[player].mostTurnKills) {
    PLAYERS[player].mostTurnKills = PLAYERS[player].turnKills;
  }
}
