#include "types.h"
#include "game/Unk_02080440.h"
#include "game/oam_cells.h"

u8 Map_GetLoadedPlayerCount(void);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Status screen sprites: one at the top, then one on each undefeated
 * player's row (rows start at y 80, 16 pixels apart, as StatusScreen_Draw lays them out). */
void StatusScreen_QueueSprites(void) {
  int i;

  Sprite_QueueAt(0, 8, 32, &data_027c0040, 0x51b4, 0);
  for (i = 0; i < Map_GetLoadedPlayerCount(); i++) {
    if (MAP_STATE->players[i + 1].defeated == 0) {
      Sprite_QueueAt(0, 8, (80 + i * 16) & 0xff, &data_027c0060,
                     (404 + i * 8) | 0x6000, 0);
    }
  }
}
