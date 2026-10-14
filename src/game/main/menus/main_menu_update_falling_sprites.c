#include "types.h"
#include "game/falling_sprites.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern FallingSpriteFrame data_021599e4[];
extern FallingSpriteFrame data_021599f8[];

#define FRAME_FIELD(sprite, field)                            \
  ((sprite)->fast == 0 ? data_021599e4[(sprite)->frame].field \
                       : data_021599f8[(sprite)->frame].field)

/* Task callback of the main menu (script data_02159a0c): draws the 16
 * falling sprites on whichever screen they are over, steps their
 * animation, and moves them down and to the left: one pixel left and one
 * or two down on even frames of the timer, one down on odd frames. */
void MainMenu_UpdateFallingSprites(FallingSpriteTask* task) {
  FallingSprite* sprite;
  int i;

  sprite = task->sprites;
  for (i = 0; i < FALLING_SPRITE_COUNT; i++, sprite++) {
    /* near the seam (y 249-255) a sprite is drawn on both screens */
    if (sprite->y > 56 && sprite->y < 256) {
      Unk_027c0280* savedGfx = data_027c0280;
      Unk_02080440* saved;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      Sprite_QueueAt(5, sprite->x & 0x1ff, (sprite->y - 56) & 0xff,
                     &data_027c0028, FRAME_FIELD(sprite, cell) | 0x800, 0);
      Screen_SetCurrent(saved);
      Screen_SetCurrentGfx(savedGfx);
    }
    if (sprite->y > 248 && sprite->y < 448) {
      Unk_027c0280* savedGfx = data_027c0280;
      Unk_02080440* saved;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      Sprite_QueueAt(5, sprite->x & 0x1ff, (sprite->y - 256) & 0xff,
                     &data_027c0028, FRAME_FIELD(sprite, cell) | 0x800, 0);
      Screen_SetCurrent(saved);
      Screen_SetCurrentGfx(savedGfx);
    }
    sprite->timer++;
    if (sprite->timer == FRAME_FIELD(sprite, duration)) {
      sprite->timer = 0;
      sprite->frame++;
      if (FRAME_FIELD(sprite, duration) == 0) {
        sprite->frame = 0;
      }
    }
    sprite->x += (sprite->timer & 1) ? 0 : -1;
    sprite->y += (sprite->timer & 1) ? 1 : (sprite->fast == 0 ? 1 : 2);
  }
}
