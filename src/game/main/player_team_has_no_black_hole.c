#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* FALSE when the current player or a player on its team has the Black Hole
 * army colour. AiTurn_Start copies the result into bit 3 of
 * gAiTurnState->flags. */
BOOL Player_TeamHasNoBlackHole(void) {
  int player;

  if (STATE->players[STATE->currentPlayer].armyColour == ARMY_BLACK_HOLE) {
    return FALSE;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (STATE->players[STATE->currentPlayer].team ==
            STATE->players[player].team &&
        STATE->players[player].armyColour == ARMY_BLACK_HOLE) {
      return FALSE;
    }
  }
  return TRUE;
}
