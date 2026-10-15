#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Unk_02031a14* task);
#define DIVISOR(scale) ((scale) ? (scale) : 2)

void func_0204923c(Unk_02031a14* task) {
  int scale = Math_Ease(0, 256, 16, task->frame, 4);

  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue = gSineTable[0];
    int cosValue = gSineTable[64] * 16;

    Oam_SetAffineParams(task->matrix, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  Sprite_Queue(1, ((task->x - 32) & 0x1ff) | (task->matrix << 9),
               ((task->y - 48) & 0xff) | 0x300, &data_027c0030, 0x4000, 0x1b4);
  if (task->frame != 4) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
