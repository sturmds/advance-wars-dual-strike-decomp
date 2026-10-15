#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

BOOL Player_IsInGame(u8 player);

/* TRUE when player is not human-controlled and every human player in the
 * game who is not on player's team is on one team (or there is none). */
BOOL Player_FacesOneHumanTeam(u16 player) {
  int i;
  int found;
  int humanTeam;

  if (MAP_STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
    return FALSE;
  }
  humanTeam = 0;
  found = 0;
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (i == player || !Player_IsInGame(i)) {
      continue;
    }
    if (found == 0) {
      if (MAP_STATE->players[i].controller == PLAYER_CONTROLLER_HUMAN &&
          MAP_STATE->players[i].team != MAP_STATE->players[player].team) {
        humanTeam = MAP_STATE->players[i].team;
        found++;
      }
    } else {
      if (MAP_STATE->players[i].controller == PLAYER_CONTROLLER_HUMAN &&
          humanTeam != MAP_STATE->players[i].team &&
          MAP_STATE->players[i].team != MAP_STATE->players[player].team) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
