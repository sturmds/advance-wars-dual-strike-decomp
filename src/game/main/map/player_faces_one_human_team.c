#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when player is not human-controlled and every human player in the
 * game who is not on player's team is on one team (or there is none). */
BOOL Player_FacesOneHumanTeam(u16 player) {
  int i;
  int found;
  int humanTeam;

  if (STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
    return FALSE;
  }
  humanTeam = 0;
  found = 0;
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (i == player || !Player_IsInGame(i)) {
      continue;
    }
    if (found == 0) {
      if (STATE->players[i].controller == PLAYER_CONTROLLER_HUMAN &&
          STATE->players[i].team != STATE->players[player].team) {
        humanTeam = STATE->players[i].team;
        found++;
      }
    } else {
      if (STATE->players[i].controller == PLAYER_CONTROLLER_HUMAN &&
          humanTeam != STATE->players[i].team &&
          STATE->players[i].team != STATE->players[player].team) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
