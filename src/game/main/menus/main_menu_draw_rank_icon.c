#include "types.h"

void Gx_DrawTexturedQuadFormat(int x, int y, int a2, int a3, int a4, int a5,
                               int a6, int a7, int a8, int a9);

/* Draws one rank icon beside a page 11 entry; icon picks the texture. */
void MainMenu_DrawRankIcon(int x, int y, int icon) {
  Gx_DrawTexturedQuadFormat(x, y, -1, 1, 1, 3, (icon * 4 + 0xd08) * 32, 0x260,
                            0, 0);
}
