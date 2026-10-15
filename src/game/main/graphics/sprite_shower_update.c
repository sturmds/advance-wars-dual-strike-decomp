#include "types.h"
#include "game/falling_sprites.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(void* task);

extern FallingSpriteFrame data_02159c50[];
extern FallingSpriteFrame data_02159c64[];

#define FRAME_FIELD(sprite, field)                               \
  ((sprite)->variant == 0 ? data_02159c64[(sprite)->frame].field \
                          : data_02159c50[(sprite)->frame].field)

/* Task callback of script data_02159ca8: draws the 16 shower sprites on
 * whichever screen they are over and moves them down and to the left, like
 * MainMenu_UpdateFallingSprites. A sprite stops once it has left both
 * screens; when none is left, the task's script resumes. */
void SpriteShower_Update(ShowerSpriteTask* task) {
  int i;
  BOOL anyActive;
  Unk_027c0280* savedGfx;
  ShowerSprite* sprite;
  Unk_02080440* saved;

  anyActive = FALSE;
  sprite = task->sprites;
  for (i = 0; i < FALLING_SPRITE_COUNT; sprite++, i++) {
    if (!sprite->active) {
      continue;
    }
    anyActive = TRUE;
    if (sprite->y > 312 && sprite->y < 512) {
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      Sprite_QueueAt(0, (sprite->x - 512) & 0x1ff, (sprite->y - 312) & 0xff,
                     &data_027c0028, FRAME_FIELD(sprite, cell) * 2 + 0x1c8, 0);
      Screen_SetCurrent(saved);
      Screen_SetCurrentGfx(savedGfx);
    }
    if (sprite->y > 504 && sprite->y < 704) {
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      Sprite_QueueAt(0, (sprite->x - 512) & 0x1ff, (sprite->y - 512) & 0xff,
                     &data_027c0028, FRAME_FIELD(sprite, cell) * 2 + 0x1c8, 0);
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
    sprite->y += (sprite->timer & 1) ? 1 : (sprite->variant == 0 ? 1 : 2);
    if (sprite->x <= 504 || sprite->y >= 704) {
      sprite->active = 0;
    }
  }
  if (!anyActive) {
    Task_ResumeScript(task);
  }
}
