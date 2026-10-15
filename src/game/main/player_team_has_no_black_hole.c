#include "types.h"
#include "game/Unk_02080440.h"

/* FALSE when the current player or a player on its team has the Black Hole
 * army colour. AiTurn_Start copies the result into bit 3 of
 * gAiTurnState->flags. */
BOOL Player_TeamHasNoBlackHole(void) {
  int player;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].armyColour ==
      ARMY_BLACK_HOLE) {
    return FALSE;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
            MAP_STATE->players[player].team &&
        MAP_STATE->players[player].armyColour == ARMY_BLACK_HOLE) {
      return FALSE;
    }
  }
  return TRUE;
}
