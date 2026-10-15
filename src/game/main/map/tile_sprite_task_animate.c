#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/tile_sprite_task.h"

void Task_ResumeScript(void* task);
BOOL Sprite_IsOnScreen(int x, int y, int size);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
void SpriteCells_Draw3d(void* data, VecFx32* pos, void* chars, void* palette,
                        int a4, int a5);

extern u8 data_02166eb4[];

/* Draws the task's sprite at its tile and steps its animation: 16 sprite
 * tiles on at frames 3, 6, 9, 12, 14, 16, 18 and 20. At frame 20 the
 * timer restarts and the script resumes, with nothing drawn. In 3D the sprite tile is taken
 * as an offset from tile 0x194 into the graphics at data_022a7c5c->unk_68. */
void TileSpriteTask_Animate(TileSpriteTask* task) {
  VecFx32 pos;

  switch (task->timer) {
    case 3:
    case 6:
    case 9:
    case 12:
    case 14:
    case 16:
    case 18:
      task->tile += 16;
      break;
    case 20:
      task->tile += 16;
      task->timer = 0;
      Task_ResumeScript(task);
      return;
  }
  if (data_027c027c == data_027c0288.unk_00) {
    pos.x = ((task->x * 16 + 8) << 13) / 256;
    pos.y = (task->y * 16 + 8) * 0x6000 / 0x300;
    pos.z = 8;
    SpriteCells_Draw3d(
        data_02166eb4, &pos,
        (u8*)data_022a7c5c->unk_68 + ((task->tile & 0x3ff) - 0x194) * 32,
        data_022a7c5c->unk_6c, 0, 1);
  } else if (Sprite_IsOnScreen(task->x * 16 - MAP_STATE->unk_1c4->scrollX,
                               task->y * 16 - MAP_STATE->unk_1c4->scrollY,
                               64)) {
    Oam_PutSprite((task->x * 16 - MAP_STATE->unk_1c4->scrollX + 8) & 0x1ff,
                  (task->y * 16 - MAP_STATE->unk_1c4->scrollY + 8) & 0xff,
                  data_02166eb4, task->tile);
  }
  task->timer++;
}
