#include "types.h"
#include "game/Unk_02080440.h"

void PowerPanel_DrawPlayer(int x, int y, int player);

/* Draws the current player's power panel at x, y 0. */
void PowerPanel_DrawCurrentPlayer(int x) {
  PowerPanel_DrawPlayer(x, 0, MAP_STATE->currentPlayer);
}
