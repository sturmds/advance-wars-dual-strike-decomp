#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/range_spread.h"
#include "game/task.h"

void Task_Destroy(Task* task);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void SpriteCells_Draw3d(void* data, VecFx32* pos, u32 a2, u32 a3, int a4,
                        int a5);
int func_020224ec(void);
void func_020f78b8(int a0);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_02169d8c[];
extern u8 data_02169d94[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Update of the range spread task: moves each dot one step and draws it.
 * The task ends after RANGE_SPREAD_FRAMES frames and starts the task of
 * func_020f78b8. On the screen whose state is data_027c0288.unk_00 the dots
 * go through the 3D quad path, otherwise they are sprites, and only up to 16
 * of them. */
void RangeSpread_Update(Task* task) {
  s16 i;
  RangeSpreadDot* dots;
  VecFx32 pos;

  dots = task->unk_4c;
  if (task->unk_66 == RANGE_SPREAD_FRAMES) {
    func_020f78b8(1);
    Task_Destroy(task);
    return;
  }
  task->unk_66++;
  if (data_027c027c == data_027c0288.unk_00) {
    if (RANGE_SPREAD_COUNT(task) > RANGE_SPREAD_MAX_DOTS) {
      return;
    }
    func_020224ec();
  } else if (RANGE_SPREAD_COUNT(task) > 16) {
    return;
  }
  for (i = 0; i < RANGE_SPREAD_COUNT(task); i++) {
    dots[i].x += dots[i].vx;
    dots[i].y += dots[i].vy;
    if (data_027c027c == data_027c0288.unk_00) {
      /* pixels to fx32: 2.0 per 256 and 6.0 per 768, both 1.0 per 128 */
      pos.x = dots[i].x * 0x2000L / 256;
      pos.y = dots[i].y * 0x6000L / 768;
      pos.z = 8;
      SpriteCells_Draw3d(data_02169d94, &pos, data_022a7c5c->unk_38,
                         data_022a7c5c->unk_3c, 0, 0);
    } else {
      Sprite_QueueAt(0, dots[i].x - GAME->unk_1c4->scrollX,
                     dots[i].y - GAME->unk_1c4->scrollY, data_02169d8c, 0, 0);
    }
  }
}
