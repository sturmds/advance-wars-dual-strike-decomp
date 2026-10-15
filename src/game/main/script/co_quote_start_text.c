#include "types.h"
#include "game/Unk_02080440.h"

void EventScript_StartText(u32 script, int arg1, int arg2);

/* Starts a text script for the current player's front CO. */
void CoQuote_StartText(u32 script) {
  EventScript_StartText(
      script, MAP_STATE->players[MAP_STATE->currentPlayer].front.id, 0);
}
