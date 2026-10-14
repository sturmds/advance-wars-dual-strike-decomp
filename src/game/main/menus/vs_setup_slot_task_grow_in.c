#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/vs_setup_slot_task.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern MenuState gMenuState;
extern u8 data_027c0060[];
extern s16 gSineTable[]; /* index 64 = cosine of 0 */

/* Divisor for one matrix entry: the scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Step of the player slot task scripts (gVsSetupSlot0TaskScript and the next three).
 * Over frames 0 to 3 it draws sprite tile 0x634 through affine matrix
 * task->matrix while the scale eases from 512 to 256, so the sprite grows from
 * half to full size. From frame 4 on it draws the sprite without the matrix
 * and waits for VsSetup_ResumePlayerTask. Nothing is drawn while
 * setup.unk_2ea is 2. */
void VsSetupSlotTask_GrowIn(VsSetupSlotTask* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (task->frame >= 4) {
    if (gMenuState.setup.unk_2ea != 2) {
      Sprite_Queue(0, task->x & 0x1ff, task->y & 0xff, &data_027c0060, 0xe000,
                   0x634);
    }
  } else {
    if (gMenuState.setup.unk_2ea != 2) {
      int scale = Math_Ease(0, 512, 256, task->frame, 4);
      int d3 = DIVISOR(scale);
      int d2 = DIVISOR(scale);
      int d1 = DIVISOR(scale);
      int d0 = DIVISOR(scale);
      int sinValue = gSineTable[0];
      int cosValue = gSineTable[64] << 4;

      Oam_SetAffineParams(task->matrix, HwDiv32(cosValue, d0),
                          HwDiv32(-sinValue << 4, d1),
                          HwDiv32(sinValue << 4, d2), HwDiv32(cosValue, d3));
      /* 0x300 sets OAM attribute 0 bits 8-9: affine, double size */
      Sprite_Queue(0, ((task->x - 16) & 0x1ff) | (task->matrix << 9),
                   ((task->y - 8) & 0xff) | 0x300, &data_027c0060, 0xe000,
                   0x634);
    }
    task->frame++;
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
