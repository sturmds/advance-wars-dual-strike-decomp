#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern u8 data_027c0040[];
extern u8 data_027c0108[];

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Draws two sprites on the first screen at y 172; the tile of the first one comes from unk_52. */
void func_0208854c(Task* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Sprite_Queue(2, 172, 172, data_027c0040, 0x1000, task->unk_52 * 4 + 240);
  Sprite_Queue(2, 188, 172, data_027c0108, 0x1000, 276);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
