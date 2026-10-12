#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/vs_setup_slot_task.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(void* task);

extern MenuState gMenuState;
extern u8 data_027c0060;
extern s16 gSineTable[]; /* index 64 = cosine of 0 */

/* Divisor for one matrix entry: the scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Step of the player slot task scripts (gVsSetupSlot0TaskScript and the next three),
 * run after the task is resumed. Over frames 0 to 4 it draws sprite tile
 * 0x634 through affine matrix task->matrix while the scale eases from 256 to 2:
 * the sprite doubles in width and flattens to a line. Nothing is drawn while
 * setup.unk_2ea is 2. Then it resumes the script. */
void VsSetupSlotTask_SquashOut(VsSetupSlotTask* task) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (task->frame > 4) {
    Task_ResumeScript(task);
  } else {
    if (gMenuState.setup.unk_2ea != 2) {
      int scale = Math_Ease(1, 256, 2, task->frame, 4);
      int d3 = DIVISOR(scale);
      int d2 = DIVISOR(512 - scale);
      int d1 = DIVISOR(scale);
      int d0 = DIVISOR(512 - scale);
      int sinValue = gSineTable[0];
      int cosValue = gSineTable[64] << 4;

      Oam_SetAffineParams(task->matrix, HwDiv32(cosValue, d0),
                          HwDiv32(-sinValue << 4, d1),
                          HwDiv32(sinValue << 4, d2), HwDiv32(cosValue, d3));
      Sprite_Queue(0, ((task->x - 16) & 0x1ff) | (task->matrix << 9),
                   ((task->y - 8) & 0xff) | 0x300, &data_027c0060, 0xe000,
                   0x634);
    }
    task->frame++;
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
