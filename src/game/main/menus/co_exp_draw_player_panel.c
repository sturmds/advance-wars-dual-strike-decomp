#include "types.h"
#include "game/co_exp.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void CoExp_DrawCoCell(CoExp* exp, int x, int y, int coSlot, int frame);

/* Draws the results panel of one player: a banner sprite, then the cells
 * of the player's front and back CO (exp[player * 2] and the next entry),
 * 112 pixels apart. */
void CoExp_DrawPlayerPanel(CoExp* exp, int x, int y, int player, int frame) {
  Sprite_QueueAt(7, (x + 112) & 0x1ff, (y + 2) & 0xff, &data_027c0060, 0xb000,
                 0x2f4 + player * 8);
  CoExp_DrawCoCell(exp, x + 24, y + 16, player * 2, frame);
  CoExp_DrawCoCell(exp, x + 136, y + 16, player * 2 + 1, frame);
}
