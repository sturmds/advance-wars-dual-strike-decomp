#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void PowerPanel_DrawPlayer(int x, int y, int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Draws the current player's power panel at x, y 0. */
void PowerPanel_DrawCurrentPlayer(int x) {
  PowerPanel_DrawPlayer(x, 0, STATE->currentPlayer);
}
