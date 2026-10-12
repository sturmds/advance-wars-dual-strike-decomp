#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void EventScript_StartText(u32 script, int arg1, int arg2);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Starts a text script for the current player's front CO. */
void CoQuote_StartText(u32 script) {
  EventScript_StartText(script, GAME->players[GAME->currentPlayer].front.id, 0);
}
