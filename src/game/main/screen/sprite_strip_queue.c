#include "types.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern MenuState gMenuState;

/* Queues one sprite per x position in MenuState.colours (low 9 bits), with tiles 16 apart from
 * tile 0x90, on sprite list 2. With affineIndex 0 the sprites are plain; with
 * 1-31 they are rotation/scaling sprites that use that OBJ affine matrix
 * (OAM attribute 1 bits 9-13; attribute 0 bit 8 is set through y + 0x100). */
void SpriteStrip_Queue(int affineIndex) {
  int i;

  for (i = 0; i < gMenuState.spriteStrip.spriteCount; i++) {
    if (affineIndex != 0) {
      Sprite_Queue(2, (affineIndex << 9) | ((s16)gMenuState.colours[i] & 0x1ff),
                   0x160, &data_027c0048, 0x1400, 0x90 + i * 16);
    } else {
      Sprite_QueueAt(2, (s16)gMenuState.colours[i] & 0x1ff, 0x60,
                     &data_027c0048, 0x1000, 0x90 + i * 16);
    }
  }
}
