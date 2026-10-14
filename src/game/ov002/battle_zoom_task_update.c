#include "types.h"
#include "game/oam_cells.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/BattleZoomTask.h"

extern MenuState gMenuState;
extern u8 data_ov002_023696ac;
extern s16 gSineTable[];

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Palette_Load(const void* src, u32 offset, u32 size);
BOOL Battle_IsSideAir(int side);

static inline void QueuePair(int x, int y, int attr1, int attr2) {
  Sprite_Queue(1, x, y, &data_027c0008, attr1, 0);
  Sprite_Queue(2, x, y, &data_027c0008, attr2, 0);
}

#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* One frame of the zoom task. While shake is set, draws data_ov002_023696ac at the screen
 * centre moved by the shake pattern, and every 4 frames copies the next colour of
 * BattleScreenWork.unk_fd0 into the palette. Then eases affine matrix 1 from scale 2 to 0x100
 * over 8 frames and draws a sprite pair through it at each side that data_0229fb30.unk_06
 * marks: lower on the screen for a ground unit, higher for an air unit. */
void BattleZoomTask_Update(BattleZoomTask* task) {
  int scale;
  int x;
  int y;

  if (task->shake != 0) {
    x = 0;
    y = 0;
    if (task->shakeStep <= 8) {
      switch (task->shakeStep) {
        case 0:
          x = 0;
          y = 0;
          break;
        case 1:
          x = task->shakeSize;
          y = -x;
          break;
        case 2:
          y = task->shakeSize;
          x = -y;
          break;
        case 3:
          x = task->shakeSize;
          y = x;
          break;
        case 4:
          x = -task->shakeSize;
          y = x;
          break;
        case 5:
          x = task->shakeSize;
          break;
        case 6:
          y = -task->shakeSize;
          break;
        case 7:
          x = -task->shakeSize;
          break;
        case 8:
          y = task->shakeSize;
          break;
      }
      task->shakeStep++;
    }
    Sprite_QueueAt(0, (x + 128) & 0x1ff, (y + 96) & 0xff, &data_ov002_023696ac,
                   0, 0);
    if ((task->frame & 3) == 0) {
      /* Needed to match: through a cast, the 0xfd0 is added at run time. */
      Palette_Load(
          &((MenuState*)&gMenuState)->battle.unk_fd0[task->paletteStep & 15],
          (u16)(task->paletteSlot * 2 + 0x200), 2);
      task->paletteStep++;
    }
  }
  if (task->frame > 8) {
    scale = 0x100;
  } else {
    scale = Math_Ease(0, 2, 0x100, task->frame, 8);
  }
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int cosValue = gSineTable[64] * 16;
    int sinValue = gSineTable[0];

    Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                        HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
  }
  /* Needed to match: scale and x, y are reused for the side sprites. */
  scale = ((0x100 - scale) << 5) >> 8;
  if (data_0229fb30.unk_06[0]) {
    if (!(Battle_IsSideAir(0) ? TRUE : FALSE)) {
      y = ((40 - (scale >> 1)) & 0xff) | 0x100;
      x = ((32 - scale) & 0x1ff) | 0x200;
      QueuePair(x, y, 0x100, 0xc0);
    } else {
      y = ((120 + (scale >> 1)) & 0xff) | 0x100;
      x = ((32 - scale) & 0x1ff) | 0x200;
      QueuePair(x, y, 0x100, 0xc0);
    }
  }
  if (data_0229fb30.unk_06[1]) {
    if (!(Battle_IsSideAir(1) ? TRUE : FALSE)) {
      y = ((40 - (scale >> 1)) & 0xff) | 0x100;
      x = ((scale + 160) & 0x1ff) | 0x200;
      QueuePair(x, y, 0x120, 0xe0);
    } else {
      y = ((120 + (scale >> 1)) & 0xff) | 0x100;
      x = ((scale + 160) & 0x1ff) | 0x200;
      QueuePair(x, y, 0x120, 0xe0);
    }
  }
  task->frame++;
}
