#include "types.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues an icon at y 8 for each CO of each player that has coShown set:
 * 40 pixels per player, 16 per CO, tile picked by the player's army. */
void PlayerInfo_DrawCoIcons(void) {
  int player;
  int co;

  for (player = 0; player < gMenuState.unk_0206d67c.playerCount; player++) {
    for (co = 0; co < (int)ARRAY_COUNT(gMenuState.unk_0206d67c.coShown[0]);
         co++) {
      if (gMenuState.unk_0206d67c.coShown[player][co] != 0) {
        Sprite_QueueAt(
            1, (24 + player * 40 + co * 16) & 0x1ff, 8, &data_027c0040, 0x1400,
            12 + (gMenuState.unk_0206d67c.playerArmy[player] - 1) * 4);
      }
    }
  }
}
